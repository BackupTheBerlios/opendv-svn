# Makefile
# make Digital Voice on Linux (Ubuntu)
# Jonathan Naylor, March 2009

export BINDIR  := "/usr/local/bin"
export DATADIR := "/usr/local/etc"
export DESKTOPDIR := "/usr/local/share/applications"
export PNGDIR := "/usr/local/share/pixmaps"

#
# Change your architecture here
#
# export ARCH := pentium
# export INTELO := ../i386/Intel.o
# export LIBNODE := -lnode-i386
export ARCH := x86-64
export INTELO := ../AMD64/Intel.o
export LIBNODE := -lnode-x86-64


export CC := $(shell wx-config --cxx)

export LIBS := -lportaudio -lusb $(shell wx-config --libs adv,core)

export CFLAGS := -g -O2 -Wall -Wno-non-virtual-dtor -Wno-strict-aliasing -march=$(ARCH) -DDATA_DIR='$(DATADIR)' -DBIN_DIR='$(BINDIR)' $(shell wx-config --cxxflags)

export LDFLAGS := -g

all:			DCSClient/DCSClient DExtraClient/DExtraClient DStarClient/DStarClient DVAPClient/DVAPClient DVRPTRClient/DVRPTRClient DVToolReader/DVToolReader GMSKClient/GMSKClient

DCSClient/DCSClient:	Common/Common.a
				make -C DCSClient

DExtraClient/DExtraClient:	Common/Common.a
				make -C DExtraClient

DStarClient/DStarClient:	Common/Common.a
				make -C DStarClient

DVAPClient/DVAPClient:	Common/Common.a
				make -C DVAPClient

DVRPTRClient/DVRPTRClient:	Common/Common.a
				make -C DVRPTRClient

DVToolReader/DVToolReader:	Common/Common.a
				make -C DVToolReader

GMSKClient/GMSKClient:	Common/Common.a
				make -C GMSKClient

Common/Common.a:
				make -C Common

install:		all
				make -C DCSClient install
				make -C DExtraClient install
				make -C DStarClient install
				make -C DVAPClient install
				make -C DVRPTRClient install
				make -C DVToolReader install
				make -C GMSKClient install

clean:
				rm -f core *~ *.bak
				make -C Common clean
				make -C DCSClient clean
				make -C DExtraClient clean
				make -C DStarClient clean
				make -C DVAPClient clean
				make -C DVRPTRClient clean
				make -C DVToolReader clean
				make -C GMSKClient clean
