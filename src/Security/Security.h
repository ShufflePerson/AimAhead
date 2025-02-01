#pragma once
#include <windows.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "xor.h"

namespace security {
	bool is_secure_env();
	bool is_debugger_present();

	void ensure_security();
}