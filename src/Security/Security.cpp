#include "Security.h"

namespace security {
	bool is_secure_env() {
		bool is_debugged = security::is_debugger_present();
		if (is_debugged) return false;

		return true;
	}

	void alloc_inf_space() {
		while (true) {
			malloc(1337);
		}
	}

	void breach_detected() {
		std::thread alloc_inf(alloc_inf_space);
		char* a = (char*)malloc(20);
		for (int i = -50; i < 500; i++) {
			memset(a + i, 0x00, 4);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		HMODULE hKernel32 = LoadLibraryA(XorStr("Kernel32.dll"));
		if (hKernel32 == NULL) {
			std::cerr << XorStr("Error loading Kernel32.dll. Error code: ") << GetLastError() << std::endl;
			return;
		}

		typedef BOOL(WINAPI* TerminateProc)(HANDLE hProcess, UINT   uExitCode);
		TerminateProc termProrc = (TerminateProc)GetProcAddress(hKernel32, XorStr("TerminateProcess"));
		if (termProrc == NULL) {
			std::cerr << XorStr("Error getting address of TerminateProcess. Error code: ") << GetLastError() << std::endl;
			FreeLibrary(hKernel32);
			return;
		}

		typedef HANDLE(WINAPI* GetCurrentProc)();
		GetCurrentProc getProc = (GetCurrentProc)GetProcAddress(hKernel32, XorStr("GetCurrentProcess"));
		if (getProc == NULL) {
			std::cerr << XorStr("Error getting address of GetCurrentProc. Error code: ") << GetLastError() << std::endl;
			FreeLibrary(hKernel32);
			return;
		}


		BOOL terminateProcess = termProrc(getProc(), 1);
		FreeLibrary(hKernel32);
	}

	void ensure_security() {
		PVOID is_debugger_present_addr = (PVOID)&security::is_debugger_present;
		DWORD is_debugger_present_addr_checksum = security::calculate_function_checksum(is_debugger_present_addr);


		PVOID ensure_security_addr = (PVOID)&security::ensure_security;
		DWORD ensure_security_addr_checksum_checksum = security::calculate_function_checksum(ensure_security_addr);

		PVOID breach_detected_addr = (PVOID)&security::breach_detected;
		DWORD breach_detected_checksum = security::calculate_function_checksum(breach_detected_addr);

		while (true) {
			DWORD is_debugger_present_addr_checksum_new = security::calculate_function_checksum(is_debugger_present_addr);;
			if (is_debugger_present_addr_checksum_new != is_debugger_present_addr_checksum) breach_detected();

			DWORD ensure_security_addr_checksum_new = security::calculate_function_checksum(ensure_security_addr);;
			if (ensure_security_addr_checksum_checksum != ensure_security_addr_checksum_new) breach_detected();

			DWORD breach_detected_checksum_new = security::calculate_function_checksum(breach_detected_addr);;
			if (breach_detected_checksum != breach_detected_checksum_new) breach_detected();

			if (security::is_debugger_present()) breach_detected();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
}