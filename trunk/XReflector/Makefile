# Makefile
#
# Build the ircDDB Gateway software on Linux
#

include settings.mk

all:	XRFRemote/xrfremote XReflector/xreflector

XRFRemote/xrfremote:	Common/Common.a
		make -C RemoteControl

XReflector/xreflector:	ircDDB/libircDDB.a Common/Common.a
		make -C XReflector

Common/Common.a:
		make -C Common

ircDDB/libircDDB.a:
		make -C ircDDB

install:	all
		make -C XRFRemote install
		make -C XReflector install
		make -C Data install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C XRFRemote clean
		make -C XReflector clean
		
include package.mk
