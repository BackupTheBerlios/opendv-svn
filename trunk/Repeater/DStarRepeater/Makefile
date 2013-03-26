all:			dstarrepeater dstarrepeaterd

CFLAGS :=		-I../Common $(CFLAGS)

dstarrepeater:	DStarRepeaterApp.o DStarRepeaterCallsignSet.o DStarRepeaterConfig.o DStarRepeaterControllerSet.o DStarRepeaterFrame.o DStarRepeaterLogger.o \
					DStarRepeaterModemSet.o DStarRepeaterPreferences.o DStarRepeaterStatusData.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o \
					DStarRepeaterRXThread.o DStarRepeaterTimesSet.o DStarRepeaterThreadHelper.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeater DStarRepeaterApp.o DStarRepeaterCallsignSet.o DStarRepeaterConfig.o DStarRepeaterControllerSet.o DStarRepeaterFrame.o \
					DStarRepeaterLogger.o DStarRepeaterModemSet.o DStarRepeaterPreferences.o DStarRepeaterStatusData.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o \
					DStarRepeaterTXThread.o DStarRepeaterRXThread.o DStarRepeaterTimesSet.o DStarRepeaterThreadHelper.o ../Common/Common.a $(LIBS)

dstarrepeaterd:	DStarRepeaterD.o DStarRepeaterConfig.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o DStarRepeaterRXThread.o \
					DStarRepeaterStatusData.o ../Common/Common.a
				$(CC) $(LDFLAGS) -o dstarrepeaterd DStarRepeaterD.o DStarRepeaterConfig.o DStarRepeaterTRXThread.o DStarRepeaterTXRXThread.o DStarRepeaterTXThread.o \
					DStarRepeaterRXThread.o DStarRepeaterStatusData.o ../Common/Common.a $(LIBS)

DStarRepeaterApp.o:	DStarRepeaterApp.cpp DStarRepeaterApp.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h DStarRepeaterTXThread.h \
					DStarRepeaterRXThread.h DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterStatusData.h DStarRepeaterLogger.h DStarRepeaterConfig.h \
					DStarRepeaterThreadHelper.h ../Common/DVRPTRController.h ../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h ../Common/DStarDefines.h \
					../Common/Logger.h ../Common/ExternalController.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h \
					../Common/DummyController.h ../Common/K8055Controller.h ../Common/RaspberryController.h
				$(CC) $(CFLAGS) -c DStarRepeaterApp.cpp

DStarRepeaterCallsignSet.o:	DStarRepeaterCallsignSet.cpp DStarRepeaterCallsignSet.h ../Common/CallsignTextCtrl.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterCallsignSet.cpp

DStarRepeaterConfig.o:	DStarRepeaterConfig.cpp DStarRepeaterConfig.h DStarRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterConfig.cpp

DStarRepeaterControllerSet.o:	DStarRepeaterControllerSet.cpp DStarRepeaterControllerSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterControllerSet.cpp

DStarRepeaterD.o:	DStarRepeaterD.cpp DStarRepeaterD.h DStarRepeaterThread.h DStarRepeaterTRXThread.h DStarRepeaterTXRXThread.h DStarRepeaterTXThread.h \
					DStarRepeaterRXThread.h DStarRepeaterConfig.h DStarRepeaterDefs.h ../Common/DStarDefines.h ../Common/Logger.h ../Common/DVRPTRController.h \
					../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h ../Common/RepeaterProtocolHandler.h ../Common/Version.h ../Common/CallsignList.h \
					../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h ../Common/RaspberryController.h
				$(CC) $(CFLAGS) -c DStarRepeaterD.cpp

DStarRepeaterFrame.o:	DStarRepeaterFrame.cpp DStarRepeaterFrame.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h DStarRepeaterPreferences.h \
					../Common/DStarDefines.h ../Common/LogEvent.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterFrame.cpp

DStarRepeaterLogger.o:	DStarRepeaterLogger.cpp DStarRepeaterLogger.h DStarRepeaterApp.h
				$(CC) $(CFLAGS) -c DStarRepeaterLogger.cpp

DStarRepeaterModemSet.o:	DStarRepeaterModemSet.cpp DStarRepeaterModemSet.h DStarRepeaterDefs.h ../Common/DVRPTRControllerV1.h ../Common/DVRPTRControllerV2.h
					../Common/DStarDefines.h ../Common/AddressTextCtrl.h ../Common/PortTextCtrl.h
				$(CC) $(CFLAGS) -c DStarRepeaterModemSet.cpp

DStarRepeaterPreferences.o:	DStarRepeaterPreferences.cpp DStarRepeaterPreferences.h DStarRepeaterDefs.h DStarRepeaterControllerSet.h DStarRepeaterModemSet.h \
					DStarRepeaterApp.h DStarRepeaterTimesSet.h DStarRepeaterCallsignSet.h ../Common/NetworkSet.h ../Common/BeaconSet.h ../Common/AnnouncementSet.h \
					../Common/DStarDefines.h ../Common/Control1Set.h ../Common/Control2Set.h
				$(CC) $(CFLAGS) -c DStarRepeaterPreferences.cpp

DStarRepeaterStatusData.o:	DStarRepeaterStatusData.cpp DStarRepeaterStatusData.h DStarRepeaterDefs.h ../Common/DStarDefines.h
				$(CC) $(CFLAGS) -c DStarRepeaterStatusData.cpp

DStarRepeaterTRXThread.o:	DStarRepeaterTRXThread.cpp DStarRepeaterTRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
					DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h ../Common/Timer.h \
					../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/OutputQueue.h ../Common/Version.h \
					../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h \
					../Common/BeaconUnit.h ../Common/BeaconCallback.h ../Common/AnnouncementUnit.h ../Common/AnnouncementCallback.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterTRXThread.cpp

DStarRepeaterTXRXThread.o:	DStarRepeaterTXRXThread.cpp DStarRepeaterTXRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h \
					DStarRepeaterStatusData.h ../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h ../Common/Timer.h \
					../Common/RepeaterProtocolHandler.h ../Common/SlowDataEncoder.h ../Common/SlowDataDecoder.h ../Common/OutputQueue.h ../Common/Version.h \
					../Common/DVTOOLFileWriter.h ../Common/CallsignList.h ../Common/ExternalController.h ../Common/DummyController.h ../Common/K8055Controller.h \
					../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXRXThread.cpp

DStarRepeaterTXThread.o:	DStarRepeaterTXThread.cpp DStarRepeaterTXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h \
					../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h ../Common/Timer.h ../Common/RepeaterProtocolHandler.h \
					../Common/OutputQueue.h ../Common/Version.h
				$(CC) $(CFLAGS) -c DStarRepeaterTXThread.cpp

DStarRepeaterRXThread.o:	DStarRepeaterRXThread.cpp DStarRepeaterRXThread.h DStarRepeaterThread.h DStarRepeaterDefs.h DStarRepeaterApp.h DStarRepeaterStatusData.h \
					../Common/AMBEFEC.h ../Common/DStarDefines.h ../Common/DVRPTRController.h ../Common/HeaderData.h ../Common/Timer.h ../Common/RepeaterProtocolHandler.h \
					../Common/SlowDataDecoder.h ../Common/Version.h ../Common/Utils.h
				$(CC) $(CFLAGS) -c DStarRepeaterRXThread.cpp

DStarRepeaterThreadHelper.o:	DStarRepeaterThreadHelper.cpp DStarRepeaterThreadHelper.h DStarRepeaterThread.h DStarRepeaterStatusData.h
				$(CC) $(CFLAGS) -c DStarRepeaterThreadHelper.cpp

DStarRepeaterTimesSet.o:	DStarRepeaterTimesSet.cpp DStarRepeaterTimesSet.h
				$(CC) $(CFLAGS) -c DStarRepeaterTimesSet.cpp

install:	all
				install -g bin -o root -m 0775 dvrptrrepeater $(BINDIR)
				install -g bin -o root -m 0775 dvrptrrepeaterd $(BINDIR)

clean:
				rm -f core dvrptrrepeater dvrptrrepeaterd *.o *~ *.bak
