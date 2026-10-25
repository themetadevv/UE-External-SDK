
#include "pch.h"

#include "Utils/Windows/ProcessManager.h"

namespace Utils::Windows {
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

	ProcessManager::ProcessManager(std::wstring_view process_name, Access access) : 
		m_ProcessName(process_name)
	{
		m_ProcessID = GetProcessIDByName(process_name);
		if (m_ProcessID == 0) {
			std::cout << "Invalid Process ID returning!\n";
			return;
		}

		m_ProcessHandle = OpenProcess(static_cast<DWORD>(access), FALSE, m_ProcessID);
		if (m_ProcessHandle == NULL) {
			std::cout << "Invalid Process Handle returning!\n";
			return;
		}
	}

	ProcessManager::~ProcessManager() {
		if (m_ProcessHandle != nullptr) {
			CloseHandle(m_ProcessHandle);
			m_ProcessHandle = nullptr;
		}
	}

	bool ProcessManager::ReadProcessMemoryNativeCall(uintptr_t address, void* buffer, size_t size) {
		SIZE_T bytes_read = 0;
		BOOL ok = ReadProcessMemory(
			m_ProcessHandle,
			reinterpret_cast<LPCVOID>(address),
			buffer,
			size,
			&bytes_read
		);

		return ok && bytes_read == size;
	}

	bool ProcessManager::WriteProcessMemoryNativeCall(uintptr_t address, const void* buffer, size_t size) {
		SIZE_T bytes_written = 0;
		BOOL ok = WriteProcessMemory(
			m_ProcessHandle,
			reinterpret_cast<LPVOID>(address),
			buffer,
			size,
			&bytes_written
		);

		return ok && bytes_written == size;
	}

}