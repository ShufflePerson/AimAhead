#pragma once
#ifdef COMPILE_GFN_MODE
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include "../Security/xor.h"

namespace injector {
	int get_gfn_pid();
	void inject();

	bool is_injected();
}
#endif