
#include "d3d_example.hpp"
#include "memory.hpp"

#include <exception>
#include <string>

#include <dinput.h>

namespace {

HMODULE load_dinput_dll() noexcept
{
   std::wstring buffer;
   buffer.resize(512u);

   const auto size = GetSystemDirectoryW(buffer.data(), buffer.size());
   buffer.resize(size);

   buffer += LR"(\DINPUT8.dll)";

   const static auto handle = LoadLibraryW(buffer.c_str());

   if (handle == nullptr) std::terminate();

   return handle;
}

template<typename Func, typename Func_ptr = std::add_pointer_t<Func>>
Func_ptr get_dinput_export(const char* name)
{
   const static auto lib_handle = load_dinput_dll();

   const auto proc = reinterpret_cast<Func_ptr>(GetProcAddress(lib_handle, name));

   if (proc == nullptr) std::terminate();

   return proc;
}
}

extern "C" HRESULT __stdcall directinput8_create(HINSTANCE instance, DWORD version,
                                                 REFIID iid, LPVOID* out,
                                                 LPUNKNOWN unknown_outer)
{
   // Hook Direct3D
   hook_direct3d_present();

   // Example for writing something out to some memory address.

   // write_memory(0x000010_adr, 0ui32);
   // write_memory(0x000014_adr, 0.0f);

   // Or write something out to a protected memory address.
   // write_protected_memory(0x000014_adr, 0.0f);

   // Or maybe even reade something.
   // auto value = read_memory<std::int32_t>(0x000010_adr);

   // Load the address of the real DirectInput8Create.
   using DirectInput8Create_Proc =
      HRESULT __stdcall(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

   const auto directinput_create =
      get_dinput_export<DirectInput8Create_Proc>("DirectInput8Create");

   // Call it.
   return directinput_create(instance, version, iid, out, unknown_outer);
}
