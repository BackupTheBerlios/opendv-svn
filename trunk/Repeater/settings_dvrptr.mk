# Makefile-include
#
# Special settings for DVRPTR-Net platform, Hans-J. Barthen, 2012-08-08
# Rename this file to settings.mk before you compile the source for the DVRPTR-Net platform
#
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export BINDIR  := "$(DESTDIR)/opt/opendv/bin"
	export DATADIR := "/opt/opendv/data"
#
# For a native build on ARM add -Wno-psabi to CFLAGS, to get rid of notes
#
	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
	export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
