/*
 * DUTCH*Star	A series of projects to implement a series of devices and
 *		software tools for use with the D-Star protocol as used in
 *		the Amateur Radio world.
 *
 *		Definitions for the Node Adapter API.
 *
 *		These values were taken from a myriad sources, including
 *		sample tools written by Satoshi Yasuda 7M3TJZ. We try to
 *		keep the same symbol names for compatibility reasons. The
 *		values discovered by reverse engineering (doesn't everyone
 *		have an USB Monitor tool? ;-) have been named as 'fitting'
 *		as possible.
 *
 *		Values here are assumed to be valid for firmware versions
 *		4.22 (or later) and the 5.00 series. They were checked with
 *		versions 4.48.1L and 5.09L. Extra info (Serial and License)
 *		was added for the V5.36L and up chips. Values were updated
 *		up to the "sample02" code example, which apparently has 
 *		some extra values introduced for the V6 and V7 series. Info
 *		on the V7 serial port was determined by USB tracing.
 *
 * Version:	@(#)node.h	1.1.23	2010/09/12
 *
 * Author:	Fred N. van Kempen, <fred.van.kempen@microwalt.nl>
 *
 *		Copyright 2009,2010 MicroWalt Corporation
 *		All Rights Reserved.
 *
 *		This  program  or  documentation  contains  proprietary
 *		confidential information and trade secrets of MicroWalt
 *		Corporation.  Reverse  engineering of  object  code  is
 *		prohibited.  Use of copyright  notice is  precautionary
 *		and does not imply publication.  Any  unauthorized use,
 *		reproduction  or transfer  of this program  is strictly
 *		prohibited.
 *
 *		RESTRICTED RIGHTS NOTICE
 *
 *		Use, duplication, or disclosure  by the U.S. Government
 *		is subject to restrictions as set  forth in subdivision
 *		(b)(3)(ii) of the Rights in Technical Data and Computer
 *		Software clause at 252.227-7013.
 *
 *		MicroWalt Corporation
 *		P O BOX 8
 *		1400AA, BUSSUM, NH
 *		THE NETHERLANDS
 *		PH:  +31 (35) 7503090
 *		FAX: +31 (35) 7503091
 */
#ifndef NODE_H
# define NODE_H


/* Node Adapter Service events. */
#define NODE_EVT_STATUS	1			/* status update event */

/* Various. */
#define NODE_PID	"NODE_PID"		/* env variable name for ID */

/* Firmware Family info (4 bits). */
#define HA_UNKNOWN	0			/* known firmware type */
#define HA_SATOSHI	1			/* Satoshi series */
#define HA_DUTCHSTAR	2			/* DUTCH*Star series */

/* Product families (8 bits). */
#define HA_NODE		1			/* NODE family of devices */
#define HA_DVAD		2			/* DVad family of devices */
#define HA_HSA		3			/* HSA family of devices */

/* Device version/release info (4 bits). */
#define HA_FWREL0	0			/* Rel.0 firmware */
#define HA_FWREL1	1			/* Rel.1 firmware */
#define HA_FWREL2	1			/* Rel.2 firmware */
#define HA_FWREL3	3			/* Rel.3 firmware */
#define HA_FWREL4	4			/* Rel.4 firmware */
#define HA_FWREL5	5			/* Rel.5 firmware */
#define HA_FWREL6	6			/* Rel.6 firmware */
#define HA_FWREL7	7			/* Rel.7 firmware */
#define HA_FWREL8	8			/* Rel.8 firmware */
#define HA_FWREL9	9			/* Rel.9 firmware */
#define HA_FWREL10	10			/* Rel.10 firmware */
#define HA_FWREL11	11			/* Rel.11 firmware */
#define HA_FWREL12	12			/* Rel.12 firmware */
#define HA_FWREL13	13			/* Rel.13 firmware */
#define HA_FWREL14	14			/* Rel.14 firmware */
#define HA_FWREL15	15			/* Rel.15 firmware */

#define HA_VENDOR	12
# define HA_ISVENDOR(x)	(((x) >> HA_VENDOR) & 0x0f)
#define HA_DEVICE	4
# define HA_ISDEVICE(x)	(((x) >> HA_DEVICE) & 0xff)
#define HA_RELEASE	0
# define HA_ISRELEASE(x)	(((x) >> HA_RELEASE) & 0x0f)


/* USB info. */
#define USB_VENDID		0x04D8	/* USB Vendor ID: Microchip Inc. */
#define USB_NAID		0x0300	/* USB PID: NA Main */
#define USB_NAID_HID		0x0301	/* USB PID: NA Main (HID mode) */
#define USB_HSAID		0x0302	/* USB PID: HSA */
#define USB_HSABID		0x0303	/* USB PID: HSA (BOOT) */
#define USB_OPTID		0x0004	/* USB PID: NA Option */
#define USB_DVID		0x0310	/* USB PID: DV Adapter */
#define USB_NABID		0x03ff	/* USB PID: NA ID (BOOT) */

/* USB parameters. */
#define USB_MAXPKT		8	/* 8 for USB1, 64 for USB2 */

/* API commands. */
#define	SET_AD_INIT		0x00	/* initialize device */
#define	SET_TimeOut		0x01	/* set TimeOut value in sec */
#define	SET_DelayTime		0x02	/* set DelayTime value in msec */
#define	SET_KeepAlive		0x03	/* set KeepAlive value in msec */
#define	SET_RESYNC_ERROR_BITS	0x04	/* set ResyncErrorBits value */
#define	SET_PTT			0x05	/* raise or drop PTT */
#define	SET_COS			0x06	/* enable (Hardware) COS checking */
#define	SET_CRC_CHECK		0x07	/* enable CRC checking */
#define	SET_RAW_OUTPUT		0x08	/* set RAW OUTPUT mode */
#define SET_RSSI		0x09	/* enable RSSI reading (HSA) */
#define	SET_LastFrame		0x0a	/* enable LastFrameSent mode */
#define	SET_SD_CONV		0x0b	/* enable Slow Data Conversion */
#define SET_DIAG		0x0c	/* enable DIAG mode (HSA) */
#define	SET_DEBUG		0x0d	/* enable DEBUG mode (bitmask) */
# define DEBUG_FL_RX		 0x01	/*  bit 0: do RX debugging */
# define DEBUG_FL_TX		 0x02	/*  bit 1: do TX debugging */
# define DEBUG_FL_SD		 0x04	/*  bit 2: do SlowData debugging */
# define DEBUG_FL_CAL		 0x40	/*  bit 6: start TX CALIBRATE (HSA) */
#define	SET_HalfFull		0x0e	/* set HALF of FULL duplex mode */
#define	SET_AD_RESET		0x0f	/* reset the device */
# define AD_RESET_NORMAL	 0x00	/*  reset the device state */
# define AD_RESET_REBOOT	 0xae	/*  reboot the device (HSA) */
# define AD_RESET_PROG		 0xea	/*  reboot to PROG mode (HSA) */
# define AD_RESET_EEPROM	 0xfa	/*  reset the EEPROM+reboot (HSA) */
#define	PUT_DATA		0x10	/* send RF Data to device */
#define	GET_DATA		0x11	/* read RF Data from device */
#define PUT_HEADER		0x20	/* write RF Header to device (HSA) */
#define	GET_HEADER		0x21	/* read RF Header from device */
#define SET_DVAD_UNL1		0x28	/* DVad V3.03+: Unlock Callsign (8by) */
#define SET_DVAD_UNL2		0x29	/* DVad V3.03+: Unlock Callsign (4by) */
#define	GET_AD_STATUS		0x30	/* get current status */
# define READ_RF_HEADER		 0x01	/*  bit0: reading RF HEADER */
# define COS_OnOff 		 0x02	/*  bit1: COS detected */
# define CRC_ERROR 		 0x04	/*  bit2: CRC error on hdr */
# define LastFrameRead		 0x08	/*  bit3: LastFrame rec'd */
# define HeaderDecodeDone	 0x10	/*  bit4: Compress Done */
# define PTT_OnOff		 0x20	/*  bit5: PTT active */
# define RF_HEADER_AVAIL	 0x40	/*  bit6: RF HEADER available (HSA) */
# define RF_HS_DETECT		 0x80	/*  bit7: HotSpot bug detected (HSA) */
#define	SET_MyCALL		0x40	/* set D-STAR TXhdr MyCall */
#define	SET_MyCALL2		0x41	/* set D-STAR TXhdr MyCall2 */
#define	SET_YourCALL		0x42	/* set D-STAR TXhdr UrCall */
#define	SET_RPT1CALL		0x43	/* set D-STAR TXhdr RPT1 */
#define	SET_RPT2CALL		0x44	/* set D-STAR TXhdr RPT2 */
#define	SET_FLAGS		0x45	/* set D-STAR TXhdr flags */
#define	SET_MyRPTCALL		0x46	/* set Repeater Callsign */
#define	SET_TXCALL		0x47	/* ?? OLD, depricated? */
#define SET_UNLOCK1		0x47	/* V5.09: unlock callsign (8by) */
#define SET_UNLOCK2		0x48	/* V5.09: unlock callsign (4by) */
#define SET_SyncReset		0x49	/* V7: set ResetResync value */
#define SET_JitterBufferSize	0x4a	/* V7: set JitterBuffer size */
#define SET_BeaconTimer		0x4b	/* V7: set BeaconTimer value */
#define SET_RogerTimer		0x4c	/* HSA: set RogerID tmr value */
#define GET_RemainSpace		0x50	/* get amount of TX buffer space */
#define	GET_TimeOut		0x51	/* get TimeOut value */
#define	GET_DelayTime		0x52	/* get DelayTime value */
#define	GET_KeepAlive		0x53	/* get KeepAlive value */
#define	GET_RESYNC_ERROR_BITS	0x54	/* get ResyncErrorBits value */
#define	GET_MyRPTCALL		0x55	/* get Repeater Callsign */
#define	GET_MODE		0x56	/* retrieve current mode flags */
# define RAW_SW			 0x01	/*  bit0: RAW OUTPUT */
# define CRC_SW			 0x02	/*  bit1: Check CRC */
# define COS_SW			 0x04	/*  bit2: Check COS */
# define LastFrame_SW		 0x08	/*  bit3: Last Frame Send */
# define SD_CONV_SW		 0x10	/*  bit4: Slow Data Conversion */
# define RSSI_SW		 0x40	/*  bit6: Enable RSSI Report (HSA) */
# define Duplex_SW		 0x80	/*  bit7: Half Duplex */
#define	GET_TXCALL		0x57	/* ?? OLD, depricated? */
#define GET_SyncReset		0x57	/* V7: get SyncResetValue (1by) */
#define GET_JitterBufferSize	0x58	/* V7: get JitterBuffer size (1by) */
#define GET_BeaconTimer		0x59	/* V7: get BeaconTimer value (1by) */
#define GET_RogerTimer		0x5a	/* HSA: get RogerID tmr value (1by) */
#define GET_DVAD_MYCALL		0x60	/* DVad: GET MYCALL (8by) */
#define GET_DVAD_MYCALL2	0x61	/* DVad: GET MYCALL2 (4by) */
#define GET_DVAD_URCALL		0x62	/* DVad: GET (8by) */
#define GET_DVAD_RPT1		0x63	/* DVad: GET (8by) */
#define GET_DVAD_RPT2		0x64	/* DVad: GET (8by) */
#define GET_DVAD_MODE		0x65	/* DVad: GET Mode (3by) */
#define GET_DVAD_MSG		0x66	/* DVad: GET (20by) */

#define	GET_SNVAL		0x70	/* V5: get RSSI S/N (2by) */
#define SET_SQLVL		0x71	/* V5: set SQ thres level (2by) */
#define GET_SQLVL		0x72	/* V5: get SQ thres level (2by) */

#define	SET_DVAD_Volume		0x81	/* DVad: get current volume level */
#define	GET_DVAD_Volume		0x82	/* DVad: set volume level */

#define	SET_RepCallCheck	0x90	/* RPT: set RepCallCheck flag */
#define	SET_RepDupCheck		0x91	/* RPT: set DUPcheck flag */
#define	SET_RepDupClear		0x92	/* RPT: set DUPclear flag */
#define	SET_ReplaceAltMyCall	0x93	/* RPT: set ReplaceAltMyCall */
#define	SET_ReplaceAltMyCall2	0x94	/* RPT: set ReplaceAltMyCall2 */
#define	SET_ReplaceAltYourCall	0x95	/* RPT: set ReplaceAltYourCall */
#define	SET_ReplaceAltRPT1Call	0x96	/* RPT: set ReplaceAltRPT1Call */
#define	SET_ReplaceAltRPT2Call	0x97	/* RPT: set ReplaceAltRPT2Call */
#define SET_AltFlags		0x98	/* RPT: set flags for TX */
#define SET_ReplaceFlags	0x99	/* RPT: set ReplaceFlags */
#define	SET_AltMyCall		0xa0	/* RPT: set my callsign */
#define	SET_AltMyCall2		0xa1	/* RPT: set my callsign extra info */
#define	SET_AltYourCall		0xa2	/* RPT: set your callsign */
#define	SET_AltRPT1Call		0xa3	/* RPT: set RPT1 callsign */
#define	SET_AltRPT2Call		0xa4	/* RPT: set RPT2 callsign */
#define	GET_AltMyCall		0xa8	/* RPT: get my callsign */
#define	GET_AltMyCall2		0xa9	/* RPT: get my callsign extra info */
#define	GET_AltYourCall		0xaa	/* RPT: get your callsign */
#define	GET_AltRPT1Call		0xab	/* RPT: get RPT1 callsign */
#define	GET_AltRPT2Call		0xac	/* RPT: get RPT2 callsign */
#define SET_RepeaterMode	0xae	/* RPT: set Reaper Mode (1by) (HSA) */
#define	GET_RepeaterMode	0xaf	/* RPT: get mode (3by) */
# define Mode_RAW		 0x01	/*  Bit 0: RAW Output */
# define Mode_CRC		 0x02	/*  Bit 1: CRC Check */
# define Mode_COS		 0x04	/*  Bit 2: COS Check */
# define Mode_LAST		 0x08	/*  Bit 3: LastFrameSend */
# define Mode_SLOW		 0x10	/*  Bit 4: SlowDataConv */
# define Mode_RSSI		 0x40	/*  Bit 6: RSSI Report (HSA) */
# define Mode_HALF		 0x80	/*  Bit 7: HalfDuplex */

#define SET_SERPRT_SPEED	0xb0	// V7: set Serial Port speed
#define GET_SERPRT_SPEED	0xb1	// V7: get Serial Port speed
# define SP_SPEED_1200		 0x0c	//  speed is 1200 bps
# define SP_SPEED_4800		 0x30	//  speed is 4800 bps
# define SP_SPEED_9600		 0x60	//  speed is 9600 bps
# define SP_SPEED_19200		 0xc0	//  speed is 19200 bps
#define SET_SERPRT_MODE		0xb2	// V7: set Serial Port mode
#define GET_SERPRT_MODE		0xb3	// V7: get Serial Port mode
# define SP_MODE_CMD		 0x01	//  Command Line mode
# define SP_MODE_GPSRX		 0x02	//  GPS Receive mode
# define SP_MODE_GPSTX		 0x04	//  GPS Transmit mode
#define SET_GPSMSG_TYP		0xb4	// V7: set GPS record types
#define GET_GPSMSG_TYP		0xb5	// V7: get GPS record types
# define SP_MSGTYP_RMC		 0x01
# define SP_MSGTYP_CGA		 0x02
# define SP_MSGTYP_GLL		 0x04
# define SP_MSGTYP_GSA		 0x08
# define SP_MSGTYP_VTG		 0x10
# define SP_GPSMSG_SEND		 0x80	// bit 7: Send GPS Message Text
#define SET_GPSMSG_TXT		0xb6	// V7: set GPS Message Text (29by)
#define GET_GPSMSG_TXT		0xb7 	// V7: get GPS Message Text (29by)
# define SP_GPSMSG_MAX		 29
#define SET_GPSMSG_RESET	0xb8	// V7: GPS Message Reset
#define GET_Polarity		0xd0	/* get Polarity value */
# define InvertTX		 0x01	/*  bit0: enb TX Invert */
# define InvertRX		 0x02	/*  bit1: enb RX Invert */
# define AutoPolarity		 0x80	/*  bit7: enb AutoPolarity */
#define SET_RXinvert		0xd1	/* set RXinvert value */
#define SET_TXinvert		0xd2	/* set TXinvert value */
#define SET_AutoPolarity	0xd3	/* set AutoPolarity value */
#define SET_PreBitPattern	0xe0	/* set PreBitPattern value */
#define SET_PostBitPattern	0xe1	/* set PostBitPattern value */
#define GET_PreBitPattern	0xe2	/* get PreBitPattern value */
#define GET_PostBitPattern	0xe3	/* get PostBitPattern value */
#define SET_SlowDataInit	0xe4	/* set SlowData mode */
#define SET_SlowDataInsert	0xe5	/* set SlowDataInsert value */
#define SET_SlowDataMsg		0xe6	/* set SlowDataMessage (20by) */
#define SET_SlowDataInsertA	0xe7	/* set SlowDataInsertA(fter) value */
#define SET_DigitalCodeInsert	0xe8	/* set DigitalCodeInsert value */
#define SET_DigitalCode		0xe9	/* set DigitalCode (1by) */
#define GET_DigitalCode		0xea	/* get DigitalCode (1by) */
#define GET_SlowDataInit	0xeb	/* get SlowData mode (1by) */
#define GET_SlowDataMsg		0xec	/* get SlowDataMessage (20by) */
#define SET_BeaconMessage	0xed	/* set BeaconMessage text (20by) */
#define GET_BeaconMessageInit	0xee	/* get BeaconMessage mode */
#define GET_BeaconMessage	0xef	/* get BeaconMessage text (20by) */
#define GET_EEDATA		0xf0	/* read EEPROM data (HSA) */
#define PUT_EEDATA		0xf1	/* write EEPROM data (HSA) */
#define SET_ProductID		0xf9	/* V6.x: set Product ID */
#define SET_Update		0xfc	/* V6.x: set to Update mode */
#define GET_Serial		0xfd	/* get serial number string(8by) */
#define GET_License		0xfe	/* get licensee callsign(8by) */
#define	GET_Version		0xff	/* get device version ID */

/*
 * The Repeater Mode is a 3-byte data field.
 *
 *  Byte0: Check Flags
 *   bit0: Call Check
 *   bit1: DUP Check
 *   bit2: DUP Clear
 *   bit3: Replace AltMyCall
 *   bit4: Replace AltMyCall2
 *   bit5: Replace AltYourCall
 *   bit6: Replace AltRPT1Call
 *   bit7: Replace AltRPT2Call
 *
 *  Byte1: SlowData flags
 *   bit0: Replace Flags
 *   bit1: BeforeSlowDataInsert
 *   bit2: AfterSlowDataInsert 
 *   bit3: DigitalCodeInsert
 *   bit7: Enable Repeater (HSA)
 *
 *  Byte2: D-STAR TX Flags
 */
#define Rep_CallCheck		0x01	/* RPT Call Check */
#define Rep_DupCheck		0x02	/* RPT DUP Check */
#define Rep_DupClear		0x04	/* RPT DUP Clear */
#define	Rep_AltMyCall		0x08	/* ReplaceAltMyCall */
#define	Rep_AltMyCall2		0x10	/* ReplaceAltMyCall2 */
#define	Rep_AltYourCall		0x20	/* ReplaceAltYourCall */
#define	Rep_AltRPT1Call		0x40	/* ReplaceAltRPT1Call */
#define	Rep_AltRPT2Call		0x80	/* ReplaceAltRPT2Call */

#define Rep_Flags		0x01	/* ReplaceFlags */
#define SlowDataInsert		0x02
#define SlowDataInsertA		0x04
#define DigitalCodeInsert	0x08
#define RepeaterActive		0x40	/* HSA: Standalone Repeater active */
#define RepeaterBoot		0x80	/* HSA: enable Standalone Repeater */


/* Functions. */
#ifdef __cplusplus
extern "C" {
#endif

/* This handles our DLL interface, which required STDCALL calling. */
#ifndef NODEAPI
# ifdef WIN32
#  define NODEAPI	__stdcall
# else
#  define NODEAPI	/*nothing*/
# endif
#endif


/* Core API functions. */
extern int	NODEAPI	node_init(int, int);
extern void	NODEAPI	node_reset(int);
extern void	NODEAPI	node_close(void);
extern int	NODEAPI	node_io(int, int, int, unsigned char *, int);
extern int	NODEAPI node_geterror(char *, int);
extern void	NODEAPI	node_sleep(long);
extern void	NODEAPI node_setvid(int);

/* Extended functions. */
extern int	NODEAPI	node_version(char *, int);
extern int	NODEAPI node_haprint(int, char *);
extern int	NODEAPI node_hatype(char *);
extern int	NODEAPI	node_status(void);
extern int	NODEAPI	node_txspace(void);
extern int	NODEAPI	node_getval(int);
extern int	NODEAPI	node_setval(int, int);
extern int	NODEAPI	node_getstr(int, unsigned char *, int);
extern int	NODEAPI	node_setstr(int, unsigned char *, int);

/* Utility functions. */
extern int	NODEAPI	node_setptt(int);
extern int	NODEAPI	node_getsn(void);
extern int	NODEAPI	node_getsqlvl(void);
extern int	NODEAPI	node_setsqlvl(int);
extern int	NODEAPI	node_getlicense(char *);
extern int	NODEAPI	node_getserial(char *);
extern int	NODEAPI	node_gethdr(unsigned char *, int);
extern int	NODEAPI	node_puthdr(unsigned char *, int);
extern int	NODEAPI	node_getdta(unsigned char *, int);
extern int	NODEAPI	node_putdta(unsigned char *, int);
extern int	NODEAPI	node_playframe(unsigned char *, int);
extern int	NODEAPI	node_playfile(char *, int);
extern int	NODEAPI	node_playsilence(int);
extern int	NODEAPI	node_geteeprom(unsigned char *, int);
extern int	NODEAPI	node_puteeprom(unsigned char *, int);


/* Functions which will soon be removed. */
extern void	NODEAPI	node_setcall(int, char *);
extern int	NODEAPI	node_txsetup(void);

/* Node Adapter Service functions. */
extern int	node_svcinit(int);
extern void	node_svcshut(void);
extern void	node_event(int, char *);
extern int	node_send(unsigned char *, int);
extern int	node_xmit(unsigned char *, int);


#ifdef __cplusplus
}
#endif

#endif	/*NODE_H*/
