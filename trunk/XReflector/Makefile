# Makefile
#
# Build the ircDDB Gateway software on Linux
#

include settings.mk

all:	RemoteControl/remotecontrol XReflector/xreflector

RemoteControl/remotecontrol:	Common/Common.a
		make -C RemoteControl

XReflector/xreflector:	ircDDB/libircDDB.a Common/Common.a
		make -C XReflector

Common/Common.a:
		make -C Common

ircDDB/libircDDB.a:
		make -C ircDDB

install:	all
		make -C RemoteControl install
		make -C XReflector install
		make -C Data install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C RemoteControl clean
		make -C XReflector clean
		
include package.mk
