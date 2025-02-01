#include "../Security.h"
#include <winternl.h>

typedef NTSTATUS(NTAPI* TNtQueryInformationProcess)(
    IN HANDLE           ProcessHandle,
    IN PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID           ProcessInformation,
    IN ULONG            ProcessInformationLength,
    OUT PULONG          ReturnLength
);

typedef NTSTATUS(NTAPI* NtQuerySystemInformationFunc)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID                    SystemInformation,
    ULONG                    SystemInformationLength,
    PULONG                   ReturnLength
);


enum { SystemKernelDebuggerInformation = 0x23 };

typedef struct _SYSTEM_KERNEL_DEBUGGER_INFORMATION {
    BOOLEAN DebuggerEnabled;
    BOOLEAN DebuggerNotPresent;
} SYSTEM_KERNEL_DEBUGGER_INFORMATION, * PSYSTEM_KERNEL_DEBUGGER_INFORMATION;

#define FLG_HEAP_ENABLE_TAIL_CHECK   0x10
#define FLG_HEAP_ENABLE_FREE_CHECK   0x20
#define FLG_HEAP_VALIDATE_PARAMETERS 0x40
#define NT_GLOBAL_FLAG_DEBUGGED (FLG_HEAP_ENABLE_TAIL_CHECK | FLG_HEAP_ENABLE_FREE_CHECK | FLG_HEAP_VALIDATE_PARAMETERS)

namespace security {

    bool check_is_debugger_present(HMODULE hKernel32) {
        typedef BOOL(WINAPI* IsDebuggerPresentFunc)();
        IsDebuggerPresentFunc pIsDebuggerPresent = (IsDebuggerPresentFunc)GetProcAddress(hKernel32, XorStr("IsDebuggerPresent"));
        if (pIsDebuggerPresent == NULL) {
            std::cerr << XorStr("Error getting address of IsDebuggerPresent. Error code: ") << GetLastError() << std::endl;
            return true; 
        }
        return pIsDebuggerPresent() != FALSE;
    }

    bool check_remote_debugger_present(HMODULE hKernel32) {
        typedef HANDLE(WINAPI* GetCurrentProc)();
        GetCurrentProc getProc = (GetCurrentProc)GetProcAddress(hKernel32, XorStr("GetCurrentProcess"));
        if (getProc == NULL) {
            std::cerr << XorStr("Error getting address of GetCurrentProc. Error code: ") << GetLastError() << std::endl;
            return true;
        }

        typedef HANDLE(WINAPI* CheckRemDebugger)(HANDLE hProcess, PBOOL  pbDebuggerPresent);
        CheckRemDebugger checkRemoteDebug = (CheckRemDebugger)GetProcAddress(hKernel32, XorStr("CheckRemoteDebuggerPresent"));
        if (checkRemoteDebug == NULL) {
            std::cerr << XorStr("Error getting address of checkRemoteDebug. Error code: ") << GetLastError() << std::endl;
            return true;
        }

        BOOL bDebuggerPresent;
        checkRemoteDebug(getProc(), &bDebuggerPresent);
        return bDebuggerPresent != FALSE;
    }

    bool check_ntquery_information_process_debug_port(HMODULE hNtdll) {
        auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
            hNtdll, XorStr("NtQueryInformationProcess"));

        if (pfnNtQueryInformationProcess == nullptr) {
            return false; 
        }

        DWORD dwProcessDebugPort, dwReturned;
        NTSTATUS status = pfnNtQueryInformationProcess(
            GetCurrentProcess(),
            ProcessDebugPort,
            &dwProcessDebugPort,
            sizeof(DWORD),
            &dwReturned);

        return NT_SUCCESS(status) && (dwProcessDebugPort == -1);
    }

    bool check_ntquery_information_process_debug_flags(HMODULE hNtdll) {
        auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
            hNtdll, XorStr("NtQueryInformationProcess"));

        if (pfnNtQueryInformationProcess == nullptr) {
            return false; 
        }

        DWORD dwProcessDebugFlags, dwReturned;
        const DWORD ProcessDebugFlagsConst = 0x1f;
        NTSTATUS status = pfnNtQueryInformationProcess(
            GetCurrentProcess(),
            (PROCESSINFOCLASS)ProcessDebugFlagsConst,
            &dwProcessDebugFlags,
            sizeof(DWORD),
            &dwReturned);

        return NT_SUCCESS(status) && (dwProcessDebugFlags == 0);
    }

    bool check_ntquery_information_process_debug_object_handle(HMODULE hNtdll) {
        auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
            hNtdll, XorStr("NtQueryInformationProcess"));

        if (pfnNtQueryInformationProcess == nullptr) {
            return false; 
        }

        DWORD dwReturned;
        HANDLE hProcessDebugObject = 0;
        const DWORD ProcessDebugObjectHandleConst = 0x1e;
        NTSTATUS status = pfnNtQueryInformationProcess(
            GetCurrentProcess(),
            (PROCESSINFOCLASS)ProcessDebugObjectHandleConst,
            &hProcessDebugObject,
            sizeof(HANDLE),
            &dwReturned);

        return NT_SUCCESS(status) && (hProcessDebugObject != 0);
    }

    bool check_ntquery_system_information_kernel_debugger(HMODULE hNtdll) {
        NTSTATUS status;
        SYSTEM_KERNEL_DEBUGGER_INFORMATION SystemInfo;

        NtQuerySystemInformationFunc pNtQuerySystemInformation =
            (NtQuerySystemInformationFunc)GetProcAddress(hNtdll, XorStr("NtQuerySystemInformation"));

        if (pNtQuerySystemInformation == nullptr) {
            return false;
        }

        status = pNtQuerySystemInformation(
            (SYSTEM_INFORMATION_CLASS)SystemKernelDebuggerInformation,
            &SystemInfo,
            sizeof(SystemInfo),
            NULL);

        return SystemInfo.DebuggerEnabled && !SystemInfo.DebuggerNotPresent;
    }

    bool check_peb_being_debugged() {
        PPEB pPeb = (PPEB)__readgsqword(0x60);
        return pPeb->BeingDebugged != FALSE;
    }

    bool check_peb_ntglobal_flag() {
        PPEB pPeb = (PPEB)__readgsqword(0x60);
        DWORD dwNtGlobalFlag = *(PDWORD)((PBYTE)pPeb + 0xBC);
        return (dwNtGlobalFlag & NT_GLOBAL_FLAG_DEBUGGED) != 0;
    }

    bool check_heap_flags() {
        PPEB pPeb = (PPEB)__readgsqword(0x60);
        PVOID pHeapBase = (PVOID)(*(PDWORD_PTR)((PBYTE)pPeb + 0x30));
        DWORD dwHeapFlagsOffset = true
            ? 0x70
            : 0x14;
        DWORD dwHeapForceFlagsOffset = true
            ? 0x74
            : 0x18;

        PDWORD pdwHeapFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapFlagsOffset);
        PDWORD pdwHeapForceFlags = (PDWORD)((PBYTE)pHeapBase + dwHeapForceFlagsOffset);

        return (*pdwHeapFlags & ~HEAP_GROWABLE) || (*pdwHeapForceFlags != 0);
    }


    bool is_debugger_present() {
        HMODULE hKernel32 = LoadLibraryA(XorStr("Kernel32.dll"));
        if (hKernel32 == NULL) {
            std::cerr << XorStr("Error loading Kernel32.dll. Error code: ") << GetLastError() << std::endl;
            return true; 
        }

        HMODULE hNtdll = LoadLibraryA(XorStr("ntdll.dll"));
        if (hNtdll == NULL) {
            std::cerr << XorStr("Warning: Error loading ntdll.dll. Some debugger checks might be skipped or fail. Error code: ") << GetLastError() << std::endl;
        }


        if (check_is_debugger_present(hKernel32)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (check_remote_debugger_present(hKernel32)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (hNtdll && check_ntquery_information_process_debug_port(hNtdll)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (hNtdll && check_ntquery_information_process_debug_flags(hNtdll)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (hNtdll && check_ntquery_information_process_debug_object_handle(hNtdll)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (hNtdll && check_ntquery_system_information_kernel_debugger(hNtdll)) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (check_peb_being_debugged()) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (check_peb_ntglobal_flag()) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }

        if (check_heap_flags()) {
            FreeLibrary(hKernel32);
            if (hNtdll) FreeLibrary(hNtdll);
            return true;
        }


        FreeLibrary(hKernel32);
        if (hNtdll) FreeLibrary(hNtdll);
        return false;
    }
}