#include "Security.h"

namespace security {
	DWORD is_debugger_present_checksum;
	DWORD ensure_security_checksum;
	DWORD breach_detected_checksum;
	DWORD is_good_parent_checksum;
	DWORD get_checksum_checksum;
	DWORD detect_function_size_checksum;

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
#define INIT_CHECKSUM(func_name, checksum_var) \
            PVOID func_addr_##func_name = (PVOID)&security::func_name; \
            checksum_var = security::calculate_function_checksum(func_addr_##func_name);

#define CHECK_CHECKSUM(func_name, checksum_var) \
            do { \
                DWORD checksum_new = security::calculate_function_checksum((PVOID)&security::func_name); \
                if (checksum_new != checksum_var) breach_detected(); \
            } while(0)



		INIT_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
;		INIT_CHECKSUM(ensure_security, ensure_security_checksum);
		INIT_CHECKSUM(breach_detected, breach_detected_checksum);
		INIT_CHECKSUM(is_good_parent, is_good_parent_checksum);
		INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
		INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);
			
		while (true) {
			INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
			INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);
			CHECK_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
			CHECK_CHECKSUM(ensure_security, ensure_security_checksum);
			CHECK_CHECKSUM(breach_detected, breach_detected_checksum);
			CHECK_CHECKSUM(is_good_parent, is_good_parent_checksum);

			if (get_checksum_checksum != 0xc8f2803a) breach_detected();
			if (detect_function_size_checksum != 0x481862d0) breach_detected();
			if (!is_good_parent()) breach_detected();
			if (security::is_debugger_present()) breach_detected();

			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}

	void check_sums() {
		INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
		INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);
		CHECK_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
		CHECK_CHECKSUM(ensure_security, ensure_security_checksum);
		CHECK_CHECKSUM(breach_detected, breach_detected_checksum);
		CHECK_CHECKSUM(is_good_parent, is_good_parent_checksum);

		if (get_checksum_checksum != 0xc8f2803a) breach_detected();
		if (detect_function_size_checksum != 0x481862d0) breach_detected();

		if (!is_good_parent()) breach_detected();
		if (security::is_debugger_present()) breach_detected();
	}
}