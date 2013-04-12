all:			dstarmodemd

CFLAGS :=		-I../Common $(CFLAGS)

dstarmodemd:	DStarModemD.o DStarModemDVAPController.o DStarModemNull.o DStarModemThread.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarmodemd DStarModemD.o DStarModemDVAPController.o DStarModemNull.o DStarModemThread.o ../Common/Common.a $(LIBS)

DStarModemD.o:	DStarModemD.cpp DStarModemD.h DStarModem.h DStarModemDefs.h DStarModemDVAPController.h DStarModemNull.h DStarModemThread.h \
					../Common/ModemProtocolServer.h ../Common/DStarModemConfig.h ../Common/DStarDefines.h ../Common/Version.h ../Common/Logger.h
				$(CC) $(CFLAGS) -c DStarModemD.cpp

DStarModemDVAPController.o:	DStarModemDVAPController.cpp DStarModemDVAPController.h DStarModem.h ../Common/SerialDataController.h ../Common/RingBuffer.h \
					../Common/HeaderData.h ../Common/Utils.h ../Common/CCITTChecksumReverse.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarModemDVAPController.cpp

DStarModemNull.o:	DStarModemNull.cpp DStarModemNull.h DStarModem.h ../Common/HeaderData.h
				$(CC) $(CFLAGS) -c DStarModemNull.cpp

DStarModemThread.o:	DStarModemThread.cpp DStarModemThread.h DStarModem.h ../Common/Timer.h ../Common/HeaderData.h ../Common/ModemProtocolServer.h
				$(CC) $(CFLAGS) -c DStarModemThread.cpp

install:	all
				install -g bin -o root -m 0775 dstarmodemd $(BINDIR)

clean:
				rm -f core dstarmodemd *.o *~ *.bak
