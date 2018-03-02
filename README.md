# dinput8_hook
This is a very simple example of how to use DirectInput 8 to hook into an application. 
Also included are a couple utilities for making life easier once you're there.

All contents of this repo are [public domain](https://github.com/SleepKiller/dinput8_hook/blob/master/UNLICENSE).

### memory.hpp
This was made to (hopefully) help out a friend. Their use case likely includes writing out to seemingly random memory locations, to
hopefully make their life easier I have included some small functions in [memory.hpp](https://github.com/SleepKiller/dinput8_hook/blob/master/src/memory.hpp)
for doing just that.

### hook_vtable.hpp
If you're interested in hooking into DirectInput, their is a reasonable chance you're also interested in vtable hooking. 
The function hook_vtable is included to aid anyone seeking to do this.

### d3d_example
Also included is a primitive example on how to use hook_vtable to tap into Direct3DDevice9::Present calls.
