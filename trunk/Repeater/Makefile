# Makefile
#
# Build the repeater software on Linux
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
# For a native build on ARM add -Wno-psabi to CFLAGS, to get rid of a warning
#
	export CFLAGS  := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)
endif

export LIBS := -lportaudio -lusb-1.0 $(shell wx-config --libs adv,core)

all:	AnalogueRepeater/analoguerepeater DCSGateway/dcsgateway DExtraGateway/dextragateway DStarRepeater/dstarrepeater DummyRepeater/dummyrepeater DVAPNode/dvapnode DVRPTRRepeater/dvrptrrepeater GMSKRepeater/gmskrepeater ParrotController/parrotcontroller SplitRepeater/splitrepeater

AnalogueRepeater/analoguerepeater:	Common/Common.a
		make -C AnalogueRepeater

DExtraGateway/dextragateway:	Common/Common.a
		make -C DExtraGateway

DCSGateway/dcsgateway:	Common/Common.a
		make -C DCSGateway

DStarRepeater/dstarrepeater:	Common/Common.a
		make -C DStarRepeater

DummyRepeater/dummyrepeater:	Common/Common.a
		make -C DummyRepeater

DVAPNode/dvapnode:	Common/Common.a
		make -C DVAPNode

DVRPTRRepeater/dvrptrrepeater:	Common/Common.a
		make -C DVRPTRRepeater

GMSKRepeater/gmskrepeater:	Common/Common.a
		make -C GMSKRepeater

Common/Common.a:
		make -C Common

ParrotController/parrotcontroller:	Common/Common.a
		make -C ParrotController

SplitRepeater/splitrepeater:	Common/Common.a
		make -C SplitRepeater

install:	all
		make -C AnalogueRepeater install
		make -C Data install
		make -C DCSGateway install
		make -C DExtraGateway install
		make -C DStarRepeater install
		make -C DummyRepeater install
		make -C DVAPNode install
		make -C DVRPTRRepeater install
		make -C GMSKRepeater install
		make -C ParrotController install
		make -C SplitRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C AnalogueRepeater clean
		make -C DCSGateway clean
		make -C DExtraGateway clean
		make -C DStarRepeater clean
		make -C DummyRepeater clean
		make -C DVAPNode clean
		make -C DVRPTRRepeater clean
		make -C GMSKRepeater clean
		make -C ParrotController clean
		make -C SplitRepeater clean
