all:			dstarrepeaterconfig

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeaterconfig:	DStarRepeaterConfigApp.o DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControllerSet.o DStarRepeaterConfigFrame.o \
					DStarRepeaterConfigModemSet.o DStarRepeaterConfigPreferences.o DStarRepeaterConfigTimesSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterconfig DStarRepeaterConfigApp.o DStarRepeaterConfigCallsignSet.o DStarRepeaterConfigControllerSet.o \
					DStarRepeaterConfigFrame.o DStarRepeaterConfigModemSet.o DStarRepeaterConfigPreferences.o DStarRepeaterConfigTimesSet.o ../Common/Common.a $(LIBS)

DStarRepeaterConfigApp.o:	DStarRepeaterConfigApp.cpp DStarRepeaterConfigApp.h DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h ../Common/DStarDefines.h \
					../Common/DStarRepeaterConfig.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigApp.cpp

DStarRepeaterConfigCallsignSet.o:	DStarRepeaterConfigCallsignSet.cpp DStarRepeaterConfigCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigCallsignSet.cpp

DStarRepeaterConfigControllerSet.o:	DStarRepeaterConfigControllerSet.cpp DStarRepeaterConfigControllerSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigControllerSet.cpp

DStarRepeaterConfigFrame.o:	DStarRepeaterConfigFrame.cpp DStarRepeaterConfigFrame.h DStarRepeaterConfigDefs.h DStarRepeaterConfigApp.h \
					DStarRepeaterConfigPreferences.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigFrame.cpp

DStarRepeaterConfigModemSet.o:	DStarRepeaterConfigModemSet.cpp DStarRepeaterConfigModemSet.h DStarRepeaterConfigDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigModemSet.cpp

DStarRepeaterConfigPreferences.o:	DStarRepeaterConfigPreferences.cpp DStarRepeaterConfigPreferences.h DStarRepeaterConfigDefs.h DStarRepeaterConfigControllerSet.h \
					DStarRepeaterConfigModemSet.h DStarRepeaterConfigApp.h DStarRepeaterConfigTimesSet.h DStarRepeaterConfigCallsignSet.h ../Common/NetworkSet.h \
					../Common/BeaconSet.h ../Common/AnnouncementSet.h ../Common/DStarDefines.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigPreferences.cpp

DStarRepeaterConfigTimesSet.o:	DStarRepeaterConfigTimesSet.cpp DStarRepeaterConfigTimesSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfigTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dstarrepeaterconfig $(BINDIR)

clean:
				rm -f core dstarrepeaterconfig *.o *~ *.bak
