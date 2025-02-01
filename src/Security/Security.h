#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "xor.h"

namespace security {
	bool is_secure_env();


	bool is_debugger_present();
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
}