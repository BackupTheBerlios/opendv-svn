all:		Common.a

OBJS :=	AddressTextCtrl.o ArduinoController.o Bleeper.o CallsignTextCtrl.o CCITTChecksumReverse.o DummyController.o DVDongleController.o DVDongleThread.o \
		DVTOOLFileReader.o DVTOOLFileWriter.o ExternalController.o FIRFilter.o Goertzel.o HardwareController.o HeaderData.o K8055Controller.o Logger.o MessageData.o \
		MessageTextCtrl.o PortTextCtrl.o RaspberryController.o RepeaterProtocolHandler.o RestrictedTextCtrl.o SerialDataController.o SerialLineController.o \
		SlowDataDecoder.o SlowDataEncoder.o SoundCardReaderWriter.o Timer.o UDPReaderWriter.o URIUSBController.o Utils.o WAVFileReader.o WAVFileWriter.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

ArduinoController.o:	ArduinoController.cpp ArduinoController.h SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c ArduinoController.cpp

Bleeper.o:	Bleeper.cpp Bleeper.h
		$(CC) $(CFLAGS) -c Bleeper.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

DummyController.o:	DummyController.cpp DummyController.h HardwareController.h
		$(CC) $(CFLAGS) -c DummyController.cpp

DVDongleController.o:	DVDongleController.cpp DVDongleController.h DStarDefines.h SerialDataController.h
		$(CC) $(CFLAGS) -c DVDongleController.cpp

DVDongleThread.o:	DVDongleThread.cpp DVDongleThread.h Bleeper.h DStarDefines.h RingBuffer.h DVDongleController.h FIRFilter.h
		$(CC) $(CFLAGS) -c DVDongleThread.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h HeaderData.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

ExternalController.o:	ExternalController.cpp ExternalController.h DStarDefines.h HardwareController.h
		$(CC) $(CFLAGS) -c ExternalController.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

Goertzel.o:	Goertzel.cpp Goertzel.h Utils.h
		$(CC) $(CFLAGS) -c Goertzel.cpp

HardwareController.o:	HardwareController.cpp HardwareController.h
		$(CC) $(CFLAGS) -c HardwareController.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

K8055Controller.o:	K8055Controller.cpp K8055Controller.h HardwareController.h
		$(CC) $(CFLAGS) -c K8055Controller.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

MessageData.o:	MessageData.cpp MessageData.h
		$(CC) $(CFLAGS) -c MessageData.cpp

MessageTextCtrl.o:	MessageTextCtrl.cpp MessageTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c MessageTextCtrl.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

RaspberryController.o:	RaspberryController.cpp RaspberryController.h HardwareController.h
		$(CC) $(CFLAGS) -c RaspberryController.cpp

RepeaterProtocolHandler.o:	RepeaterProtocolHandler.cpp RepeaterProtocolHandler.h DStarDefines.h HeaderData.h UDPReaderWriter.h CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c RepeaterProtocolHandler.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialDataController.o:	SerialDataController.cpp SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialDataController.cpp

SerialLineController.o:	SerialLineController.cpp SerialLineController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialLineController.cpp

SlowDataDecoder.o:	SlowDataDecoder.cpp SlowDataDecoder.h DStarDefines.h
		$(CC) $(CFLAGS) -c SlowDataDecoder.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

SoundCardReaderWriter.o:	SoundCardReaderWriter.cpp SoundCardReaderWriter.h AudioCallback.h
		$(CC) $(CFLAGS) -c SoundCardReaderWriter.cpp

Timer.o:	Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

URIUSBController.o:	URIUSBController.cpp URIUSBController.h HardwareController.h
		$(CC) $(CFLAGS) -c URIUSBController.cpp

Utils.o:	Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

WAVFileReader.o:	WAVFileReader.cpp WAVFileReader.h
		$(CC) $(CFLAGS) -c WAVFileReader.cpp

WAVFileWriter.o:	WAVFileWriter.cpp WAVFileWriter.h
		$(CC) $(CFLAGS) -c WAVFileWriter.cpp

clean:
		rm -f core Common.a *.o *~ *.bak
