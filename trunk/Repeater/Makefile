# Makefile-include
#
# Build the repeater software on Linux
#

include settings.mk

all:	pre-build main-build

pre-build:
		./mksvnrev.sh

main-build:	AnalogueRepeater/analoguerepeater DummyRepeater/dummyrepeater DVAPNode/dvapnode DVRPTRRepeater/dvrptrrepeater GMSKRepeater/gmskrepeater ParrotController/parrotcontroller SoundCardRepeater/soundcardrepeater SplitRepeater/splitrepeater

AnalogueRepeater/analoguerepeater:	Common/Common.a
		make -C AnalogueRepeater

DummyRepeater/dummyrepeater:	Common/Common.a
		make -C DummyRepeater

DVAPNode/dvapnode:	Common/Common.a
		make -C DVAPNode

DVRPTRRepeater/dvrptrrepeater:	Common/Common.a
		make -C DVRPTRRepeater

GMSKRepeater/gmskrepeater:	Common/Common.a
		make -C GMSKRepeater

ParrotController/parrotcontroller:	Common/Common.a
		make -C ParrotController

SoundCardRepeater/soundcardrepeater:	Common/Common.a
		make -C SoundCardRepeater

SplitRepeater/splitrepeater:	Common/Common.a
		make -C SplitRepeater

Common/Common.a:
		make -C Common

install:	all
		install -d -g bin -o root -m 0775 $(BINDIR)
		make -C AnalogueRepeater install
		make -C Data install
		make -C DummyRepeater install
		make -C DVAPNode install
		make -C DVRPTRRepeater install
		make -C GMSKRepeater install
		make -C ParrotController install
		make -C SoundCardRepeater install
		make -C SplitRepeater install

clean:
		rm -f core *~ *.bak
		make -C Common clean
		make -C AnalogueRepeater clean
		make -C DummyRepeater clean
		make -C DVAPNode clean
		make -C DVRPTRRepeater clean
		make -C GMSKRepeater clean
		make -C ParrotController clean
		make -C SoundCardRepeater clean
		make -C SplitRepeater clean

include package.mk
