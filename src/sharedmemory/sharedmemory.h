#pragma once

#include <windows.h>
#include <iostream>
#include <thread>
#include <atomic>

namespace sharedmemory {
    // Structure to hold the shared data (two integers)
    struct SharedData {
        uint16_t action; //0 = move, 1 = click
        int32_t value1;
        int32_t value2;
        uint32_t tick;
    };

    // --- Sender Functions ---
    struct SenderContext {
        HANDLE hMapFile;
        SharedData* pSharedData;
    };

    SenderContext* initSender(const wchar_t* sharedMemoryName);
    bool sendData(SenderContext* context, int32_t val1, int32_t val2, uint32_t tick, uint16_t action = 0);
    void cleanupSender(SenderContext* context);

    // --- Receiver Functions ---
    struct ReceiverContext {
        HANDLE hMapFile;
        SharedData* pSharedData;
        std::atomic<bool> running; // Flag to control the receiver thread
    };

    ReceiverContext* initReceiver(const wchar_t* sharedMemoryName);
    void receiveLoop(ReceiverContext* context);
    void cleanupReceiver(ReceiverContext* context);


    void init_sender();
    void send_data(int32_t x, int32_t y, uint16_t action = 0);
}