# Makefile-include
#
# Special settings for DVRPTR-Net platform, Hans-J. Barthen, 2012-08-08
# Rename this file to settings.mk before you compile the source for the DVRPTR-Net platform
#
#
	export LIBS := $(shell wx-config --libs adv,core)
	export CC      := $(shell wx-config --cxx)
	export LDFLAGS := -g
	export BINDIR  := $(DESTDIR)/usr/bin
	export DATADIR := "/var/opendv/data"
#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the thrird entrt. You must do a "make clean" before rebuilding the software.
#
# For a native build on ARM add -Wno-psabi to CFLAGS, to get rid of notes
#
	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
# 	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDEXTRA_LINK $(shell wx-config --cxxflags)
# 	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDCS_LINK $(shell wx-config --cxxflags)
