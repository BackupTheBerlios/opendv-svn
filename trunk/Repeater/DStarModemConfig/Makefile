all:			dstarmodemconfig

CFLAGS :=		-I../Common $(CFLAGS)

dstarmodemconfig:	DStarModemConfigApp.o DStarModemConfigFrame.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarmodemconfig DStarModemConfigApp.o DStarModemConfigFrame.o ../Common/Common.a $(LIBS)

DStarModemConfigApp.o:	DStarModemConfigApp.cpp DStarModemConfigApp.h DStarModemConfigFrame.h DStarModemConfigDefs.h ../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarModemConfigApp.cpp

DStarModemConfigFrame.o:	DStarModemConfigFrame.cpp DStarModemConfigFrame.h DStarModemConfigDefs.h DStarModemConfigApp.h ../Common/DStarModemConfig.h \
					../Common/DStarDefines.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarModemConfigFrame.cpp

install:	all
				install -g bin -o root -m 0775 dstarmodemconfig $(BINDIR)

clean:
				rm -f core dstarmodemconfig *.o *~ *.bak
