#include "../Security.h"

namespace security {
	bool is_good_parent() {
        DWORD processId = GetCurrentProcessId();
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return false; 
        }

        PROCESSENTRY32W pe32; 
        pe32.dwSize = sizeof(PROCESSENTRY32W);

        if (!Process32FirstW(snapshot, &pe32)) {
            CloseHandle(snapshot);
            return false; 
        }

        DWORD parentProcessId = 0;
        do {
            if (pe32.th32ProcessID == processId) {
                parentProcessId = pe32.th32ParentProcessID;
                break; 
            }
        } while (Process32NextW(snapshot, &pe32));

        CloseHandle(snapshot);

        if (parentProcessId == 0) {
            return false;
        }

        HANDLE parentProcessHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, parentProcessId);
        if (parentProcessHandle == NULL) {
            return false;
        }

        WCHAR parentProcessNameBuffer[MAX_PATH]; 
        DWORD bufferSize = MAX_PATH;

        if (!QueryFullProcessImageNameW(parentProcessHandle, 0, parentProcessNameBuffer, &bufferSize)) { 
            CloseHandle(parentProcessHandle);
            return false;
        }

        CloseHandle(parentProcessHandle);

        std::wstring parentProcessNameW = parentProcessNameBuffer; 
        size_t lastSlash = parentProcessNameW.find_last_of(L"\\/");
        if (lastSlash != std::wstring::npos) {
            parentProcessNameW = parentProcessNameW.substr(lastSlash + 1);
        }

        std::string parentProcessName;
        for (wchar_t wc : parentProcessNameW) {
            parentProcessName += static_cast<char>(wc);
        }

        std::transform(parentProcessName.begin(), parentProcessName.end(), parentProcessName.begin(), ::tolower);

        return parentProcessName == XorStr("explorer.exe");
	}
}