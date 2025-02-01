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

    cv::cuda::GpuMat convertTo3Channels(const cv::cuda::GpuMat& input) {
        if (input.type() == CV_8UC4) {
            cv::cuda::GpuMat channels[4];
            cv::cuda::split(input, channels);  

            cv::cuda::GpuMat rgb; 
            cv::cuda::merge(channels, 3, rgb); 

            return rgb;  
        }
        else {
            throw std::runtime_error("Input is not CV_8UC4");
        }
    }


    bool initializeCUDAResources() {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = captureWidth;
        desc.Height = captureHeight;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; 
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = 0;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

        HRESULT hr = g_device->CreateTexture2D(&desc, nullptr, &g_cudaTexture);
        if (FAILED(hr)) {
            std::cerr << "Failed to create CUDA texture: " << std::hex << hr << std::endl;
            return false;
        }

        cudaError_t cudaErr = cudaGraphicsD3D11RegisterResource(&g_cudaResource, g_cudaTexture, cudaGraphicsRegisterFlagsNone);
        if (cudaErr != cudaSuccess) {
            std::cerr << "CUDA resource registration failed: " << cudaGetErrorString(cudaErr) << std::endl;
            return false;
        }

        return true;
    }
    bool InitializeCapture() {
        HRESULT hr;
        cudaSetDevice(0);

        hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&g_factory);
        if (FAILED(hr)) {
            std::cerr << "Error creating DXGIFactory: " << std::hex << hr << std::endl;
            return false;
        }

        hr = g_factory->EnumAdapters(0, &g_adapter);
        if (FAILED(hr)) {
            std::cerr << "Error enumerating adapter: " << std::hex << hr << std::endl;
            return false;
        }

        hr = g_adapter->EnumOutputs(0, &g_output);
        if (FAILED(hr)) {
            std::cerr << "Error enumerating output: " << std::hex << hr << std::endl;
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
            std::cerr << "Error creating D3D11 device: " << std::hex << hr << std::endl;
            return false;
        }

        hr = g_output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&g_output1);
        if (FAILED(hr)) {
            std::cerr << "Error getting IDXGIOutput1 interface: " << std::hex << hr << std::endl;
            return false;
        }

        hr = g_output1->DuplicateOutput(g_device, &g_desktopDuplication);
        if (FAILED(hr)) {
            std::cerr << "Error creating desktop duplication: " << std::hex << hr << std::endl;
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
            std::cerr << "Error creating staging texture: " << std::hex << hr << std::endl;
            return false;
        }
        initializeCUDAResources();
        spdlog::info("D3D11 Capture initialized successfully!");
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
    bool captureScreenRegion(std::vector<BYTE>& capturedData) {
        HRESULT hr;
        IDXGIResource* tempDesktopResource = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ID3D11Texture2D* texture = nullptr;
        bool frameReleased = false;

        // Use non-blocking frame acquisition
        hr = g_desktopDuplication->AcquireNextFrame(0, &frameInfo, &tempDesktopResource);
        if (FAILED(hr)) {
            if (hr != DXGI_ERROR_WAIT_TIMEOUT) {
                // Consider removing error logging in release builds
                // std::cerr << "Acquire error: 0x" << std::hex << hr << std::endl;
            }
            return false;
        }

        do {
            // Directly query for texture interface from acquired resource
            hr = tempDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture));
            if (FAILED(hr)) break;

            // Copy to staging texture (assumes g_stagingTexture is pre-created)
            g_context->CopyResource(g_stagingTexture, texture);

            // Map staging texture for CPU access
            D3D11_MAPPED_SUBRESOURCE mappedResource;
            hr = g_context->Map(g_stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
            if (FAILED(hr)) break;

            // Calculate capture parameters
            const int uStride = captureWidth * 4;
            const int requiredSize = captureWidth * captureHeight * 4;

            // Optimized resize check
            if (capturedData.size() != requiredSize) {
                capturedData.resize(requiredSize);
            }

            // Calculate safe capture area (consider precomputing if screen size is static)
            int captureStartX = std::clamp((g_screenWidth - captureWidth) / 2, 0, g_screenWidth - captureWidth);
            int captureStartY = std::clamp((g_screenHeight - captureHeight) / 2, 0, g_screenHeight - captureHeight);

            // Set up pointers
            BYTE* pSource = static_cast<BYTE*>(mappedResource.pData)
                + captureStartY * mappedResource.RowPitch
                + captureStartX * 4;
            BYTE* pDest = capturedData.data();

            // Optimized copy based on row pitch
            const size_t destStride = captureWidth * 4;
            const size_t sourceStride = mappedResource.RowPitch;

            if (sourceStride == destStride) {
                // Single copy if pitches match
                memcpy(pDest, pSource, destStride * captureHeight);
            }
            else {
                // Copy row-by-row if pitches differ
                for (int y = 0; y < captureHeight; ++y) {
                    memcpy(pDest + y * destStride,
                        pSource + y * sourceStride,
                        destStride);
                }
            }

            g_context->Unmap(g_stagingTexture, 0);
            frameReleased = true;
        } while (false);

        // Cleanup resources
        if (texture) texture->Release();
        if (tempDesktopResource) tempDesktopResource->Release();

        // Release frame reference
        g_desktopDuplication->ReleaseFrame();

        return frameReleased;
    }
    inline void cudaCheckError(cudaError_t error) {
        if (error != cudaSuccess) {
            std::cerr << "CUDA Error: " << cudaGetErrorString(error) << std::endl;
            exit(-1);
        }
    }

    cv::cuda::GpuMat crop_center_square(const cv::cuda::GpuMat& inputGpuMat) {
        int originalWidth = inputGpuMat.cols;
        int originalHeight = inputGpuMat.rows;

        int cropSize = 640;

        int startX = (originalWidth - cropSize) / 2;
        int startY = (originalHeight - cropSize) / 2;

        cv::Rect cropRect(startX, startY, cropSize, cropSize);

        return inputGpuMat(cropRect);
    }

    cv::cuda::GpuMat captureScreenRegion() {
        HRESULT hr;
        IDXGIResource* tempDesktopResource = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ID3D11Texture2D* desktopTexture = nullptr;
        cv::cuda::GpuMat gpuMat;
        bool frameAcquired = false;

        hr = g_desktopDuplication->AcquireNextFrame(1000, &frameInfo, &tempDesktopResource);
        if (FAILED(hr)) {
            if (hr != DXGI_ERROR_WAIT_TIMEOUT)
                std::cerr << "AcquireNextFrame failed: 0x" << std::hex << hr << std::endl;
            return gpuMat; // Return empty GpuMat on failure
        }
        frameAcquired = true;

        do {
            // Query for ID3D11Texture2D
            hr = tempDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&desktopTexture));
            if (FAILED(hr)) {
                std::cerr << "Failed to get desktop texture: 0x" << std::hex << hr << std::endl;
                break;
            }

            // Calculate capture region (adjust as in original code)
            int captureStartX = std::max(0, (g_screenWidth - captureWidth) / 2);
            int captureStartY = std::max(0, (g_screenHeight - captureHeight) / 2);
            captureStartX = std::min(captureStartX, g_screenWidth - captureWidth);
            captureStartY = std::min(captureStartY, g_screenHeight - captureHeight);

            // Copy the region of interest to CUDA texture
            D3D11_BOX region = {
                static_cast<UINT>(captureStartX), static_cast<UINT>(captureStartY), 0,
                static_cast<UINT>(captureStartX + captureWidth), static_cast<UINT>(captureStartY + captureHeight), 1
            };
            g_context->CopySubresourceRegion(g_cudaTexture, 0, 0, 0, 0, desktopTexture, 0, &region);

            // Map CUDA resource
            cudaError_t cudaErr = cudaGraphicsMapResources(1, &g_cudaResource, 0);
            if (cudaErr != cudaSuccess) {
                std::cerr << "cudaGraphicsMapResources failed: " << cudaGetErrorString(cudaErr) << std::endl;
                break;
            }

            cudaArray* cuArray;
            cudaErr = cudaGraphicsSubResourceGetMappedArray(&cuArray, g_cudaResource, 0, 0);
            if (cudaErr != cudaSuccess) {
                std::cerr << "cudaGraphicsSubResourceGetMappedArray failed: " << cudaGetErrorString(cudaErr) << std::endl;
                cudaGraphicsUnmapResources(1, &g_cudaResource, 0);
                break;
            }

            // Create GpuMat and copy data from CUDA array
            gpuMat.create(captureHeight, captureWidth, CV_8UC4);
            size_t spitch = captureWidth * 4; // BGRA (4 bytes per pixel)
            cudaErr = cudaMemcpy2DFromArray(gpuMat.data, gpuMat.step, cuArray, 0, 0, spitch, captureHeight, cudaMemcpyDeviceToDevice);
            if (cudaErr != cudaSuccess) {
                std::cerr << "cudaMemcpy2DFromArray failed: " << cudaGetErrorString(cudaErr) << std::endl;
                gpuMat.release();
            }

            // Unmap CUDA resource
            cudaGraphicsUnmapResources(1, &g_cudaResource, 0);

        } while (false);

        // Cleanup
        if (desktopTexture) desktopTexture->Release();
        if (tempDesktopResource) tempDesktopResource->Release();
        if (frameAcquired) g_desktopDuplication->ReleaseFrame();

        return gpuMat;
    }

}
