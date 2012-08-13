# Makefile
#
# Build the ircDDB Gateway software on Linux
#
# Emdebian Compile for ARM processor, John Hays, June 2012
#

export DATADIR := "/usr/local/etc"

# Uncomment the CROSS_COMPILE line for ARM
# export CROSS_COMPILE := ARM

ifeq ($(CROSS_COMPILE),ARM)
#
# Assumes Emdebian Compile on Debian
#
	export BINDIR  := "/usr/local/arm/bin"

	export CC      := arm-linux-gnueabi-g++

	export LDFLAGS := -g -L/usr/arm-linux-gnueabi/lib

	export CFLAGS  := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -Wno-psabi -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/lib/wx/include/gtk2-unicode-release-2.8 -I/usr/arm-linux-gnueabi/include/wx-2.8 -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -pthread
else
	export BINDIR  := "/usr/local/bin"

	export CC      := $(shell wx-config --cxx)

	export LDFLAGS := -g

#
# Comment out the first CFLAGS entry and uncomment the second to enable the DExtra Link. For DCS Link you must comment out the first
# CFLAGS entry and uncomment the thrird entrt. You must do a "make clean" before rebuilding the software.
#
# For a native build on ARM add -Wno-psabi to CFLAGS, to get rid of a warning
#
	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDEXTRA_LINK $(shell wx-config --cxxflags)
#	export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' -DDCS_LINK $(shell wx-config --cxxflags)
endif

export LIBS := $(shell wx-config --libs adv,core)

all:	ircDDBGateway/ircddbgateway RemoteControl/remotecontrol StarNetServer/starnetserver TimerControl/timercontrol TimeServer/timeserver XReflector/xreflector

ircDDBGateway/ircddbgateway:	ircDDB/libircDDB.a Common/Common.a
		make -C ircDDBGateway

RemoteControl/remotecontrol:	Common/Common.a
		make -C RemoteControl

StarNetServer/starnetserver:	ircDDB/libircDDB.a Common/Common.a
		make -C StarNetServer

TimerControl/timercontrol:	Common/Common.a
		make -C TimerControl

TimeServer/timeserver:	Common/Common.a
		make -C TimeServer

XReflector/xreflector:	ircDDB/libircDDB.a Common/Common.a
		make -C XReflector

Common/Common.a:
		make -C Common

ircDDB/libircDDB.a:
		make -C ircDDB

install:	all
		make -C ircDDBGateway install
		make -C RemoteControl install
		make -C StarNetServer install
		make -C TimerControl install
		make -C TimeServer install
		make -C XReflector install
		make -C Data install

clean:
		rm -f core *~ *.bak
		make -C ircDDB clean
		make -C Common clean
		make -C ircDDBGateway clean
		make -C RemoteControl clean
		make -C StarNetServer clean
		make -C TimerControl clean
		make -C TimeServer clean
		make -C XReflector clean
