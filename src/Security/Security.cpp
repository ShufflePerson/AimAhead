#include "Security.h"
#define PRINT_NEW_SUMS 0

namespace security {
	DWORD is_debugger_present_checksum;
	DWORD ensure_security_checksum;
	DWORD breach_detected_checksum;
	DWORD is_good_parent_checksum;
	DWORD get_checksum_checksum;
	DWORD detect_function_size_checksum;

	std::vector<t_checksum> checksums = {};

	static void alloc_inf_space() {
		return;
		while (true) {
			malloc(1337);
		}
	}

	void breach_detected() {
		return;
		std::thread alloc_inf(alloc_inf_space);
		char* a = (char*)malloc(20);
		if (a != nullptr) {
			for (int i = -50; i < 500; i++) {
				memset(a + i, 0x00, 4);
			}
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

	static void static_breach_detected() {
		return;
		std::thread alloc_inf(alloc_inf_space);
		char* a = (char*)malloc(20);
		if (a != nullptr) {
			for (int i = -50; i < 500; i++) {
				memset(a + i, 0x00, 4);
			}
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

	void add_checksum_watch(PVOID func_addr) {
		DWORD value;
		value = security::calculate_function_checksum(func_addr);
		checksums.push_back({ func_addr, value });
	}

	void ensure_security() {
		return;
		INIT_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
;		INIT_CHECKSUM(ensure_security, ensure_security_checksum);
		INIT_CHECKSUM(breach_detected, breach_detected_checksum);
		INIT_CHECKSUM(is_good_parent, is_good_parent_checksum);
		INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
		INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);

#if PRINT_NEW_SUMS == 1
		std::cout << "#pragma once" << std::endl;
		std::cout << "#define CALC_FUNC_SUM 0x" << std::hex << std::setw(8) << std::setfill('0') << get_checksum_checksum << std::endl;
		std::cout << "#define DETECT_FUNC_SUM 0x" << std::hex << std::setw(8) << std::setfill('0') << detect_function_size_checksum << std::endl;
#endif
			

		while (true) {
#if PRINT_NEW_SUMS == 0
			if (get_checksum_checksum != CALC_FUNC_SUM) static_breach_detected();
			if (detect_function_size_checksum != DETECT_FUNC_SUM) static_breach_detected();
#endif

			INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
			INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);
			CHECK_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
			CHECK_CHECKSUM(ensure_security, ensure_security_checksum);
			CHECK_CHECKSUM(breach_detected, breach_detected_checksum);
			CHECK_CHECKSUM(is_good_parent, is_good_parent_checksum);

			for (auto& checksum : checksums) {
				DWORD value;
				value = security::calculate_function_checksum(checksum.func_addr);
				if (value != checksum.value) {
					//connection::send_crash_packet(ECRASHREASON_CODE_PATCH_DETECTED);
					static_breach_detected();
				}
			}

			if (!is_good_parent()) {
				//connection::send_crash_packet(ECRASHREASON_BAD_PARENT);
				breach_detected();
			}
			if (security::is_debugger_present()) {
				//connection::send_crash_packet(ECRASHREASON_DEBUGGER_DETECTED);
				breach_detected();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
		}
	}

	void check_sums() {
		return;
		INIT_CHECKSUM(calculate_function_checksum, get_checksum_checksum);
		INIT_CHECKSUM(detect_function_size, detect_function_size_checksum);
		CHECK_CHECKSUM(is_debugger_present, is_debugger_present_checksum);
		CHECK_CHECKSUM(ensure_security, ensure_security_checksum);
		CHECK_CHECKSUM(breach_detected, breach_detected_checksum);
		CHECK_CHECKSUM(is_good_parent, is_good_parent_checksum);

#if PRINT_NEW_SUMS == 0
		if (get_checksum_checksum != CALC_FUNC_SUM) static_breach_detected();
		if (detect_function_size_checksum != DETECT_FUNC_SUM) static_breach_detected();
#endif
		if (!is_good_parent()) {
			//connection::send_crash_packet(ECRASHREASON_BAD_PARENT);
			breach_detected();
		}
		if (security::is_debugger_present()) {
			//connection::send_crash_packet(ECRASHREASON_DEBUGGER_DETECTED);
			breach_detected();
		}

		for (auto& checksum : checksums) {
			DWORD value;
			value = security::calculate_function_checksum(checksum.func_addr);
			if (value != checksum.value) {
				//connection::send_crash_packet(ECRASHREASON_CODE_PATCH_DETECTED);
				static_breach_detected();
			}
		}
	}
}