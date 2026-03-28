ifeq ($(CROSS),mingw32)
	VPATH=../../ref_gl ../../win32 ../../game
	PLATFORM_SRC=glw_imp.c qgl_win.c q_shared.c q_shwin.c
	FILTER_SRC=gl_sdl.c q_shlinux.c qgl_linux.c glob.c
	LDFLAGS=-static -Wl,--allow-multiple-definition -Wl,--enable-stdcall-fixup -lm -ljpeg -lpng -lz -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm -lcomdlg32 -lole32 -lshell32 -luuid -lcomctl32 -ladvapi32
	TARGET_EXT=.dll
	COMBINE_EXT=-combine.dll
else ifeq ($(CROSS),mingw64)
	VPATH=../../ref_gl ../../win32 ../../game
	PLATFORM_SRC=glw_imp.c qgl_win.c q_shared.c q_shwin.c
	FILTER_SRC=gl_sdl.c q_shlinux.c qgl_linux.c glob.c
	LDFLAGS=-static -Wl,--allow-multiple-definition -Wl,--enable-stdcall-fixup -lm -ljpeg -lpng -lz -lopengl32 -lgdi32 -luser32 -lkernel32 -lwinmm -lcomdlg32 -lole32 -lshell32 -luuid -lcomctl32 -ladvapi32
	TARGET_EXT=.dll
	COMBINE_EXT=-combine.dll
else
	VPATH=../../ref_gl ../../linux ../../game
	PLATFORM_SRC=gl_sdl.c glob.c q_shared.c q_shlinux.c qgl_linux.c
	FILTER_SRC=glw_imp.c qgl_win.c
	LDFLAGS=$(shell sdl-config --libs) -lm -lGL -ljpeg -lpng
	TARGET_EXT=.so
	COMBINE_EXT=-combine.so
endif

CFLAGS+=-fPIC $(shell sdl-config --cflags)

ref_gl_SRC:=gl_draw.c gl_image.c gl_light.c gl_mesh.c gl_model.c gl_rmain.c\
			gl_rmisc.c gl_rsurf.c gl_warp.c
ref_gl_SRC:=$(filter-out $(FILTER_SRC),$(ref_gl_SRC)) $(PLATFORM_SRC)

ref_gl_OBJ:=$(ref_gl_SRC:.c=.o)
ALLSRC:=$(ref_gl_SRC)

.PHONY: default combine

default: ref_gl$(TARGET_EXT)

TARGETS:=ref_gl$(TARGET_EXT) ref_gl-combine$(TARGET_EXT)

include ../make.inc

ref_gl$(TARGET_EXT): $(ref_gl_OBJ)
	$(CC) -shared -g -o $@ $^ $(LDFLAGS)
ifeq ($(CROSS),mingw64)
	$(STRIP) $@
endif

combine: ref_gl-combine$(TARGET_EXT)
ref_gl-combine.so: $(ref_gl_SRC)
	$(CC) $(if $(CROSS),,-sdl-config --cflags) -shared -O2 -ffloat-store -DNDEBUG -DLINUX -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)
ref_gl-combine.dll: $(ref_gl_SRC)
	$(CC) $(if $(CROSS),,-sdl-config --cflags) -shared -O2 -ffloat-store -DNDEBUG -DWIN32 -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)

-include .depends/*.d

