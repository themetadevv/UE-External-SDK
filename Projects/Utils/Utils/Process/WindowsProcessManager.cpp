
#include "pch.h"

#include "Utils/Process/WindowsProcessManager.h"

namespace Utils {
	WindowsProcessManager::WindowsProcessManager(std::wstring_view process_name, Access access) : 
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

	WindowsProcessManager::~WindowsProcessManager() {
		if (m_ProcessHandle != nullptr) {
			CloseHandle(m_ProcessHandle);
			m_ProcessHandle = nullptr;
		}
	}

	bool WindowsProcessManager::ReadProcessMemoryNativeCall(uintptr_t address, void* buffer, size_t size) {
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

	bool WindowsProcessManager::WriteProcessMemoryNativeCall(uintptr_t address, const void* buffer, size_t size) {
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