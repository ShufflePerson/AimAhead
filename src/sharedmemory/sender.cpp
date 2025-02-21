#include "sharedmemory.h"
#include <chrono>
#include <thread>


namespace sharedmemory {
    SenderContext* senderContext;
    uint32_t last_tick = 0;
    void init_sender() {
        const wchar_t* sharedMemoryName = L"XX9922SS";
        senderContext = initSender(sharedMemoryName);
        if (senderContext == nullptr) {
            return;
        }
    }

    void send_data(int32_t x, int32_t y, bool send_mouse, bool down) {
        sendData(senderContext, x, y, last_tick, send_mouse, down);
        last_tick = last_tick + 2;
    }


    ITextureData* get_texture_data() {
        return &senderContext->pSharedData->m_cTextureData;
    }
}