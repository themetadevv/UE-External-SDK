#pragma once

#include <string>

#include <windows.h>
#include <TlHelp32.h>

#include "Utils/Common.h"

namespace Utils::Windows {
	// not applicable for games protected by kernel level anticheat,
	// using this will result in instant ban.

	// kernel level ac : EAC, BE, etc.
	// user mode ac: VAC, etc. (safe to use but consider using an IOCTL driver for extra safety)

	DWORD GetProcessIDByName(std::wstring_view process_name);
	uintptr_t GetModuleBaseAddress(std::wstring_view module_name, DWORD pid);

	class ProcessManager {
	public:
		enum class Access : DWORD {
			None  = 0,
			Read  = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			Write = PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
			All   = PROCESS_ALL_ACCESS
		};
		
		friend constexpr Access operator|(Access a, Access b) {
			return static_cast<Access>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}

	private:
		std::wstring m_ProcessName;
		DWORD  m_ProcessID = 0;
		HANDLE m_ProcessHandle = nullptr;

	public:
		ProcessManager() = default;
		ProcessManager(std::wstring_view process_name, Access access);
		~ProcessManager();

	private:
		// private methods
		bool ReadProcessMemoryNativeCall(uintptr_t address, void* buffer, size_t size);
		bool WriteProcessMemoryNativeCall(uintptr_t address, const void* buffer, size_t size);

	public:
		// public methods
		template <typename T>
		bool Read(uintptr_t address, T& value) {
			return ReadProcessMemoryNativeCall(address, &value, sizeof(T));
		}

		template <typename T>
		T Read(uintptr_t address) {
			T v = {};
			ReadProcessMemoryNativeCall(address, &v, sizeof(T));
			return v;
		}

		template <typename T>
		bool Write(uintptr_t address, const T& value) {
			return WriteProcessMemoryNativeCall(address, &value, sizeof(T));
		}

	public:
		// getters
		DWORD GetProcessID() const { return m_ProcessID; }
	};
}