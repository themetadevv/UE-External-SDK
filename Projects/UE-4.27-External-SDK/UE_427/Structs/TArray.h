#pragma once

#include <Utils/Common.h>

template <typename T>
struct TArray {
	uintptr_t Data;
	int Count;
	int Max;
};