all:			dummyrepeater

CFLAGS :=		-I../Common $(CFLAGS)

dummyrepeater:	DummyRepeaterApp.o DummyRepeaterBleeper.o DummyRepeaterBleepSet.o DummyRepeaterCallsignSet.o DummyRepeaterControllerSet.o \
				DummyRepeaterDVDongleSet.o DummyRepeaterDVDongleThread.o DummyRepeaterErrorEvent.o DummyRepeaterFrame.o DummyRepeaterHeaderEvent.o \
				DummyRepeaterMessageData.o DummyRepeaterMessageEvent.o DummyRepeaterMessageSet.o DummyRepeaterPreferences.o \
				DummyRepeaterSlowDataDecoder.o DummyRepeaterSlowDataEncoder.o DummyRepeaterSlowDataEvent.o DummyRepeaterSoundcardSet.o \
				DummyRepeaterStatusEvent.o DummyRepeaterThread.o DummyRepeaterTimeoutSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dummyrepeater DummyRepeaterApp.o DummyRepeaterBleeper.o DummyRepeaterBleepSet.o DummyRepeaterCallsignSet.o \
				DummyRepeaterControllerSet.o DummyRepeaterDVDongleSet.o DummyRepeaterDVDongleThread.o DummyRepeaterErrorEvent.o DummyRepeaterFrame.o \
				DummyRepeaterHeaderEvent.o DummyRepeaterMessageData.o DummyRepeaterMessageEvent.o DummyRepeaterMessageSet.o DummyRepeaterPreferences.o \
				DummyRepeaterSlowDataDecoder.o DummyRepeaterSlowDataEncoder.o DummyRepeaterSlowDataEvent.o DummyRepeaterSoundcardSet.o \
				DummyRepeaterStatusEvent.o DummyRepeaterThread.o DummyRepeaterTimeoutSet.o ../Common/Common.a $(LIBS)

DummyRepeaterApp.o:	DummyRepeaterApp.cpp DummyRepeaterApp.h DummyRepeaterThread.h DummyRepeaterFrame.h DummyRepeaterDefs.h DummyRepeaterMessageData.h \
					DummyRepeaterDVDongleSet.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/HeaderData.h ../Common/RepeaterProtocolHandler.h \
					../Common/SoundCardReaderWriter.h ../Common/SerialDataController.h ../Common/SerialLineController.h ../Common/K8055Controller.h \
					../Common/URIUSBController.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/DVDongleController.h \
					../Common/RaspberryController.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DummyRepeaterApp.cpp

DummyRepeaterBleeper.o:	DummyRepeaterBleeper.cpp DummyRepeaterBleeper.h
				$(CC) $(CFLAGS) -c DummyRepeaterBleeper.cpp

DummyRepeaterBleepSet.o:	DummyRepeaterBleepSet.cpp DummyRepeaterBleepSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterBleepSet.cpp

DummyRepeaterCallsignSet.o:	DummyRepeaterCallsignSet.cpp DummyRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h
				$(CC) $(CFLAGS) -c DummyRepeaterCallsignSet.cpp

DummyRepeaterControllerSet.o:	DummyRepeaterControllerSet.cpp DummyRepeaterControllerSet.h ../Common/SerialLineController.h
				$(CC) $(CFLAGS) -c DummyRepeaterControllerSet.cpp

DummyRepeaterDVDongleSet.o:	DummyRepeaterDVDongleSet.cpp DummyRepeaterDVDongleSet.h ../Common/SerialDataController.h
				$(CC) $(CFLAGS) -c DummyRepeaterDVDongleSet.cpp

DummyRepeaterDVDongleThread.o:	DummyRepeaterDVDongleThread.cpp DummyRepeaterDVDongleThread.h DummyRepeaterBleeper.h ../Common/DStarDefines.h \
								../Common/RingBuffer.h ../Common/DVDongleController.h ../Common/FIRFilter.h
				$(CC) $(CFLAGS) -c DummyRepeaterDVDongleThread.cpp

DummyRepeaterErrorEvent.o:	DummyRepeaterErrorEvent.cpp DummyRepeaterErrorEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterErrorEvent.cpp

DummyRepeaterFrame.o:	DummyRepeaterFrame.cpp DummyRepeaterFrame.h DummyRepeaterDefs.h DummyRepeaterApp.h DummyRepeaterMessageData.h \
						DummyRepeaterMessageEvent.h DummyRepeaterErrorEvent.h DummyRepeaterSlowDataEvent.h DummyRepeaterHeaderEvent.h \
						DummyRepeaterStatusEvent.h DummyRepeaterPreferences.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h \
						../Common/HeaderData.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DummyRepeaterFrame.cpp

DummyRepeaterHeaderEvent.o:	DummyRepeaterHeaderEvent.cpp DummyRepeaterHeaderEvent.h ../Common/HeaderData.h
				$(CC) $(CFLAGS) -c DummyRepeaterHeaderEvent.cpp

DummyRepeaterMessageData.o:	DummyRepeaterMessageData.cpp DummyRepeaterMessageData.h
				$(CC) $(CFLAGS) -c DummyRepeaterMessageData.cpp

DummyRepeaterMessageEvent.o:	DummyRepeaterMessageEvent.cpp DummyRepeaterMessageEvent.h DummyRepeaterMessageData.h
				$(CC) $(CFLAGS) -c DummyRepeaterMessageEvent.cpp

DummyRepeaterMessageSet.o:	DummyRepeaterMessageSet.cpp DummyRepeaterMessageSet.h ../Common/MessageTextCtrl.h
				$(CC) $(CFLAGS) -c DummyRepeaterMessageSet.cpp

DummyRepeaterPreferences.o:	DummyRepeaterPreferences.cpp DummyRepeaterPreferences.h DummyRepeaterDefs.h DummyRepeaterSoundcardSet.h \
							DummyRepeaterCallsignSet.h DummyRepeaterControllerSet.h DummyRepeaterDVDongleSet.h DummyRepeaterMessageSet.h \
							DummyRepeaterTimeoutSet.h DummyRepeaterBleepSet.h ../Common/NetworkSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterPreferences.cpp

DummyRepeaterSlowDataDecoder.o:	DummyRepeaterSlowDataDecoder.cpp DummyRepeaterSlowDataDecoder.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DummyRepeaterSlowDataDecoder.cpp

DummyRepeaterSlowDataEncoder.o:	DummyRepeaterSlowDataEncoder.cpp DummyRepeaterSlowDataEncoder.h ../Common/HeaderData.h ../Common/DStarDefines.h\
								../Common/CCITTChecksumReverse.h
				$(CC) $(CFLAGS) -c DummyRepeaterSlowDataEncoder.cpp

DummyRepeaterSlowDataEvent.o:	DummyRepeaterSlowDataEvent.cpp DummyRepeaterSlowDataEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterSlowDataEvent.cpp

DummyRepeaterSoundcardSet.o:	DummyRepeaterSoundcardSet.cpp DummyRepeaterSoundcardSet.h ../Common/SoundCardReaderWriter.h
				$(CC) $(CFLAGS) -c DummyRepeaterSoundcardSet.cpp

DummyRepeaterStatusEvent.o:	DummyRepeaterStatusEvent.cpp DummyRepeaterStatusEvent.h
				$(CC) $(CFLAGS) -c DummyRepeaterStatusEvent.cpp

DummyRepeaterThread.o:	DummyRepeaterThread.cpp DummyRepeaterThread.h DummyRepeaterDefs.h DummyRepeaterApp.h DummyRepeaterMessageData.h \
						DummyRepeaterSlowDataDecoder.h DummyRepeaterSlowDataEncoder.h DummyRepeaterDVDongleThread.h ../Common/DStarDefines.h \
						../Common/SoundCardReaderWriter.h ../Common/DVDongleController.h ../Common/HeaderData.h ../Common/AudioCallback.h \
						../Common/RingBuffer.h ../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/ExternalController.h \
						../Common/Version.h
				$(CC) $(CFLAGS) -c DummyRepeaterThread.cpp

DummyRepeaterTimeoutSet.o:	DummyRepeaterTimeoutSet.cpp DummyRepeaterTimeoutSet.h
				$(CC) $(CFLAGS) -c DummyRepeaterTimeoutSet.cpp

install:	all
				install -g bin -o root -m 0775 dummyrepeater $(BINDIR)

clean:
				rm -f core dummyrepeater *.o *~ *.bak
