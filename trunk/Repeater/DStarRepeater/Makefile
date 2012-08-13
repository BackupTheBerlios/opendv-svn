all:			dstarrepeater dstarrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeater:	DStarRepeaterApp.o DStarRepeaterCallsignSet.o DStarRepeaterConfig.o DStarRepeaterFrame.o DStarRepeaterControllerSet.o \
				DStarRepeaterLogger.o DStarRepeaterPreferences.o DStarRepeaterRadioSet.o DStarRepeaterStatusData.o DStarRepeaterTRXThread.o \
				DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterTimesSet.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeater DStarRepeaterApp.o DStarRepeaterCallsignSet.o DStarRepeaterConfig.o DStarRepeaterControllerSet.o \
				DStarRepeaterFrame.o DStarRepeaterLogger.o DStarRepeaterPreferences.o DStarRepeaterRadioSet.o DStarRepeaterStatusData.o \
				DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterTimesSet.o \
				../Common/Common.a $(LIBS)

dstarrepeaterd:	DStarRepeaterD.o DStarRepeaterConfig.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o \
				DStarRepeaterRXThread.o DStarRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterd DStarRepeaterD.o DStarRepeaterConfig.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o \
				DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterStatusData.o ../Common/Common.a $(LIBS)

DStarRepeaterApp.o:	DStarRepeaterApp.cpp DStarRepeaterApp.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h \
					DStarRepeaterTXThread.h DStarRepeaterRXThread.h DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterStatusData.h \
					DStarRepeaterLogger.h DStarRepeaterConfig.h ../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/Logger.h \
					../Common/DummyController.h ../Common/K8055Controller.h ../Common/URIUSBController.h ../Common/SerialController.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c DStarRepeaterApp.cpp

DStarRepeaterCallsignSet.o:	DStarRepeaterCallsignSet.cpp DStarRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterCallsignSet.cpp

DStarRepeaterConfig.o:	DStarRepeaterConfig.cpp DStarRepeaterConfig.h DStarRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfig.cpp

DStarRepeaterControllerSet.o:	DStarRepeaterControllerSet.cpp DStarRepeaterControllerSet.h ../Common/SerialController.h
				$(CC) $(CFLAGS) -c DStarRepeaterControllerSet.cpp

DStarRepeaterD.o:	DStarRepeaterD.cpp DStarRepeaterD.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h DStarRepeaterTXThread.h \
					DStarRepeaterRXThread.h DStarRepeaterConfig.h DStarRepeaterDefs.h ../Common/DStarDefines.h ../Common/Logger.h \
					../Common/DummyController.h ../Common/K8055Controller.h ../Common/URIUSBController.h ../Common/SerialController.h \
					../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h
				$(CC) $(CFLAGS) -c DStarRepeaterD.cpp

DStarRepeaterFrame.o:	DStarRepeaterFrame.cpp DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h \
						DStarRepeaterPreferences.h ../Common/WAVFileReader.h ../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterFrame.cpp

DStarRepeaterLogger.o:	DStarRepeaterLogger.cpp DStarRepeaterLogger.h DStarRepeaterApp.h
				$(CC) $(CFLAGS) -c DStarRepeaterLogger.cpp

DStarRepeaterPreferences.o:	DStarRepeaterPreferences.cpp DStarRepeaterPreferences.h DStarRepeaterDefs.h DStarRepeaterRadioSet.h DStarRepeaterApp.h \
							DStarRepeaterControllerSet.h DStarRepeaterTimesSet.h DStarRepeaterCallsignSet.h ../Common/NetworkSet.h \
							../Common/BeaconSet.h ../Common/DStarDefines.h ../Common/ActiveHangSet.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c DStarRepeaterPreferences.cpp

DStarRepeaterRadioSet.o:	DStarRepeaterRadioSet.cpp DStarRepeaterRadioSet.h DStarRepeaterApp.h DStarRepeaterDefs.h ../Common/SoundCardReaderWriter.h \
							../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterRadioSet.cpp

DStarRepeaterStatusData.o:	DStarRepeaterStatusData.cpp DStarRepeaterStatusData.h DStarRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterStatusData.cpp

DStarRepeaterTRXThread.o:	DStarRepeaterTRXThread.cpp DStarRepeaterTRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
							DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h \
							../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h ../Common/PatternMatcher.h ../Common/HeaderData.h \
							../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h \
							../Common/PTTDelay.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h \
							../Common/SlowDataEncoder.h ../Common/AudioDelay.h ../Common/Version.h ../Common/Goertzel.h ../Common/DVTOOLFileWriter.h \
							../Common/CallsignList.h ../Common/BeaconUnit.h ../Common/BeaconCallback.h
				$(CC) $(CFLAGS) -c DStarRepeaterTRXThread.cpp

DStarRepeaterTXRXThread.o:	DStarRepeaterTXRXThread.cpp DStarRepeaterTXRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
							DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/DStarGMSKModulator.h \
							../Common/RadioHeaderDecoder.h ../Common/RadioHeaderEncoder.h ../Common/PatternMatcher.h ../Common/HeaderData.h \
							../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h \
							../Common/PTTDelay.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h \
							../Common/SlowDataEncoder.h ../Common/AudioDelay.h ../Common/Version.h ../Common/Goertzel.h ../Common/DVTOOLFileWriter.h \
							../Common/CallsignList.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXRXThread.cpp

DStarRepeaterTXThread.o:	DStarRepeaterTXThread.cpp DStarRepeaterTXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
							DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/DStarGMSKModulator.h ../Common/RadioHeaderEncoder.h ../Common/HeaderData.h ../Common/AudioCallback.h \
							../Common/RingBuffer.h ../Common/Timer.h ../Common/ExternalController.h ../Common/PTTDelay.h ../Common/Utils.h \
							../Common/RepeaterProtocolHandler.h ../Common/AudioDelay.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXThread.cpp

DStarRepeaterRXThread.o:	DStarRepeaterRXThread.cpp DStarRepeaterRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
							DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/SoundCardReaderWriter.h \
							../Common/LimitedLengthBuffer.h ../Common/DStarGMSKDemodulator.h ../Common/RadioHeaderDecoder.h ../Common/PatternMatcher.h \
							../Common/HeaderData.h ../Common/WAVFileReader.h ../Common/AudioCallback.h ../Common/RingBuffer.h ../Common/Timer.h \
							../Common/ExternalController.h ../Common/Utils.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h \
							../Common/Goertzel.h
				$(CC) $(CFLAGS) -c DStarRepeaterRXThread.cpp

DStarRepeaterTimesSet.o:	DStarRepeaterTimesSet.cpp DStarRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dstarrepeater $(BINDIR)
				install -g bin -o root -m 0775 dstarrepeaterd $(BINDIR)

clean:
				rm -f core dstarrepeater dstarrepeaterd *.o *~ *.bak

