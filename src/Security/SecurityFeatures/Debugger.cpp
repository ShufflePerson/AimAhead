#include "../Security.h"

bool security::is_debugger_present() {
	HMODULE hKernel32 = LoadLibraryA(XorStr("Kernel32.dll"));
    if (hKernel32 == NULL) {
        std::cerr << XorStr("Error loading Kernel32.dll. Error code: ") << GetLastError() << std::endl;
        return true;
    }

    typedef BOOL(WINAPI* IsDebuggerPresentFunc)();
    IsDebuggerPresentFunc pIsDebuggerPresent = (IsDebuggerPresentFunc)GetProcAddress(hKernel32, XorStr("IsDebuggerPresent"));
    if (pIsDebuggerPresent == NULL) {
        std::cerr << XorStr("Error getting address of IsDebuggerPresent. Error code: ") << GetLastError() << std::endl;
        FreeLibrary(hKernel32); 
        return true;
    }


    BOOL isDebuggerPresent = pIsDebuggerPresent();
    FreeLibrary(hKernel32);

    if (isDebuggerPresent) return true;

    return false;
}