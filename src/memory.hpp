#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>

#include <Windows.h>

// User-defined literal for defining a memory address in code.
// usage:
// auto test = 0x000010_adr;
constexpr std::byte* operator""_adr(unsigned long long value)
{
   return reinterpret_cast<std::byte*>(value);
}

// Function for writing a trivial value to a memory address.
// usage:
// write_memory(0x000010_adr, 2048i32);
template<typename Type>
inline void write_memory(std::byte* const dest, const Type& value)
{
   static_assert(std::is_trivial_v<Type>,
                 "The type of the value to write to must be trivial.");

   std::memcpy(dest, &value, sizeof(Type));
}

// Function for reading a trivial value to a memory address.
// usage:
// auto value = read_memory<std::int32_t>(0x000010_adr);
template<typename Type>
inline Type read_memory(const std::byte* const from)
{
   static_assert(std::is_trivial_v<Type>,
                 "The type of the value to read to must be trivial.");

   Type value;

   std::memcpy(&value, from, sizeof(Type));

   return value;
}

// Function for writing a trivial value to a protected memory address.
// usage:
// write_protected_memory(0x000010_adr, 2048i32);
template<typename Type>
inline void write_protected_memory(std::byte* const dest, const Type& value)
{
   static_assert(std::is_trivial_v<Type>,
                 "The type of the value to write to must be trivial.");

   DWORD old_protect{};

   VirtualProtect(dest, sizeof(Type), PAGE_EXECUTE_READWRITE, &old_protect);

   std::memcpy(dest, &value, sizeof(Type));

   VirtualProtect(dest, sizeof(Type), old_protect, nullptr);
}
