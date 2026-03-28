ifeq ($(CROSS),mingw32)
	VPATH=../../game ../../win32 ../../qcommon ../../server
	PLATFORM_SRC=sys_win.c q_shwin.c net_wins.c conproc.c
	FILTER_SRC=sys_linux.c q_shlinux.c net_udp.c glob.c
	LDFLAGS=-static -Wl,--allow-multiple-definition -Wl,--enable-stdcall-fixup -lm -lz -lwinmm -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomdlg32 -lole32 -luuid -lcomctl32 -lws2_32
	CFLAGS+=-fno-lto
	ARCH=32
	TARGET_EXT=.exe
else ifeq ($(CROSS),mingw64)
	VPATH=../../game ../../win32 ../../qcommon ../../server
	PLATFORM_SRC=sys_win.c q_shwin.c net_wins.c conproc.c
	FILTER_SRC=sys_linux.c q_shlinux.c net_udp.c glob.c
	LDFLAGS=-mconsole -static -Wl,--allow-multiple-definition -Wl,--enable-stdcall-fixup -lm -lz -lws2_32 -lwinmm -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomdlg32 -lole32 -luuid -lcomctl32
	TARGET_EXT=.exe
	ARCH=
	CC=i686-w64-mingw32-gcc
else
	VPATH=../../game ../../linux ../../qcommon ../../server
	PLATFORM_SRC=sys_linux.c q_shlinux.c net_udp.c
	FILTER_SRC=sys_win.c q_shwin.c net_wins.c
	LDFLAGS=-lm -lz
	ARCH=
	TARGET_EXT=
ifeq ($(shell uname),Linux)
		LDFLAGS+=-ldl
endif
ifeq ($(shell uname),FreeBSD)
		LDFLAGS+=-lexecinfo
endif
endif

CFLAGS+=-DDEDICATED_ONLY -DANTICHEAT

r1q2ded_SRC:=cmd.c cmodel.c common.c crc.c cvar.c files.c md4.c net_chan.c \
	     mersennetwister.c redblack.c sv_ccmds.c sv_ents.c sv_game.c \
	     sv_init.c sv_main.c sv_send.c sv_user.c sv_world.c \
	     sys_linux.c glob.c net_udp.c q_shared.c pmove.c ioapi.c unzip.c \
	     sv_anticheat.c
r1q2ded_SRC:=$(filter-out $(FILTER_SRC),$(r1q2ded_SRC)) $(PLATFORM_SRC)

r1q2ded_OBJ:=$(r1q2ded_SRC:.c=.o)
ALLSRC:=$(r1q2ded_SRC)

.PHONY: default combine

default: r1q2ded$(ARCH)$(TARGET_EXT)

TARGETS:=r1q2ded$(ARCH)$(TARGET_EXT) r1q2ded$(ARCH)-combine$(TARGET_EXT)

r1q2ded$(ARCH)$(TARGET_EXT): $(r1q2ded_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
ifneq ($(findstring mingw,$(CROSS)),)
	$(STRIP) $@
endif

combine: r1q2ded$(ARCH)-combine$(TARGET_EXT)
r1q2ded$(ARCH)-combine: $(r1q2ded_SRC)
	$(CC) -O2 -ffloat-store -DNDEBUG -LINUX -DDEDICATED_ONLY -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)
r1q2ded$(ARCH)-combine.exe: $(r1q2ded_SRC)
	$(CC) -O2 -ffloat-store -DNDEBUG -DWIN32 -DDEDICATED_ONLY -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)

include ../make.inc

-include .depends/*.d
