# Makefile
# make Digital Voice on Linux (Ubuntu)
# Jonathan Naylor, March 2009

include settings.mk

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
