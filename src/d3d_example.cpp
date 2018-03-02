
#include "com_ptr.hpp"
#include "hook_vtable.hpp"

#include <d3d9.h>

namespace {

HRESULT __stdcall d3d_present_hook(IDirect3DDevice9& device, const RECT* source_rect,
                                   const RECT* dest_rect, HWND dest_window_override,
                                   const RGNDATA* dirty_region) noexcept;

using Create_device_func = HRESULT __stdcall(IDirect3D9&, UINT, D3DDEVTYPE,
                                             HWND, DWORD, D3DPRESENT_PARAMETERS*,
                                             IDirect3DDevice9**) noexcept;

using Present_func = HRESULT __stdcall(IDirect3DDevice9&, const RECT*,
                                       const RECT*, HWND, const RGNDATA*) noexcept;

Create_device_func* d3d_create_device = nullptr;
Present_func* d3d_present = nullptr;

HRESULT __stdcall create_device_hook(IDirect3D9& self, UINT adapter,
                                     D3DDEVTYPE device_type, HWND focus_window,
                                     DWORD behavior_flags,
                                     D3DPRESENT_PARAMETERS* presentation_parameters,
                                     IDirect3DDevice9** returned_device_interface) noexcept
{
   const auto result =
      d3d_create_device(self, adapter, device_type, focus_window, behavior_flags,
                        presentation_parameters, returned_device_interface);

   if (result == S_OK) {
      // Hook IDirect3DDevice9::Present using the device just created.
      d3d_present = hook_vtable<Present_func>(**returned_device_interface, 17,
                                              d3d_present_hook);
   }

   return result;
}

HRESULT __stdcall d3d_present_hook(IDirect3DDevice9& device, const RECT* source_rect,
                                   const RECT* dest_rect, HWND dest_window_override,
                                   const RGNDATA* dirty_region) noexcept
{
   // device has called IDirect3DDevice9::Present, indicating the end of a
   // frame, do something useful with that knowledge here...

   return d3d_present(device, source_rect, dest_rect, dest_window_override, dirty_region);
}
}

void hook_direct3d_present() noexcept
{
   Com_ptr<IDirect3D9> d3d{Direct3DCreate9(D3D_SDK_VERSION)};

   if (!d3d) std::terminate();

   // You can get the vtable index of a function by counting down
   // a class' functions from the top to the function you want to hook
   // and then subtracting one.

   // So the vtable indices of the functions we hook in this example are
   // IDirect3D9::CreateDevice = 16
   // IDirect3DDevice9::Present = 17

   // So first here we hook IDirect3D9::CreateDevice, so we can use the device
   // it creates to hook IDirect3DDevice9::Present.
   d3d_create_device = hook_vtable<Create_device_func>(*d3d, 16, create_device_hook);
}
