# r1q2

## Changes in this fork

- fixed compiling on Linux
- fixed checking map checksums on 64-bit
- unified resolutions for Win/Lin, added FullHD (`gl_mode 15`)
- added cross compilation on Linux for win32/win64
- win64 build uses now `OpenAL64.dll` instead of `OpenAL32.dll`
- changes win builds so that console is used instead of Windows
  service, so that it works under Wine and I can tested
  dedicated server without having Windows

# Building

```bash
# Building
sudo pacman -S libpng sdl12-compat # Arch/Artix/CachyOS/Manjaro/...
git clone https://github.com/MashedD/r1q2.git
cd r1q2

./build-lin64.sh
# Copy files that were created to where your `r1q2` will be, like so:
# (all of them should have execute permissions)
#     baseq2/gamex86_64.so
#     r1q2
#     r1q2ded
#     ref_gl.so

# Now copy files from non-remastered Quake 2:
#     baseq2/players/
#     baseq2/pak0.pak
#     baseq2/pak1.pak
#     baseq2/pak2.pak

# Create `baseq2/autoexec.cfg` with at least:
#     set vid_ref "gl" // To use ref_gl.so
#     set s_initsound "2" // OpenAL
#     set _windowed_mouse "1" // Fix clipped mouse movement
#     set cl_run "1"
#     set freelook "1"

# Win32
./clean.sh
./build-win32.sh

# Win64
./clean.sh
./build-win64.sh
```

## TODO

- bump version number
- fix compilation warnings
- win32/64 builds should use differently named renderer dll-s

