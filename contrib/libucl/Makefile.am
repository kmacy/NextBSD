ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = uthash klib README.md

pkgconfigdir = $(prefix)/libdata/pkgconfig
pkgconfig_DATA = libucl.pc

if LUA_SUB
  LUA_SUBDIR = lua
endif

SUBDIRS = src tests utils doc $(LUA_SUBDIR)
