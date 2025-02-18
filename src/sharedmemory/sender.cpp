#include "sharedmemory.h"
#include <chrono>
#include <thread>


namespace sharedmemory {
    SenderContext* senderContext;
    uint32_t last_tick = 0;
    void init_sender() {
        const wchar_t* sharedMemoryName = L"MySharedMemory";
        senderContext = initSender(sharedMemoryName);
        if (senderContext == nullptr) {
            return;
        }
    }

    void send_data(int32_t x, int32_t y, uint16_t action) {
        sendData(senderContext, x, y, last_tick, action);
        last_tick++;
    }
}