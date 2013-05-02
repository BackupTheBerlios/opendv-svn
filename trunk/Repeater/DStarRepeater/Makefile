all:			dstarrepeater dstarrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeater:	DStarRepeaterApp.o DStarRepeaterFrame.o DStarRepeaterLogger.o DStarRepeaterStatusData.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o \
					DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterThreadHelper.o DStarRepeaterModemDVAPController.o DStarRepeaterModemDVRPTRV1Controller.o \
					DStarRepeaterModemDVRPTRV2Controller.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeater DStarRepeaterApp.o DStarRepeaterFrame.o DStarRepeaterLogger.o DStarRepeaterStatusData.o DStarRepeaterTRXThread.o \
					DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterThreadHelper.o DStarRepeaterModemDVAPController.o \
					DStarRepeaterModemDVRPTRV1Controller.o DStarRepeaterModemDVRPTRV2Controller.o ../Common/Common.a $(LIBS)

dstarrepeaterd:	DStarRepeaterD.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterStatusData.o \
					DStarRepeaterModemDVAPController.o DStarRepeaterModemDVRPTRV1Controller.o DStarRepeaterModemDVRPTRV2Controller.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterd DStarRepeaterD.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o \
					DStarRepeaterStatusData.o DStarRepeaterModemDVAPController.o DStarRepeaterModemDVRPTRV1Controller.o DStarRepeaterModemDVRPTRV2Controller.o \
					../Common/Common.a $(LIBS)

DStarRepeaterApp.o:	DStarRepeaterApp.cpp DStarRepeaterApp.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h DStarRepeaterTXThread.h \
					DStarRepeaterRXThread.h DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterStatusData.h DStarRepeaterLogger.h DStarRepeaterThreadHelper.h \
					DStarRepeaterModemDVAPController.h DStarRepeaterModemDVRPTRV1Controller.h DStarRepeaterModemDVRPTRV2Controller.h \
					../Common/DStarDefines.h ../Common/Logger.h ../Common/ExternalController.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h \
					../Common/CallsignList.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/RaspberryController.h ../Common/DStarRepeaterConfig.h 
				$(CC) $(CFLAGS) -c DStarRepeaterApp.cpp

DStarRepeaterD.o:	DStarRepeaterD.cpp DStarRepeaterD.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h DStarRepeaterTXThread.h \
					DStarRepeaterRXThread.h DStarRepeaterDefs.h DStarRepeaterModemDVAPController.h DStarRepeaterModemDVRPTRV1Controller.h \
					DStarRepeaterModemDVRPTRV2Controller.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/RepeaterProtocolHandler.h \
					../Common/Version.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h \
					../Common/RaspberryController.h ../Common/DStarRepeaterConfig.h
				$(CC) $(CFLAGS) -c DStarRepeaterD.cpp

DStarRepeaterModemDVAPController.o:	DStarRepeaterModemDVAPController.cpp DStarRepeaterModemDVAPController.h DStarRepeaterModem.h ../Common/SerialDataController.h \
					../Common/RingBuffer.h ../Common/HeaderData.h ../Common/Utils.h ../Common/CCITTChecksumReverse.h ../Common/DStarDefines.h ../Common/Timer.h
				$(CC) $(CFLAGS) -c DStarRepeaterModemDVAPController.cpp

DStarRepeaterModemDVRPTRV1Controller.o:	DStarRepeaterModemDVRPTRV1Controller.cpp DStarRepeaterModemDVRPTRV1Controller.h DStarRepeaterModem.h \
					../Common/SerialDataController.h ../Common/RingBuffer.h ../Common/HeaderData.h ../Common/Utils.h ../Common/CCITTChecksum.h \
					../Common/CCITTChecksumReverse.h ../Common/Timer.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterModemDVRPTRV1Controller.cpp

DStarRepeaterModemDVRPTRV2Controller.o:	DStarRepeaterModemDVRPTRV2Controller.cpp DStarRepeaterModemDVRPTRV2Controller.h DStarRepeaterModem.h \
					../Common/SerialDataController.h ../Common/RingBuffer.h ../Common/HeaderData.h ../Common/Utils.h ../Common/TCPReaderWriter.h ../Common/Timer.h \
					../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterModemDVRPTRV2Controller.cpp

DStarRepeaterFrame.o:	DStarRepeaterFrame.cpp DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h ../Common/DStarDefines.h \
					../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterFrame.cpp

DStarRepeaterLogger.o:	DStarRepeaterLogger.cpp DStarRepeaterLogger.h DStarRepeaterApp.h
				$(CC) $(CFLAGS) -c DStarRepeaterLogger.cpp

DStarRepeaterStatusData.o:	DStarRepeaterStatusData.cpp DStarRepeaterStatusData.h DStarRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterStatusData.cpp

DStarRepeaterTRXThread.o:	DStarRepeaterTRXThread.cpp DStarRepeaterTRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
					DStarRepeaterStatusData.h DStarRepeaterModem.h DStarRepeaterModemDVAPController.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h \
					../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/OutputQueue.h ../Common/Version.h \
					../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h \
					../Common/BeaconUnit.h ../Common/BeaconCallback.h ../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterTRXThread.cpp

DStarRepeaterTXRXThread.o:	DStarRepeaterTXRXThread.cpp DStarRepeaterTXRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
					DStarRepeaterStatusData.h DStarRepeaterModem.h DStarRepeaterModemDVAPController.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h \
					../Common/Timer.h ../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/OutputQueue.h ../Common/Version.h \
					../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h \
					../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXRXThread.cpp

DStarRepeaterTXThread.o:	DStarRepeaterTXThread.cpp DStarRepeaterTXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h \
					DStarRepeaterModem.h DStarRepeaterModemDVAPController.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Timer.h \
					../Common/RepeaterProtocolHandler.h ../Common/OutputQueue.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXThread.cpp

DStarRepeaterRXThread.o:	DStarRepeaterRXThread.cpp DStarRepeaterRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h \
					DStarRepeaterModem.h DStarRepeaterModemDVAPController.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/HeaderData.h ../Common/Timer.h \
					../Common/RepeaterProtocolHandler.h ../Common/SlowDataDecoder.h ../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterRXThread.cpp

DStarRepeaterThreadHelper.o:	DStarRepeaterThreadHelper.cpp DStarRepeaterThreadHelper.h DStarRepeaterThread.h DStarRepeaterStatusData.h
				$(CC) $(CFLAGS) -c DStarRepeaterThreadHelper.cpp

install:	all
				install -g bin -o root -m 0775 dstarrepeater $(BINDIR)
				install -g bin -o root -m 0775 dstarrepeaterd $(BINDIR)

clean:
				rm -f core dstarrepeater dstarrepeaterd *.o *~ *.bak
