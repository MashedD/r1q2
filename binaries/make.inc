CFLAGS+=-DNDEBUG -O2 -fno-strict-aliasing -Wall -ffloat-store -pipe -g -MF .depends/$*.d -MMD -std=gnu99 -Wno-incompatible-pointer-types
ifeq ($(CROSS),mingw32)
	CFLAGS+=-DWIN32 -Wno-pedantic-ms-format -isystem../../win32
	CC=i686-w64-mingw32-gcc -mno-mmx -mno-sse
	LD=i686-w64-mingw32-gcc
	STRIP=i686-w64-mingw32-strip
	WINDRES=i686-w64-mingw32-windres
else ifeq ($(CROSS),mingw64)
	CFLAGS+=-DWIN32 -Wno-pedantic-ms-format -isystem../../win32
	CC=x86_64-w64-mingw32-gcc
	LD=x86_64-w64-mingw32-gcc
	STRIP=x86_64-w64-mingw32-strip
	WINDRES=x86_64-w64-mingw32-windres
else
	CFLAGS+=-DLINUX
endif
ALLOBJ:=$(ALLSRC:.c=.o)

.PHONY: clean distclean

clean:
	-rm -f *.o $(TARGETS)

DEPENDS=$(foreach file, $(ALLSRC:.c=.d), .depends/$(file))

distclean: clean
	-rm -f $(DEPENDS)

-include $(DEPENDS)

