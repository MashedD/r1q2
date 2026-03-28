ifeq ($(CROSS),mingw32)
	VPATH=../../game ../../win32 ../../qcommon ../../server ../../client
	PLATFORM_SRC=sys_win.c q_shwin.c in_win.c vid_dll.c vid_menu.c snd_win.c alw_win.c cd_win.c net_wins.c qal_win.c conproc.c
	FILTER_SRC=sys_linux.c q_shlinux.c vid_so.c snd_linux.c al_linux.c cd_linux.c qal_linux.c net_udp.c glob.c
	LDFLAGS=-static -lwinmm -lopengl32 -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomdlg32 -lole32 -luuid -lcomctl32 -ldsound -ldinput8 -ldxguid -lws2_32 -lm -lz
	ARCH=32
	TARGET_EXT=.exe
else ifeq ($(CROSS),mingw64)
	VPATH=../../game ../../win32 ../../qcommon ../../server ../../client
	PLATFORM_SRC=sys_win.c q_shwin.c in_win.c vid_dll.c vid_menu.c snd_win.c alw_win.c cd_win.c net_wins.c qal_win.c conproc.c
	FILTER_SRC=sys_linux.c q_shlinux.c vid_so.c snd_linux.c al_linux.c cd_linux.c qal_linux.c net_udp.c glob.c
	LDFLAGS=-static -Wl,--enable-stdcall-fixup -lm -lz -lws2_32 -lwinmm -lopengl32 -lkernel32 -luser32 -lgdi32 -ladvapi32 -lshell32 -lcomdlg32 -lole32 -luuid -lcomctl32 -ldsound -ldinput8 -ldxguid
	ARCH=
	TARGET_EXT=.exe
else
	VPATH=../../game ../../linux ../../qcommon ../../server ../../client
	PLATFORM_SRC=sys_linux.c q_shlinux.c vid_so.c vid_menu.c snd_linux.c al_linux.c cd_linux.c net_udp.c qal_linux.c
	FILTER_SRC=sys_win.c q_shwin.c vid_dll.c snd_win.c alw_win.c cd_win.c net_wins.c qal_win.c
	LDFLAGS=-lm -lz
	ARCH=
	TARGET_EXT=
ifeq ($(shell uname),Linux)
		LDFLAGS+=-ldl -lopenal
endif
ifeq ($(shell uname),FreeBSD)
		LDFLAGS+=-lexecinfo
endif
endif

quake2_SRC:=cl_cin.c cl_ents.c cl_fx.c cl_input.c cl_inv.c cl_main.c\
	    cl_parse.c cl_pred.c cl_tent.c cl_scrn.c cl_view.c cl_newfx.c\
	    console.c keys.c menu.c snd_dma.c snd_mem.c snd_mix.c qmenu.c\
	    m_flash.c\
	    cmd.c cmodel.c common.c crc.c cvar.c files.c md4.c net_chan.c\
	    sv_ccmds.c sv_ents.c sv_game.c sv_init.c sv_main.c sv_send.c\
	    sv_user.c sv_world.c \
	    q_shlinux.c vid_menu.c vid_so.c sys_linux.c glob.c net_udp.c\
	    q_shared.c pmove.c mersennetwister.c le_util.c\
	    le_physics.c redblack.c cd_linux.c snd_linux.c unzip.c ioapi.c\
	    al_linux.c qal_linux.c

quake2_SRC:=$(filter-out $(FILTER_SRC),$(quake2_SRC)) $(PLATFORM_SRC)

CFLAGS+=-DUSE_OPENAL
quake2_OBJ:=$(quake2_SRC:.c=.o)
ALLSRC:=$(quake2_SRC)

.PHONY: default combine

default: r1q2$(ARCH)$(TARGET_EXT)

TARGETS:=r1q2$(ARCH)$(TARGET_EXT) combine r1q2-combine$(TARGET_EXT)

include ../make.inc

r1q2$(ARCH)$(TARGET_EXT): $(quake2_OBJ)
	$(CC) -g -o $@ $^ $(LDFLAGS)
ifneq ($(findstring mingw,$(CROSS)),)
	$(STRIP) $@
endif

# only works with gcc >4.1
combine: r1q2$(ARCH)-combine$(TARGET_EXT)
r1q2$(ARCH)-combine: $(quake2_SRC)
	$(CC) -O2 -ffloat-store -DNDEBUG -DLINUX -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)
r1q2$(ARCH)-combine.exe: $(quake2_SRC)
	$(CC) -O2 -ffloat-store -DNDEBUG -DWIN32 -fwhole-program --combine -g -o $@ $^ $(LDFLAGS)

-include .depends/*.d

