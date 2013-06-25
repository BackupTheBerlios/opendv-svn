# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	DStarRepeater/dstarrepeater DStarRepeaterConfig/dstarrepeaterconfig DummyRepeater/dummyrepeater DVAPNode/dvapnode \
			DVRPTRRepeater/dvrptrrepeater GMSKRepeater/gmskrepeater SoundCardRepeater/soundcardrepeater SplitRepeater/splitrepeater

DStarRepeater/dstarrepeater:	Common/Common.a
		make -C DStarRepeater

DStarRepeaterConfig/dstarrepeaterconfig:	Common/Common.a
		make -C DStarRepeaterConfig

DummyRepeater/dummyrepeater:	Common/Common.a
		make -C DummyRepeater

DVAPNode/dvapnode:	Common/Common.a
		make -C DVAPNode

DVRPTRRepeater/dvrptrrepeater:	Common/Common.a
		make -C DVRPTRRepeater

GMSKRepeater/gmskrepeater:	Common/Common.a
		make -C GMSKRepeater

SoundCardRepeater/soundcardrepeater:	Common/Common.a
		make -C SoundCardRepeater

SplitRepeater/splitrepeater:	Common/Common.a
		make -C SplitRepeater

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C Data install
		make -C DStarRepeater install
		make -C DStarRepeaterConfig install
		make -C DummyRepeater install
		make -C DVAPNode install
		make -C DVRPTRRepeater install
		make -C GMSKRepeater install
		make -C SoundCardRepeater install
		make -C SplitRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C DStarRepeater clean
		make -C DStarRepeaterConfig clean
		make -C DummyRepeater clean
		make -C DVAPNode clean
		make -C DVRPTRRepeater clean
		make -C GMSKRepeater clean
		make -C SoundCardRepeater clean
		make -C SplitRepeater clean

include package.mk
