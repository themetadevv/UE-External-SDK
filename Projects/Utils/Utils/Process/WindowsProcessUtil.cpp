
#include "pch.h"

#include "Utils/Process/WindowsProcessUtil.h"

namespace Utils {
	DWORD GetProcessIDByName(std::wstring_view process_name) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE)
			return 0;

		PROCESSENTRY32W entry = {};
		entry.dwSize = sizeof(entry);

		DWORD pid = 0;
		if (Process32FirstW(snapshot, &entry)) {
			do
			{
				if (_wcsicmp(entry.szExeFile, process_name.data()) == 0) {
					pid = entry.th32ProcessID;
					break;
				}
			} while (Process32NextW(snapshot, &entry));
		}

		return pid;
	}

	uintptr_t GetModuleBaseAddress(std::wstring_view module_name, DWORD pid) {
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		if (snapshot == INVALID_HANDLE_VALUE)
			return 0;

		MODULEENTRY32W entry = {};
		entry.dwSize = sizeof(entry);

		uintptr_t base_address = 0;
		if (Module32FirstW(snapshot, &entry)) {
			do 
			{
				if (_wcsicmp(entry.szModule, module_name.data()) == 0) {
					base_address = reinterpret_cast<uintptr_t>(entry.modBaseAddr);
					break;
				}
			} while (Module32NextW(snapshot, &entry));
		}

		return base_address;
	}

}