MAJOR	= 1
MINOR	= 0
SHLIB_LDFLAGS += -Wl,--version-script=libxentoollog.map -shared

CFLAGS	+= -Werror -Wmissing-prototypes
CFLAGS	+= -I./include -I./include/userland

SRCS-y	+= src/xtl_core.c
SRCS-y	+= src/xtl_logger_stdio.c

LIB_OBJS := $(patsubst %.c,%.o,$(SRCS-y))
PIC_OBJS := $(patsubst %.c,%.opic,$(SRCS-y))

# Temporary; for out-of-tree builds.
SYMLINK_SHLIB := ln -sf
SONAME_LDFLAG := -soname

LIB := libxentoollog.a
ifneq ($(nosharedlibs),y)
LIB += libxentoollog.so
endif

.PHONY: all
all: build

.PHONY: build
build:
	$(MAKE) libs

.PHONY: libs
libs: headers.chk $(LIB)

headers.chk: $(wildcard include/*.h)

libxentoollog.a: $(LIB_OBJS)
	$(AR) rc $@ $^

libxentoollog.so: libxentoollog.so.$(MAJOR)
	$(SYMLINK_SHLIB) $< $@
libxentoollog.so.$(MAJOR): libxentoollog.so.$(MAJOR).$(MINOR)
	$(SYMLINK_SHLIB) $< $@

libxentoollog.so.$(MAJOR).$(MINOR): $(PIC_OBJS) libxentoollog.map
	$(CC) $(LDFLAGS) -Wl,$(SONAME_LDFLAG) -Wl,libxentoollog.so.$(MAJOR) $(SHLIB_LDFLAGS) -o $@ $(PIC_OBJS) $(APPEND_LDFLAGS)

%.opic: %.c
	$(CC) $(CPPFLAGS) -DPIC $(CFLAGS) $(CFLAGS_$*.opic) -fPIC -c -o $@ $< $(APPEND_CFLAGS)

.PHONY: install
install: build
	$(INSTALL_DIR) $(DESTDIR)$(libdir)
	$(INSTALL_DIR) $(DESTDIR)$(includedir)
	$(INSTALL_SHLIB) libxentoollog.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) libxentoollog.a $(DESTDIR)$(libdir)
	$(SYMLINK_SHLIB) libxentoollog.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)/libxentoollog.so.$(MAJOR)
	$(SYMLINK_SHLIB) libxentoollog.so.$(MAJOR) $(DESTDIR)$(libdir)/libxentoollog.so
	$(INSTALL_DATA) include/xentoollog.h $(DESTDIR)$(includedir)
	$(INSTALL_DATA) include/userland/xentoollog-osdep.h $(DESTDIR)$(includedir)

.PHONY: TAGS
TAGS:
	etags -t *.c *.h

.PHONY: clean
clean:
	rm -rf *.rpm $(LIB) *~ $(DEPS) $(LIB_OBJS) $(PIC_OBJS)
	rm -f libxentoollog.so.$(MAJOR).$(MINOR) libxentoollog.so.$(MAJOR)
	rm -f headers.chk

.PHONY: distclean
distclean: clean
