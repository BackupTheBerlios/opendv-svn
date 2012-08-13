all:			DStarClient

CFLAGS :=		-I../Common $(CFLAGS)

OBJS :=			DStarClientApp.o DStarClientCallsignSet.o DStarClientControllerSet.o DStarClientFrame.o DStarClientPreferences.o DStarClientThread.o

DStarClient:	$(OBJS)
				$(CC) $(LDFLAGS) -o DStarClient $(OBJS) ../Common/Common.a $(LIBS)

DStarClientApp.o:	DStarClientApp.cpp DStarClientApp.h DStarClientThread.h DStarClientFrame.h DStarClientDefs.h ../Common/SerialDataController.h \
				../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/MessageData.h ../Common/DVDongleSet.h ../Common/HeaderData.h \
				../Common/BusyData.h ../Common/Logger.h ../Common/Version.h ../Common/ExternalController.h ../Common/URIUSBController.h \
				../Common/K8055Controller.h ../Common/DummyController.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c DStarClientApp.cpp

DStarClientCallsignSet.o:	DStarClientCallsignSet.cpp DStarClientCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DStarClientCallsignSet.cpp

DStarClientControllerSet.o:	DStarClientControllerSet.cpp DStarClientControllerSet.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c DStarClientControllerSet.cpp

DStarClientFrame.o:	DStarClientFrame.cpp DStarClientFrame.h DStarClientPreferences.h DStarClientDefs.h DStarClientApp.h DStarClient.xpm \
				../Common/CallsignTextCtrl.h ../Common/WAVFileReader.h ../Common/MessageEvent.h ../Common/DStarDefines.h ../Common/HeaderEvent.h \
				../Common/MessageData.h ../Common/ErrorEvent.h ../Common/HeaderData.h ../Common/BusyEvent.h ../Common/BusyData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarClientFrame.cpp

DStarClientPreferences.o:	DStarClientPreferences.cpp DStarClientPreferences.h DStarClientCallsignSet.h DStarClientControllerSet.h DStarClientDefs.h ../Common/SoundcardSet.h \
				../Common/InversionSet.h ../Common/DVDongleSet.h ../Common/MessageSet.h  ../Common/BleepSet.h
				$(CC) $(CFLAGS) -c DStarClientPreferences.cpp

DStarClientThread.o:	DStarClientThread.cpp DStarClientThread.h DStarClientDefs.h DStarClientApp.h ../Common/DStarDefines.h \
				../Common/SoundCardReaderWriter.h ../Common/LimitedPatternMatcher.h ../Common/LimitedLengthBuffer.h ../Common/DVDongleController.h \
				../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h ../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h \
				../Common/DVTOOLFileWriter.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/PatternMatcher.h \
				../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/WAVFileWriter.h ../Common/MessageData.h ../Common/RingBuffer.h \
				../Common/HeaderData.h ../Common/BusyData.h ../Common/Bleeper.h ../Common/DVDongleThread.h ../Common/ExternalController.h \
				../Common/PTTDelay.h
				$(CC) $(CFLAGS) -c DStarClientThread.cpp

install:		DStarClient
				install -g bin -o root -m 0775 DStarClient $(BINDIR)

clean:
				rm -f core DStarClient *.o *~ *.bak
