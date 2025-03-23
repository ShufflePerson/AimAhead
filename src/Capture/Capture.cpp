#include "capture.h"

namespace capture {
    IDXGIFactory1* g_factory = nullptr;
    IDXGIAdapter* g_adapter = nullptr;
    IDXGIOutput* g_output = nullptr;
    IDXGIOutput1* g_output1 = nullptr;
    ID3D11Device* g_device = nullptr;
    ID3D11DeviceContext* g_context = nullptr;
    IDXGIOutputDuplication* g_desktopDuplication = nullptr;
    ID3D11Texture2D* g_stagingTexture = nullptr;
    ID3D11Texture2D* g_cudaTexture = nullptr;
    cudaGraphicsResource* g_cudaResource = nullptr;

  

    int g_screenWidth = 0;
    int g_screenHeight = 0;

    const int captureWidth = 640;
    const int captureHeight = 640;

    std::atomic<std::shared_ptr<cv::cuda::GpuMat>> g_matCachePtr;
    std::atomic<bool> g_shouldUpdateCache = true;
    std::atomic<bool> g_waitingForNewFrame = false;

    bool should_update() {
        return g_shouldUpdateCache.load(std::memory_order_relaxed);
    }

    void set_should_update(bool val) {
        g_shouldUpdateCache.store(val, std::memory_order_release);
    }

    void capture_cache_loop() {
        while (true) {
            if (!should_update()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            cv::cuda::GpuMat new_mat(640, 640, CV_8UC3);
            g_waitingForNewFrame.store(true, std::memory_order_release);
            captureScreenRegionGPU(new_mat);
            std::shared_ptr<cv::cuda::GpuMat> new_mat_ptr = std::make_shared<cv::cuda::GpuMat>();
            new_mat.copyTo(*new_mat_ptr);
            g_matCachePtr.store(new_mat_ptr, std::memory_order_release);
            g_waitingForNewFrame.store(false, std::memory_order_release);
        }
    }

    bool get_cached_mat(cv::cuda::GpuMat& dst) {
        if (g_waitingForNewFrame.load(std::memory_order_relaxed) == true) return false;
        std::shared_ptr<cv::cuda::GpuMat> current_mat_ptr = g_matCachePtr.load(std::memory_order_acquire);
        if (current_mat_ptr->empty()) return false;
        if (current_mat_ptr) {
            current_mat_ptr->copyTo(dst);
            return true;
        }
        return false;
    }

    cv::cuda::GpuMat convertTo3Channels(const cv::cuda::GpuMat& input) {
        if (input.type() == CV_8UC4) {
            cv::cuda::GpuMat channels[4];
            cv::cuda::split(input, channels);  

            cv::cuda::GpuMat rgb; 
            cv::cuda::merge(channels, 3, rgb); 

            return rgb;  
        }
        else {
            throw std::runtime_error(XorStr("Input is not CV_8UC4"));
        }
    }


    bool initializeCUDAResources() {
        D3D11_TEXTURE2D_DESC desc = {
            static_cast<UINT>(captureWidth),
            static_cast<UINT>(captureHeight),
            1, 1,
            DXGI_FORMAT_B8G8R8A8_UNORM,
            {1, 0},
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0,
            0
        };


        HRESULT hr = g_device->CreateTexture2D(&desc, nullptr, &g_cudaTexture);
        if (FAILED(hr)) {
            std::cerr << XorStr("Failed to create CUDA texture: ") << std::hex << hr << std::endl;
            return false;
        }
        return true;
    }

    bool InitializeCapture() {
        HRESULT hr;
        cudaError_t cudaStatus = cudaSetDevice(0); 
        if (cudaStatus != cudaSuccess) {
            std::cerr << "cudaSetDevice(0) failed: " << cudaGetErrorString(cudaStatus) << std::endl;
            return false;
        }
        hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&g_factory);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error creating DXGIFactory: ") << std::hex << hr << std::endl;
            return false;
        }

        hr = g_factory->EnumAdapters(0, &g_adapter);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error enumerating adapter: ") << std::hex << hr << std::endl;
            return false;
        }

        hr = g_adapter->EnumOutputs(0, &g_output);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error enumerating output: ") << std::hex << hr << std::endl;
            return false;
        }

        DXGI_OUTPUT_DESC outputDesc;
        g_output->GetDesc(&outputDesc);
        g_screenWidth = outputDesc.DesktopCoordinates.right - outputDesc.DesktopCoordinates.left;
        g_screenHeight = outputDesc.DesktopCoordinates.bottom - outputDesc.DesktopCoordinates.top;
        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        hr = D3D11CreateDevice(
            g_adapter,
            D3D_DRIVER_TYPE_UNKNOWN,
            nullptr,
            0,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &g_device,
            nullptr,
            &g_context
        );
        if (FAILED(hr)) {
            std::cerr << XorStr("Error creating D3D11 device: ") << std::hex << hr << std::endl;
            return false;
        }

        hr = g_output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&g_output1);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error getting IDXGIOutput1 interface: ") << std::hex << hr << std::endl;
            return false;
        }

        hr = g_output1->DuplicateOutput(g_device, &g_desktopDuplication);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error creating desktop duplication: ") << std::hex << hr << std::endl;
            return false;
        }

        HRESULT releaseHr = g_desktopDuplication->ReleaseFrame();


        DXGI_OUTDUPL_DESC deskDuplDesc;
        g_desktopDuplication->GetDesc(&deskDuplDesc);
        D3D11_TEXTURE2D_DESC stagingDesc;
        stagingDesc.Width = deskDuplDesc.ModeDesc.Width;
        stagingDesc.Height = deskDuplDesc.ModeDesc.Height;
        stagingDesc.Format = deskDuplDesc.ModeDesc.Format;
        stagingDesc.MipLevels = 1;
        stagingDesc.ArraySize = 1;
        stagingDesc.SampleDesc.Count = 1;
        stagingDesc.SampleDesc.Quality = 0;
        stagingDesc.Usage = D3D11_USAGE_STAGING;
        stagingDesc.BindFlags = 0;
        stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        stagingDesc.MiscFlags = 0;
        hr = g_device->CreateTexture2D(&stagingDesc, nullptr, &g_stagingTexture);
        if (FAILED(hr)) {
            std::cerr << XorStr("Error creating staging texture: ") << std::hex << hr << std::endl;
            return false;
        }
        initializeCUDAResources();
        spdlog::info(XorStr("D3D11 Capture initialized successfully!"));
        return true;
    }


    void _ReleaseCapture() {
        if (g_stagingTexture) g_stagingTexture->Release();
        if (g_desktopDuplication) g_desktopDuplication->Release();
        if (g_context) g_context->Release();
        if (g_device) g_device->Release();
        if (g_output1) g_output1->Release();
        if (g_output) g_output->Release();
        if (g_adapter) g_adapter->Release();
        if (g_factory) g_factory->Release();
    }


    bool captureScreenRegionGPU(cv::cuda::GpuMat& gpuMat) {
        int captureStartX = (g_screenWidth - captureWidth) / 2;
        int captureStartY = (g_screenHeight - captureHeight) / 2;
        HRESULT hr;
        IDXGIResource* tempDesktopResource = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ID3D11Texture2D* texture = nullptr;
        bool frameReleased = false;
        cudaError_t cudaStatus;
        cudaGraphicsResource_t cudaResource = nullptr;
        hr = g_desktopDuplication->AcquireNextFrame(0, &frameInfo, &tempDesktopResource);
        if (FAILED(hr)) {
            return false;
        }
        do {
            hr = tempDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
            if (FAILED(hr)) {
                break;
            }

            if (captureStartX < 0 || captureStartY < 0) {
                captureStartX = std::max(0, captureStartX);
                captureStartY = std::max(0, captureStartY);
            }

            D3D11_BOX srcBox = {
                static_cast<UINT>(captureStartX),
                static_cast<UINT>(captureStartY),
                0,
                static_cast<UINT>(captureStartX + captureWidth),
                static_cast<UINT>(captureStartY + captureHeight),
                1
            };

            g_context->CopySubresourceRegion(
                g_cudaTexture,
                0,
                0, 0, 0,
                texture,
                0,
                &srcBox
            );

            cudaStatus = cudaGraphicsD3D11RegisterResource(&cudaResource, g_cudaTexture, cudaGraphicsRegisterFlagsNone);
            if (cudaStatus != cudaSuccess) {
                break;
            }

            cudaStatus = cudaGraphicsMapResources(1, &cudaResource, 0);
            if (cudaStatus != cudaSuccess) {
                cudaGraphicsUnregisterResource(cudaResource);
                cudaResource = nullptr;
                break;
            }

            cudaArray* cuArray = nullptr;
            cudaStatus = cudaGraphicsSubResourceGetMappedArray(&cuArray, cudaResource, 0, 0);
            if (cudaStatus != cudaSuccess) {
                cudaGraphicsUnmapResources(1, &cudaResource, 0);
                cudaGraphicsUnregisterResource(cudaResource);
                cudaResource = nullptr;
                break;
            }

            cv::cuda::GpuMat tempGpuMat(
                captureHeight,
                captureWidth,
                CV_8UC4
            );

            if (gpuMat.empty() || gpuMat.size() != tempGpuMat.size() || gpuMat.type() != tempGpuMat.type()) {
                gpuMat.create(tempGpuMat.size(), tempGpuMat.type());
            }

            size_t spitch = captureWidth * 4;
            cudaStatus = cudaMemcpy2DFromArray(gpuMat.data, gpuMat.step, cuArray, 0, 0, spitch, captureHeight, cudaMemcpyDeviceToDevice);
            if (cudaStatus != cudaSuccess) {
                cudaGraphicsUnmapResources(1, &cudaResource, 0);
                cudaGraphicsUnregisterResource(cudaResource);
                cudaResource = nullptr;
                break;
            }

            cudaStatus = cudaGraphicsUnmapResources(1, &cudaResource, 0);
            if (cudaStatus != cudaSuccess) {
            }

            cudaStatus = cudaGraphicsUnregisterResource(cudaResource);
            if (cudaStatus != cudaSuccess) {
            }
            cudaResource = nullptr;

            frameReleased = true;
        } while (false);

        if (texture) {
            texture->Release();
        }
        if (tempDesktopResource) {
            tempDesktopResource->Release();
        }
        g_desktopDuplication->ReleaseFrame();

        if (frameReleased) {
            cv::cuda::cvtColor(gpuMat, gpuMat, cv::COLOR_BGRA2RGB);
            if (640 != captureHeight || 640 != captureWidth) {
                cv::cuda::resize(gpuMat, gpuMat, cv::Size(640, 640));
            }
        }
        return frameReleased;
    }

    bool get_mat_from_shared_handle(HANDLE sharedHandle, cv::cuda::GpuMat& gpuMat) {
        ID3D11Texture2D* pTexture = nullptr;

        HRESULT hr;

        hr = g_device->OpenSharedResource(sharedHandle, __uuidof(ID3D11Texture2D), (void**)&pTexture);
        if (!SUCCEEDED(hr)) {
            std::cout << "[capture::get_mat_from_shared_handle] Failed due to OpenSharedResource -> " << hr << "\n";
        }
        return false;
    }

}
