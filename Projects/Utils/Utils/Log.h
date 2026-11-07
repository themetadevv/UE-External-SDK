#pragma once

#include <iostream>
#include <string>
#include <format>
#include <concepts>

namespace Utils {
	template <typename... Args>
	void Log(std::wstring_view format_str, Args&&... args) {
		std::wcout << std::vformat(format_str, std::make_wformat_args(args...));
	}
}