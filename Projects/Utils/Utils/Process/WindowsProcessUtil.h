#pragma once

// stdlib
#include <string>

// win api
#include <windows.h>
#include <TlHelp32.h>

#include "Utils/Common.h"

namespace Utils {
	DWORD GetProcessIDByName(std::wstring_view process_name);
	uintptr_t GetModuleBaseAddress(std::wstring_view module_name, DWORD pid);
}