#include "sharedmemory.h"


namespace sharedmemory {

    SenderContext* initSender(const wchar_t* sharedMemoryName) {
        SenderContext* context = new SenderContext();
        context->hMapFile = CreateFileMappingW(
            INVALID_HANDLE_VALUE,    // Use paging file
            NULL,                    // Default security
            PAGE_READWRITE,          // Read/write access
            0,                       // Maximum object size (high-order DWORD)
            sizeof(SharedData),      // Maximum object size (low-order DWORD)
            sharedMemoryName);       // Name of mapping object

        if (context->hMapFile == NULL) {
            std::cerr << "Sender: Could not create file mapping object (" << GetLastError() << ").\n";
            delete context;
            return nullptr;
        }

        context->pSharedData = static_cast<SharedData*>(MapViewOfFile(
            context->hMapFile,         // Handle to map object
            FILE_MAP_ALL_ACCESS,      // Read/write permission
            0,
            0,
            sizeof(SharedData)));

        if (context->pSharedData == NULL) {
            std::cerr << "Sender: Could not map view of file (" << GetLastError() << ").\n";
            CloseHandle(context->hMapFile);
            delete context;
            return nullptr;
        }
        return context;
    }

    bool sendData(SenderContext* context, int32_t val1, int32_t val2, uint32_t tick, uint16_t action) {
        if (context == nullptr || context->pSharedData == nullptr) {
            std::cerr << "Sender: Invalid context in sendData.\n";
            return false;
        }
        context->pSharedData->value1 = val1;
        context->pSharedData->value2 = val2;
        context->pSharedData->tick = tick;
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

    // --- Receiver Implementation ---

    ReceiverContext* initReceiver(const wchar_t* sharedMemoryName) {
        ReceiverContext* context = new ReceiverContext();
        context->running = true; // Initialize running flag

        context->hMapFile = OpenFileMappingW(
            FILE_MAP_ALL_ACCESS,   // Read/write access
            FALSE,                 // Do not inherit handle
            sharedMemoryName);     // Name of mapping object

        if (context->hMapFile == NULL) {
            std::cerr << "Receiver: Could not open file mapping object (" << GetLastError() << ").\n";
            delete context;
            return nullptr;
        }

        context->pSharedData = static_cast<SharedData*>(MapViewOfFile(
            context->hMapFile,         // Handle to map object
            FILE_MAP_ALL_ACCESS,      // Read/write permission
            0,
            0,
            sizeof(SharedData)));

        if (context->pSharedData == NULL) {
            std::cerr << "Receiver: Could not map view of file (" << GetLastError() << ").\n";
            CloseHandle(context->hMapFile);
            delete context;
            return nullptr;
        }
        return context;
    }

    void receiveLoop(ReceiverContext* context) {
        if (context == nullptr || context->pSharedData == nullptr) {
            std::cerr << "Receiver Thread: Invalid context.\n";
            return;
        }

        std::cout << "Receiver Thread: Starting to listen for messages..." << std::endl;
        while (context->running) {
            int receivedValue1 = context->pSharedData->value1;
            int receivedValue2 = context->pSharedData->value2;
            std::cout << "Receiver Thread: Received values: " << receivedValue1 << ", " << receivedValue2 << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(2)); // Short sleep to reduce CPU usage, adjust as needed
        }
        std::cout << "Receiver Thread: Exiting." << std::endl;
    }

    void cleanupReceiver(ReceiverContext* context) {
        if (context != nullptr) {
            context->running = false; // Signal thread to stop (if you had a mechanism to wait for it to stop)
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

