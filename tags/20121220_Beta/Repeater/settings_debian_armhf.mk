# Makefile-include
#
# Special settings for  Emdebian Compile for ARM processor, John Hays, June 2012
# modified by Hans-J. Barthen, DL5DI, 2012-10-04
# Rename this file to settings.mk before you compile the source for the Debian armhf platform
#
	export DATADIR := "/home/opendv/data"
	export BINDIR  := "$(DESTDIR)/usr/bin"
	export CC      := arm-linux-gnueabihf-g++
	export LDFLAGS := -g -L/usr/arm-linux-gnueabihf/lib 
	export CFLAGS  := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabihf/include -I/usr/arm-linux-gnueabihf/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabihf/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
	export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)
