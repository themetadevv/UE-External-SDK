#pragma once

#include "Utils/Common.h"
#include "Utils/Process/WindowsProcessUtil.h"

namespace Utils {
	class WindowsProcessManager {
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
		WindowsProcessManager() = default;
		WindowsProcessManager(std::wstring_view process_name, Access access);
		~WindowsProcessManager();

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