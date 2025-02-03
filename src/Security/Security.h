#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <algorithm>
#include "checksums.h"
#include "./SecurityUtils/crc/crc.h"

#include "xor.h"

#define INIT_CHECKSUM(func_name, checksum_var) \
            PVOID func_addr_##func_name = (PVOID)&security::func_name; \
            checksum_var = security::calculate_function_checksum(func_addr_##func_name);

#define CHECK_CHECKSUM(func_name, checksum_var) \
            do { \
                DWORD checksum_new = security::calculate_function_checksum((PVOID)&security::func_name); \
                if (checksum_new != checksum_var) { /*connection::send_crash_packet(ECRASHREASON_CODE_PATCH_DETECTED);*/ breach_detected(); } \
            } while(0)

namespace security {
	bool is_debugger_present();
	bool do_debug_registers_exist();
	bool check_heap_flags();
	bool check_peb_ntglobal_flag();
	bool check_peb_being_debugged();
	bool check_ntquery_system_information_kernel_debugger(HMODULE hNtdll);
	bool check_ntquery_information_process_debug_object_handle(HMODULE hNtdll);
	bool check_ntquery_information_process_debug_flags(HMODULE hNtdll);
	bool check_ntquery_information_process_debug_port(HMODULE hNtdll);
	bool check_remote_debugger_present(HMODULE hKernel32);
	bool check_is_debugger_present(HMODULE hKernel32);

	void ensure_security();
	void breach_detected();

	bool is_good_parent();

	void check_sums();

	size_t detect_function_size(PVOID pFunc);
	DWORD calculate_function_checksum(PVOID func_address);

	void add_checksum_watch(PVOID func_addr);

	struct t_checksum {
		PVOID func_addr;
		DWORD value;
	};
}