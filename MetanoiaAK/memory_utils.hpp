#pragma once

#include "pointer_utils.hpp"

#include <optional>
#include <Windows.h>


[[nodiscard]]
inline bool read_safe(void *to, const void *from, size_t size)
{
	return ReadProcessMemory(GetCurrentProcess(), from, to, size, NULL) != FALSE;
}

template <typename T>
[[nodiscard]]
inline bool read_safe(T *to, const T *from)
{
	return read_safe(to, from, sizeof(T));
}

template <typename T>
inline std::optional<T> read_safe(const T *from)
{
	alignas(T)
	char buffer[sizeof(T)];

	if (read_safe(buffer, from, sizeof(T)))
		return voff<T>(buffer);

	return std::nullopt;
}


