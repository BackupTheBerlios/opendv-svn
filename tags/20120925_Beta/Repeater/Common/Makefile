all:		Common.a

OBJS :=	ActiveHangSet.o AddressTextCtrl.o AMBEFEC.o AudioDelay.o BeaconSet.o BeaconUnit.o CallsignList.o CallsignTextCtrl.o CCITTChecksum.o \
		CCITTChecksumReverse.o Control1Set.o Control2Set.o Correlator.o CWKeyer.o CWTextCtrl.o DStarFECDecoder.o DStarFECEncoder.o DStarGMSKDemodulator.o \
		DStarGMSKModulator.o DStarInterleaver.o DStarScrambler.o DTMFController.o DTMFDecoder.o DTMFTextCtrl.o DummyController.o DVAPController.o \
		DVDongleController.o DVRPTRControllerV1.o DVRPTRControllerV2.o DVTOOLFileReader.o DVTOOLFileWriter.o ExternalController.o FirstOrderIIR.o FIRFilter.o \
		GMSKModemLibUsb.o Goertzel.o Golay.o HeaderData.o K8055Controller.o LimitedLengthBuffer.o LogEvent.o Logger.o NCO.o MessageTextCtrl.o \
		NetworkController.o NetworkSet.o OutputQueue.o PatternMatcher.o PortTextCtrl.o PTTDelay.o RadioHeaderDecoder.o RadioHeaderEncoder.o \
		RepeaterProtocolHandler.o RestrictedTextCtrl.o SerialDataController.o SerialController.o SlowDataDecoder.o SlowDataEncoder.o \
		SoundCardReaderWriter.o TimeoutTones.o Timer.o UDPReaderWriter.o URIUSBController.o Utils.o VOGAD.o WAVFileReader.o WAVFileStore.o WAVFileWriter.o

Common.a:	$(OBJS)
		ar rcs Common.a $(OBJS)

ActiveHangSet.o:	ActiveHangSet.cpp ActiveHangSet.h
		$(CC) $(CFLAGS) -c ActiveHangSet.cpp

AddressTextCtrl.o:	AddressTextCtrl.cpp AddressTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c AddressTextCtrl.cpp

AMBEFEC.o:	AMBEFEC.cpp AMBEFEC.h Golay.h
		$(CC) $(CFLAGS) -c AMBEFEC.cpp

AudioDelay.o:	AudioDelay.cpp AudioDelay.h
		$(CC) $(CFLAGS) -c AudioDelay.cpp

BeaconSet.o:	BeaconSet.cpp BeaconSet.h MessageTextCtrl.h DStarDefines.h
		$(CC) $(CFLAGS) -c BeaconSet.cpp

BeaconUnit.o:	BeaconUnit.cpp BeaconUnit.h BeaconCallback.h SlowDataEncoder.h DStarDefines.h
		$(CC) $(CFLAGS) -c BeaconUnit.cpp

CallsignList.o:	CallsignList.cpp CallsignList.h DStarDefines.h
		$(CC) $(CFLAGS) -c CallsignList.cpp

CallsignTextCtrl.o:	CallsignTextCtrl.cpp CallsignTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CallsignTextCtrl.cpp

CCITTChecksum.o:	CCITTChecksum.cpp CCITTChecksum.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksum.cpp

CCITTChecksumReverse.o:	CCITTChecksumReverse.cpp CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c CCITTChecksumReverse.cpp

Control1Set.o:	Control1Set.cpp Control1Set.h CallsignTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c Control1Set.cpp

Control2Set.o:	Control2Set.cpp Control2Set.h CallsignTextCtrl.h DStarDefines.h
				$(CC) $(CFLAGS) -c Control2Set.cpp

Correlator.o:	Correlator.cpp Correlator.h
		$(CC) $(CFLAGS) -c Correlator.cpp

CWKeyer.o:	CWKeyer.cpp CWKeyer.h FixedAudioSource.h
		$(CC) $(CFLAGS) -c CWKeyer.cpp

CWTextCtrl.o:	CWTextCtrl.cpp CWTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c CWTextCtrl.cpp

DStarFECDecoder.o:	DStarFECDecoder.cpp DStarFECDecoder.h FECDecoder.h
		$(CC) $(CFLAGS) -c DStarFECDecoder.cpp

DStarFECEncoder.o:	DStarFECEncoder.cpp DStarFECEncoder.h FECEncoder.h
		$(CC) $(CFLAGS) -c DStarFECEncoder.cpp

DStarGMSKDemodulator.o:	DStarGMSKDemodulator.cpp DStarGMSKDemodulator.h Demodulator.h FIRFilter.h DStarDefines.h
		$(CC) $(CFLAGS) -c DStarGMSKDemodulator.cpp

DStarGMSKModulator.o:	DStarGMSKModulator.cpp DStarGMSKModulator.h DStarDefines.h Modulator.h FIRFilter.h
		$(CC) $(CFLAGS) -c DStarGMSKModulator.cpp

DStarInterleaver.o:	DStarInterleaver.cpp DStarInterleaver.h DStarDefines.h Interleaver.h
		$(CC) $(CFLAGS) -c DStarInterleaver.cpp

DStarScrambler.o:	DStarScrambler.cpp DStarScrambler.h Scrambler.h
		$(CC) $(CFLAGS) -c DStarScrambler.cpp

DTMFController.o:	DTMFController.cpp DTMFController.h DTMFDecoder.h
		$(CC) $(CFLAGS) -c DTMFController.cpp

DTMFDecoder.o:	DTMFDecoder.cpp DTMFDecoder.h Goertzel.h
		$(CC) $(CFLAGS) -c DTMFDecoder.cpp

DTMFTextCtrl.o:	DTMFTextCtrl.cpp DTMFTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c DTMFTextCtrl.cpp

DummyController.o:	DummyController.cpp DummyController.h HardwareController.h
		$(CC) $(CFLAGS) -c DummyController.cpp

DVAPController.o:	DVAPController.cpp DVAPController.h SerialDataController.h HeaderData.h Utils.h RingBuffer.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c DVAPController.cpp

DVDongleController.o:	DVDongleController.cpp DVDongleController.h DStarDefines.h SerialDataController.h
		$(CC) $(CFLAGS) -c DVDongleController.cpp

DVRPTRControllerV1.o:	DVRPTRControllerV1.cpp DVRPTRControllerV1.h DVRPTRController.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h CCITTChecksum.h \
						CCITTChecksumReverse.h Timer.h
		$(CC) $(CFLAGS) -c DVRPTRControllerV1.cpp

DVRPTRControllerV2.o:	DVRPTRControllerV2.cpp DVRPTRControllerV2.h DVRPTRController.h HeaderData.h Utils.h RingBuffer.h SerialDataController.h Timer.h
		$(CC) $(CFLAGS) -c DVRPTRControllerV2.cpp

DVTOOLFileReader.o:	DVTOOLFileReader.cpp DVTOOLFileReader.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c DVTOOLFileReader.cpp

DVTOOLFileWriter.o:	DVTOOLFileWriter.cpp DVTOOLFileWriter.h HeaderData.h CCITTChecksumReverse.h DStarDefines.h
		$(CC) $(CFLAGS) -c DVTOOLFileWriter.cpp

ExternalController.o:	ExternalController.cpp ExternalController.h DStarDefines.h HardwareController.h
		$(CC) $(CFLAGS) -c ExternalController.cpp

FirstOrderIIR.o:	FirstOrderIIR.cpp FirstOrderIIR.h
		$(CC) $(CFLAGS) -c FirstOrderIIR.cpp

FIRFilter.o:	FIRFilter.cpp FIRFilter.h Filter.h
		$(CC) $(CFLAGS) -c FIRFilter.cpp

GMSKModemLibUsb.o:	GMSKModemLibUsb.cpp GMSKModemLibUsb.h GMSKModem.h HeaderData.h DStarDefines.h Utils.h
		$(CC) $(CFLAGS) -c GMSKModemLibUsb.cpp

Goertzel.o:	Goertzel.cpp Goertzel.h Utils.h
		$(CC) $(CFLAGS) -c Goertzel.cpp

Golay.o:	Golay.cpp Golay.h
		$(CC) $(CFLAGS) -c Golay.cpp

HeaderData.o:	HeaderData.cpp HeaderData.h DStarDefines.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c HeaderData.cpp

K8055Controller.o:	K8055Controller.cpp K8055Controller.h HardwareController.h
		$(CC) $(CFLAGS) -c K8055Controller.cpp

LimitedLengthBuffer.o:	LimitedLengthBuffer.cpp LimitedLengthBuffer.h
		$(CC) $(CFLAGS) -c LimitedLengthBuffer.cpp

LogEvent.o:	LogEvent.cpp LogEvent.h
		$(CC) $(CFLAGS) -c LogEvent.cpp

Logger.o:	Logger.cpp Logger.h
		$(CC) $(CFLAGS) -c Logger.cpp

MessageTextCtrl.o:	MessageTextCtrl.cpp MessageTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c MessageTextCtrl.cpp

NCO.o:		NCO.cpp NCO.h
		$(CC) $(CFLAGS) -c NCO.cpp

NetworkController.o:	NetworkController.cpp NetworkController.h SerialController.h
		$(CC) $(CFLAGS) -c NetworkController.cpp

NetworkSet.o:	NetworkSet.cpp NetworkSet.h AddressTextCtrl.h PortTextCtrl.h
		$(CC) $(CFLAGS) -c NetworkSet.cpp

OutputQueue.o:	OutputQueue.cpp OutputQueue.h RingBuffer.h HeaderData.h
		$(CC) $(CFLAGS) -c OutputQueue.cpp

PatternMatcher.o:	PatternMatcher.cpp PatternMatcher.h
		$(CC) $(CFLAGS) -c PatternMatcher.cpp

PortTextCtrl.o:	PortTextCtrl.cpp PortTextCtrl.h RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c PortTextCtrl.cpp

PTTDelay.o:	PTTDelay.cpp PTTDelay.h
		$(CC) $(CFLAGS) -c PTTDelay.cpp

RadioHeaderDecoder.o:	RadioHeaderDecoder.cpp RadioHeaderDecoder.h DStarDefines.h RadioHeaderEncoder.h DStarInterleaver.h DStarFECDecoder.h DStarScrambler.h \
						CCITTChecksumReverse.h HeaderData.h
		$(CC) $(CFLAGS) -c RadioHeaderDecoder.cpp

RadioHeaderEncoder.o:	RadioHeaderEncoder.cpp RadioHeaderEncoder.h DStarDefines.h DStarInterleaver.h DStarFECEncoder.h DStarScrambler.h CCITTChecksumReverse.h \
						HeaderData.h
		$(CC) $(CFLAGS) -c RadioHeaderEncoder.cpp

RepeaterProtocolHandler.o:	RepeaterProtocolHandler.cpp RepeaterProtocolHandler.h DStarDefines.h HeaderData.h UDPReaderWriter.h CCITTChecksumReverse.h Utils.h
		$(CC) $(CFLAGS) -c RepeaterProtocolHandler.cpp

RestrictedTextCtrl.o:	RestrictedTextCtrl.cpp RestrictedTextCtrl.h
		$(CC) $(CFLAGS) -c RestrictedTextCtrl.cpp

SerialDataController.o:	SerialDataController.cpp SerialDataController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialDataController.cpp

SerialController.o:	SerialController.cpp SerialController.h HardwareController.h
		$(CC) $(CFLAGS) -c SerialController.cpp

SlowDataDecoder.o:	SlowDataDecoder.cpp SlowDataDecoder.h DStarDefines.h HeaderData.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataDecoder.cpp

SlowDataEncoder.o:	SlowDataEncoder.cpp SlowDataEncoder.h DStarDefines.h HeaderData.h CCITTChecksumReverse.h
		$(CC) $(CFLAGS) -c SlowDataEncoder.cpp

SoundCardReaderWriter.o:	SoundCardReaderWriter.cpp SoundCardReaderWriter.h AudioCallback.h
		$(CC) $(CFLAGS) -c SoundCardReaderWriter.cpp

TimeoutTones.o:	TimeoutTones.cpp TimeoutTones.h NCO.h
		$(CC) $(CFLAGS) -c TimeoutTones.cpp

Timer.o:	Timer.cpp Timer.h
		$(CC) $(CFLAGS) -c Timer.cpp

UDPReaderWriter.o:	UDPReaderWriter.cpp UDPReaderWriter.h Utils.h
		$(CC) $(CFLAGS) -c UDPReaderWriter.cpp

URIUSBController.o:	URIUSBController.cpp URIUSBController.h HardwareController.h
		$(CC) $(CFLAGS) -c URIUSBController.cpp

Utils.o:	Utils.cpp Utils.h
		$(CC) $(CFLAGS) -c Utils.cpp

VOGAD.o:	VOGAD.cpp VOGAD.h
		$(CC) $(CFLAGS) -c VOGAD.cpp

WAVFileReader.o:	WAVFileReader.cpp WAVFileReader.h
		$(CC) $(CFLAGS) -c WAVFileReader.cpp

WAVFileStore.o:	WAVFileStore.cpp WAVFileStore.h FixedAudioSource.h WAVFileReader.h AnalogueDefines.h
		$(CC) $(CFLAGS) -c WAVFileStore.cpp

WAVFileWriter.o:	WAVFileWriter.cpp WAVFileWriter.h
		$(CC) $(CFLAGS) -c WAVFileWriter.cpp

clean:
		rm -f core Common.a *.o *~ *.bak

