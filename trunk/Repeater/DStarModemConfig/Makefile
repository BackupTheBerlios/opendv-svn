all:			dstarmodemconfig

CFLAGS :=		-I../Common $(CFLAGS)

dstarmodemconfig:	DStarModemConfigApp.o DStarModemConfigFrame.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarmodemconfig DStarModemConfigApp.o DStarModemConfigFrame.o ../Common/Common.a $(LIBS)

DStarModemConfigApp.o:	DStarModemConfigApp.cpp DStarModemConfigApp.h DStarModemConfigFrame.h DStarModemConfigDefs.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarModemConfigApp.cpp

DStarModemConfigFrame.o:	DStarModemConfigFrame.cpp DStarModemConfigFrame.h DStarModemConfigDefs.h DStarModemDVAPSet.h DStarModemDVRPTR1.h DStarModemDVRPTR2Set.h \
					DStarModemGMSKSet.h ../Common/DStarModemConfig.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarModemConfigFrame.cpp

DStarModemDVAPSet.o:	DStarModemDVAPSet.cpp DStarModemDVAPSet.h DStarModemConfigDefs.h ../Common/DStarDefines.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DStarModemDVAPSet.cpp

DStarModemDVRPTR1Set.o:	DStarModemDVRPTR1Set.cpp DStarModemDVRPTR1Set.h DStarModemConfigDefs.h ../Common/DStarDefines.h ../Common/DVRPTRControllerV1.h
				$(CC) $(CFLAGS) -c DStarModemDVRPTR1Set.cpp

DStarModemDVRPTR2Set.o:	DStarModemDVRPTR2Set.cpp DStarModemDVRPTR2Set.h DStarModemConfigDefs.h ../Common/DStarDefines.h ../Common/DVRPTRControllerV2.h \
					../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c DStarModemDVRPTR2Set.cpp

DStarModemGMSKSet.o:	DStarModemGMSKSet.cpp DStarModemGMSKSet.h DStarModemConfigDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarModemGMSKSet.cpp

install:	all
				install -g bin -o root -m 0775 dstarmodemconfig $(BINDIR)

clean:
				rm -f core dstarmodemconfig *.o *~ *.bak
