ifeq ($(CROSS),mingw32)
	ARCH=x86
	TARGET_EXT=.dll
else ifeq ($(CROSS),mingw64)
	ARCH=x86_64
	TARGET_EXT=.dll
else
	# this nice line comes from the linux kernel makefile
	ARCH := $(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ -e s/arm.*/arm/ -e s/sa110/arm/ -e s/alpha/axp/)
	TARGET_EXT=.so
endif

VPATH=../../game

CFLAGS+=-fPIC -DGAME_DLL

game_SRC:=g_ai.c g_chase.c g_cmds.c g_combat.c g_func.c g_items.c g_main.c\
	  g_misc.c g_monster.c g_phys.c g_save.c g_spawn.c g_svcmds.c\
	  g_target.c g_trigger.c g_turret.c g_utils.c g_weapon.c m_actor.c\
	  m_berserk.c m_boss2.c m_boss3.c m_boss31.c m_boss32.c m_brain.c\
	  m_chick.c m_flash.c m_flipper.c m_float.c m_flyer.c m_gladiator.c\
	  m_gunner.c m_hover.c m_infantry.c m_insane.c m_medic.c m_move.c\
	  m_mutant.c m_parasite.c m_soldier.c m_supertank.c m_tank.c\
	  p_client.c p_hud.c p_trail.c p_view.c p_weapon.c q_shared.c

game_OBJ:=$(game_SRC:.c=.o)
ALLSRC:=$(game_SRC)

.PHONY: default combine

default: game$(ARCH)$(TARGET_EXT)

TARGETS:=game$(ARCH)$(TARGET_EXT) game$(ARCH)-combine$(TARGET_EXT)

game$(ARCH)$(TARGET_EXT): $(game_OBJ)
	$(CC) -shared -g -o $@ $^ $(LDFLAGS)
ifneq ($(findstring mingw,$(CROSS)),)
	$(STRIP) $@
endif

include ../make.inc

#-include .depends/*.d
