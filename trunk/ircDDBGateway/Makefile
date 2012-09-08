# Makefile
#
# Build the ircDDB Gateway software on Linux
#

include settings.mk

all:	pre-build main-build

pre-build:
		./mksvnrev.sh

main-build:	ircDDBGateway/ircddbgateway RemoteControl/remotecontrol StarNetServer/starnetserver TimerControl/timercontrol TimeServer/timeserver XReflector/xreflector

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
		
include package.mk
