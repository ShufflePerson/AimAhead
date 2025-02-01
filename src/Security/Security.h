#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <TlHelp32.h>
#include <string>
#include <algorithm>
#include "./SecurityUtils/crc/crc.h"

#include "xor.h"

namespace security {
	bool is_secure_env();


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

	size_t detect_function_size(PVOID pFunc);
	DWORD calculate_function_checksum(PVOID func_address);
}