all:			dstarrepeaterconfig

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeaterconfig:	DStarRepeaterConfigApp.o DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControllerSet.o DStarRepeaterConfigDVAPSet.o \
					DStarRepeaterConfigDVRPTR1Set.o DStarRepeaterConfigDVRPTR2Set.o DStarRepeaterConfigFrame.o DStarRepeaterConfigGMSKSet.o DStarRepeaterConfigModemSet.o \
					DStarRepeaterConfigRaspberrySet.o DStarRepeaterConfigTimesSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterconfig DStarRepeaterConfigApp.o DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControllerSet.o \
					DStarRepeaterConfigDVAPSet.o DStarRepeaterConfigDVRPTR1Set.o DStarRepeaterConfigDVRPTR2Set.o DStarRepeaterConfigFrame.o DStarRepeaterConfigGMSKSet.o \
					DStarRepeaterConfigModemSet.o DStarRepeaterConfigRaspberrySet.o DStarRepeaterConfigTimesSet.o ../Common/Common.a $(LIBS)

DStarRepeaterConfigApp.o:	DStarRepeaterConfigApp.cpp DStarRepeaterConfigApp.h DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h ../Common/DStarDefines.h \
					../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigApp.cpp

DStarRepeaterConfigCallsignSet.o:	DStarRepeaterConfigCallsignSet.cpp DStarRepeaterConfigCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigCallsignSet.cpp

DStarRepeaterConfigControllerSet.o:	DStarRepeaterConfigControllerSet.cpp DStarRepeaterConfigControllerSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigControllerSet.cpp

DStarRepeaterConfigDVAPSet.o:	DStarRepeaterConfigDVAPSet.cpp DStarRepeaterConfigDVAPSet.h ../Common/SerialDataController.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVAPSet.cpp

DStarRepeaterConfigDVRPTR1Set.o:	DStarRepeaterConfigDVRPTR1Set.cpp DStarRepeaterConfigDVRPTR1Set.h ../Common/DVRPTRControllerV1.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVRPTR1Set.cpp

DStarRepeaterConfigDVRPTR2Set.o:	DStarRepeaterConfigDVRPTR2Set.cpp DStarRepeaterConfigDVRPTR2Set.h ../Common/DVRPTRControllerV2.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigDVRPTR2Set.cpp

DStarRepeaterConfigFrame.o:	DStarRepeaterConfigFrame.cpp DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h DStarRepeaterConfigControllerSet.h \
					DStarRepeaterConfigModemSet.h DStarRepeaterConfigTimesSet.h DStarRepeaterConfigCallsignSet.h ../Common/NetworkSet.h ../Common/BeaconSet.h \
					../Common/AnnouncementSet.h ../Common/Control1Set.h ../Common/Control2Set.h ../Common/DStarRepeaterConfig.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigFrame.cpp

DStarRepeaterConfigGMSKSet.o:	DStarRepeaterConfigGMSKSet.cpp DStarRepeaterConfigGMSKSet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigGMSKSet.cpp

DStarRepeaterConfigModemSet.o:	DStarRepeaterConfigModemSet.cpp DStarRepeaterConfigModemSet.h DStarRepeaterConfigDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigModemSet.cpp

DStarRepeaterConfigRaspberrySet.o:	DStarRepeaterConfigRaspberrySet.cpp DStarRepeaterConfigRaspberrySet.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigRaspberrySet.cpp

DStarRepeaterConfigTimesSet.o:	DStarRepeaterConfigTimesSet.cpp DStarRepeaterConfigTimesSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dstarrepeaterconfig $(BINDIR)

clean:
				rm -f core dstarrepeaterconfig *.o *~ *.bak
