# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

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
