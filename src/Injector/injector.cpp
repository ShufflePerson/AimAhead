#ifdef COMPILE_GFN_MODE
#include "injector.h"

static bool g_bGfnInjected = false;

bool injector::is_injected() {
	return g_bGfnInjected;
}

int injector::get_gfn_pid() {
	HANDLE h_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (h_snapshot == INVALID_HANDLE_VALUE) return -1;

	PROCESSENTRY32 proc{};
	proc.dwSize = sizeof(PROCESSENTRY32);
	char gfn_proc_name[] = ("GeForceNOW.exe");

	if (Process32First(h_snapshot, &proc)) {
		while (Process32Next(h_snapshot, &proc)) {
			if (memcmp(gfn_proc_name, proc.szExeFile, 15) == 0) {
				CloseHandle(h_snapshot);
				return proc.th32ProcessID;
			}
		}
	}

	CloseHandle(h_snapshot);

	return -1;
}


void injector::inject() {
	std::string path = std::filesystem::current_path().string() + "\\bin\\gfn.dll";
	const char* dll_path = path.c_str();
	printf(XorStr("[injector::inject] %s.\n"), dll_path);

	int pid = get_gfn_pid();
	if (pid == -1) {
		printf(XorStr("[injector::inject] Waiting for Geforce Now to be launched.\n"));
		while (pid == -1) {
			pid = get_gfn_pid();
			Sleep(10);
		}
	}

	printf(XorStr("[injector::inject] Geforce Now found! Injecting...\n"));

	HANDLE gfn_handle = OpenProcess(PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, pid);
	LPVOID address = VirtualAllocEx(gfn_handle, 0, strlen(dll_path) + 1, MEM_COMMIT, PAGE_WRITECOMBINE | PAGE_READWRITE);
	if (!address) {
		printf(XorStr("[injector::inject] Unable to inject due to VirtualAlloc failing\n"));
		return;
	}
	WriteProcessMemory(gfn_handle, address, dll_path, strlen(dll_path) + 1, NULL);



	HMODULE kernel32_module = GetModuleHandleA(XorStr("kernel32.dll"));
	if (!kernel32_module) {
		printf(XorStr("[injector::inject] Unable to inject due to GetModuleHandleA failing\n"));
		return;
	}
	FARPROC loadlibrary_address = GetProcAddress(kernel32_module, XorStr("LoadLibraryA"));
	if (!loadlibrary_address) {
		printf(XorStr("[injector::inject] Unable to inject due to GetProcAddress failing\n"));
		return;
	}
	
	HANDLE remote_thread = CreateRemoteThread(gfn_handle, nullptr, 0, (LPTHREAD_START_ROUTINE)loadlibrary_address, address, 0, nullptr);
	if (remote_thread == INVALID_HANDLE_VALUE) {
		printf(XorStr("[injector::inject] Unable to inject due to CreateRemoteThread failing\n"));
		return;
	}
	g_bGfnInjected = true;
	printf(XorStr("[injector::inject] Aimahead GFN has been injected!\n"));
}

#endif