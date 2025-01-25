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


    bool InitializeCapture() {
        HRESULT hr;

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
        IDXGIResource* desktopResource = nullptr;
        IDXGIResource* tempDesktopResource = nullptr;
        DXGI_OUTDUPL_FRAME_INFO frameInfo;
        ID3D11Texture2D* texture = nullptr;
        bool frameAcquired = false;

        hr = g_desktopDuplication->AcquireNextFrame(1000, &frameInfo, &tempDesktopResource);
        if (FAILED(hr)) {
            if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
                return false; // It's okay if we time out
            }
            else {
                std::cerr << "Error acquiring next frame: " << std::hex << hr << std::endl;
                return false;
            }
        }
        frameAcquired = true;
        bool frameReleased = false;

        do {
            hr = tempDesktopResource->QueryInterface(__uuidof(IDXGIResource), (void**)&desktopResource);
            if (FAILED(hr)) {
                std::cerr << "Error querying IDXGIResource interface: " << std::hex << hr << std::endl;
                break;
            }

            hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&texture);
            if (FAILED(hr)) {
                std::cerr << "Error querying texture interface: " << std::hex << hr << std::endl;
                break;
            }

            g_context->CopyResource(g_stagingTexture, texture);

            D3D11_MAPPED_SUBRESOURCE mappedResource;
            hr = g_context->Map(g_stagingTexture, 0, D3D11_MAP_READ, 0, &mappedResource);
            if (FAILED(hr)) {
                std::cerr << "Error mapping staging texture: " << std::hex << hr << std::endl;
                break;
            }

            int captureStartX = (g_screenWidth - captureWidth) / 2;
            int captureStartY = (g_screenHeight - captureHeight) / 2;

            if (captureStartX < 0) captureStartX = 0;
            if (captureStartY < 0) captureStartY = 0;
            if (captureStartX + captureWidth > g_screenWidth) captureStartX = g_screenWidth - captureWidth;
            if (captureStartY + captureHeight > g_screenHeight) captureStartY = g_screenHeight - captureHeight;

            if (capturedData.size() != captureWidth * captureHeight * 4) {
                capturedData.resize(captureWidth * captureHeight * 4);
            }
            BYTE* pCapturedData = capturedData.data();

            BYTE* pSourceData = static_cast<BYTE*>(mappedResource.pData);
            int sourceRowPitch = mappedResource.RowPitch;
            BYTE* pStartRow = pSourceData + captureStartY * sourceRowPitch + captureStartX * 4; 

            for (int y = 0; y < captureHeight; ++y) {
                memcpy(pCapturedData + y * captureWidth * 4, pStartRow + y * sourceRowPitch, captureWidth * 4);
            }

            g_context->Unmap(g_stagingTexture, 0);

            frameReleased = true;

        } while (false);

        if (texture) texture->Release();
        if (desktopResource) desktopResource->Release();
        if (tempDesktopResource) tempDesktopResource->Release(); 

        if (g_desktopDuplication && frameAcquired) {
            HRESULT releaseHr = g_desktopDuplication->ReleaseFrame();
            if (FAILED(releaseHr)) {
                std::cerr << "Error releasing frame: " << std::hex << releaseHr << std::endl;
            }
        }

        return frameReleased;
    }


}
