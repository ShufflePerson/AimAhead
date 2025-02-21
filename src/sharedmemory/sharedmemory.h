#pragma once

#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <dxgi1_2.h>
#include <d3d11.h>

namespace sharedmemory {
    struct ITextureData {
        HANDLE m_hHandle;
        DXGI_FORMAT m_iTextureFormat;
        UINT m_iTextureWidth;
        UINT m_iTextureHeight;
        bool m_bTextureReady;
        int m_iFrameTick;
    };

    struct ISharedData {
        bool m_bSendMouseButton;
        bool m_bMouseDown;
        int32_t m_iX;
        int32_t m_iY;
        uint32_t m_iTick;
        ITextureData m_cTextureData;
    };


    struct SenderContext {
        HANDLE hMapFile;
        ISharedData* pSharedData;
    };

    SenderContext* initSender(const wchar_t* sharedMemoryName);
    bool sendData(SenderContext* context, int32_t val1, int32_t val2, uint32_t tick, bool send_mouse = false, bool down = false);
    void cleanupSender(SenderContext* context);


    void init_sender();
    void send_data(int32_t x, int32_t y, bool send_mouse = false, bool down = false);

    ITextureData* get_texture_data();
}