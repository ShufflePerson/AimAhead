#include "sharedmemory.h"


namespace sharedmemory {

    SenderContext* initSender(const wchar_t* sharedMemoryName) {
        SenderContext* context = new SenderContext();
        context->hMapFile = CreateFileMappingW(
            INVALID_HANDLE_VALUE,    // Use paging file
            NULL,                    // Default security
            PAGE_READWRITE,          // Read/write access
            0,                       // Maximum object size (high-order DWORD)
            sizeof(ISharedData),      // Maximum object size (low-order DWORD)
            sharedMemoryName);       // Name of mapping object

        if (context->hMapFile == NULL) {
            std::cerr << "Sender: Could not create file mapping object (" << GetLastError() << ").\n";
            delete context;
            return nullptr;
        }

        context->pSharedData = static_cast<ISharedData*>(MapViewOfFile(
            context->hMapFile,         // Handle to map object
            FILE_MAP_ALL_ACCESS,      // Read/write permission
            0,
            0,
            sizeof(ISharedData)));

        if (context->pSharedData == NULL) {
            std::cerr << "Sender: Could not map view of file (" << GetLastError() << ").\n";
            CloseHandle(context->hMapFile);
            delete context;
            return nullptr;
        }
        context->pSharedData->m_cTextureData = ITextureData();
        context->pSharedData->m_cTextureData.m_bTextureReady = false;
        return context;
    }

    bool sendData(SenderContext* context, int32_t val1, int32_t val2, uint32_t tick, bool send_mouse, bool down) {
        if (context == nullptr || context->pSharedData == nullptr) {
            std::cerr << "Sender: Invalid context in sendData.\n";
            return false;
        }
        context->pSharedData->m_bSendMouseButton = false;

        if (val1 != -999)
            context->pSharedData->m_iX = val1;
        if (val2 != -999)
            context->pSharedData->m_iY = val2;
        else {
            context->pSharedData->m_bSendMouseButton = send_mouse;
            context->pSharedData->m_bMouseDown = down;
        }
        


        context->pSharedData->m_iTick = tick;
        return true;
    }

    void cleanupSender(SenderContext* context) {
        if (context != nullptr) {
            if (context->pSharedData != nullptr) {
                UnmapViewOfFile(context->pSharedData);
            }
            if (context->hMapFile != NULL) {
                CloseHandle(context->hMapFile);
            }
            delete context;
        }
    }
}

