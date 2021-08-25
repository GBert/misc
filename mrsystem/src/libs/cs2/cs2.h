#ifndef CS2_H
#define CS2_H

/**
* @mainpage cs2
*
* Encoder/Decoder f&uuml;r die Datenbytes eines M&auml;rklin CAN Frames
*
* @author Michael Bernstein
*/

#include <linux/can.h>
#include <boolean.h>
#include <bitmask.h>

#ifdef __cpluplus
extern "C" {
#endif

/** @file */

/**
* Konstanten f&uuml;r Anzahl Datenbytes in CAN Frame
*/
#define CS2_NUM_CAN_BYTES 8

/**
* Konstanten f&uuml;er Auswertung CS2 CAN Id
*/
#define CS2_SHIFT_HASH     0
#define CS2_SHIFT_RESP    16
#define CS2_SHIFT_COMMAND 17
#define CS2_SHIFT_PRIO    25

#define CS2_MASK_HASH    BIT_MASK( 0, 15)
#define CS2_MASK_RESP    BIT_MASK(16, 16)
#define CS2_MASK_COMMAND BIT_MASK(17, 24)
#define CS2_MASK_PRIO    BIT_MASK(25, 29)

#define CS2_SHIFT_HASH_MSB  3
#define CS2_MASK_HASH_MAGIC BIT_MASK(7,9)
#define CS2_HASH_MAGIC      BIT_MASK(8,9)
#define CS2_MASK_HASH_LOW_BYTE  BIT_MASK(0, 7)
#define CS2_MASK_HASH_HIGH_BYTE BIT_MASK(7,15)

/**
* Konstanten f&uuml;r Folgenummer
*/
#define CS2_FOLGENUMMER_LOW  BIT_MASK( 0, 6)
#define CS2_FOLGENUMMER_HIGH BIT_MASK(10,15)

/**
* Konstanten f&uuml;r das prio Feld
*/
#ifdef CS2_USE_PRIO
#define CS2_PRIO_0 0 /* not prio set */
#define CS2_PRIO_1 1 /* Stopp / Go / Kurzschluss-Meldungen */
#define CS2_PRIO_2 2 /* Rueckmeldungen */
#define CS2_PRIO_3 3 /* Lok anhalten */
#define CS2_PRIO_4 4 /* Lok / Zubehoerbefehle */
#else
#define CS2_PRIO_0 0 /* not prio set */
#define CS2_PRIO_1 0 /* Stopp / Go / Kurzschluss-Meldungen */
#define CS2_PRIO_2 0 /* Rueckmeldungen */
#define CS2_PRIO_3 0 /* Lok anhalten */
#define CS2_PRIO_4 0 /* Lok / Zubehoerbefehle */
#endif

/**
* Konstanten f&uuml;r CAN Kommandos (Command Feld der CS2 can id)
*/
#define CS2_CMD_SYSTEM        0x00
#define CS2_CMD_DISCOVERY     0x01
#define CS2_CMD_BIND          0x02
#define CS2_CMD_MFX_VERIFY    0x03
#define CS2_CMD_SPEED         0x04
#define CS2_CMD_DIRECTION     0x05
#define CS2_CMD_FUNCTION      0x06
#define CS2_CMD_READ_CONFIG   0x07
#define CS2_CMD_WRITE_CONFIG  0x08
#define CS2_CMD_ACC_SWITCH    0x0B
#define CS2_CMD_ACC_CONF      0x0C
#define CS2_CMD_S88_POLL      0x10
#define CS2_CMD_S88_EVENT     0x11
#define CS2_CMD_SX1_EVENT     0x12
#define CS2_CMD_PING          0x18
#define CS2_CMD_OFFER_UPDATE  0x19
#define CS2_CMD_READ_CFG_DATA 0x1A
#define CS2_CMD_BOOTLDR_CAN   0x1B
#define CS2_CMD_BOOTLDR_TRACK 0x1C
#define CS2_CMD_STATUS        0x1D
#define CS2_CMD_CONFIG_QUERY  0x20
#define CS2_CMD_CFGDAT_STREAM 0x21
#define CS2_CMD_CONNECT_6021  0x22
#define CS2_CMD_AUTOMATIC     0x30
#define CS2_CMD_DEBUG_MESSAGE 0x42

/**
* Konstanten f&uuml;r Subcommand des CS2 system Kommando (data byte 4)
*/
#define CS2_SUBCMD_SYSTEM_STOP              0x00
#define CS2_SUBCMD_SYSTEM_GO                0x01
#define CS2_SUBCMD_SYSTEM_HALT              0x02
#define CS2_SUBCMD_SYSTEM_EMERGENCY_HALT    0x03
#define CS2_SUBCMD_SYSTEM_CYCLE_END         0x04
#define CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL    0x05
#define CS2_SUBCMD_SYSTEM_DECODER_TIMEING   0x06
#define CS2_SUBCMD_SYSTEM_MFX_FAST_READ     0x07
#define CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL   0x08
#define CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER 0x09
#define CS2_SUBCMD_SYSTEM_OVERLOAD          0x0A
#define CS2_SUBCMD_SYSTEM_STATUS            0x0B
#define CS2_SUBCMD_SYSTEM_SYSTEM_ID         0x0C
#define CS2_SUBCMD_SYSTEM_MFX_SEEK          0x30
#define CS2_SUBCMD_SYSTEM_SYSTEM_RESET      0x80

/**
* Konstanten f&uuml;r uids im CS2 system
*/
#define CS2_UID_BROADCAST 0x00000000l

/**
* Konstanten f&uuml;r Lok Datenprotokoll
* (MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL)
*/
#define CS2_DATA_PROTO_MM2_2040   0
#define CS2_DATA_PROTO_MM2_LOK_20 1
#define CS2_DATA_PROTO_MM2_FKT_40 2
#define CS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define CS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define CS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define CS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define CS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/**
* Konstanten f&uuml;r Gleisprotokoll Bitmaske
* (MR_CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL)
*/
#define CS2_TRACK_PROTO_MM2  0x01
#define CS2_TRACK_PROTO_MFX  0x02
#define CS2_TRACK_PROTO_DCC  0x04
#define CS2_TRACK_PROTO_RES1 0x08
#define CS2_TRACK_PROTO_RES2 0x10
#define CS2_TRACK_PROTO_RES3 0x20
#define CS2_TRACK_PROTO_RES4 0x40
#define CS2_TRACK_PROTO_RES5 0x80

/**
* Konstanten f&uuml;r range/protcoll Feld von lok discovery
*/
#define CS2_DISCOVERY_MFX_1   0
#define CS2_DISCOVERY_MFX_2  32
#define CS2_DISCOVERY_MM2_1  33
#define CS2_DISCOVERY_MM2_2  34
#define CS2_DISCOVERY_DCC_1  35
#define CS2_DISCOVERY_DCC_2  36
#define CS2_DISCOVERY_DCC_3  37
#define CS2_DISCOVERY_SX1_1  38
#define CS2_DISCOVERY_SX1_2  39
#define CS2_DISCOVERY_MFX_3  64
#define CS2_DISCOVERY_MFX_4  96
#define CS2_DISCOVERY_MM2_3  98
#define CS2_DISCOVERY_DCC_4  99
#define CS2_DISCOVERY_DCC_5 100
#define CS2_DISCOVERY_SX1_3 101
#define CS2_DISCOVERY_SX1_4 102

/**
* cv addresse f&uuml;r SX1
*/
#define CS2_SX1_CV_ADDRESS      1
#define CS2_SX1_CV_MAXSPEED     2
#define CS2_SX1_CV_ACCELERATION 3
#define CS2_SX1_CV_PULSEWIDTH   4
#define CS2_SX1_CV_STOPPART     5

/**
* Konstanten f&uuml;r Position im Kommando "switch acc"
*/
#define CS2_POSITION_RED    0x00
#define CS2_POSITION_GREEN  0x01
#define CS2_POSITION_YELLOW 0x02
#define CS2_POSITION_WHITE  0x03

/**
* Konstanten f&uuml;r Device id im ping Kommando
*/
#define CS2_DEVID_GFP      0x0000
#define CS2_DEVID_GLEISBOX 0x0010
#define CS2_DEVID_CONNECT  0x0020
#define CS2_DEVID_MS2_1    0x0030
#define CS2_DEVID_MS2_2    0x0032
#define CS2_DEVID_CS2      0xEEEE
#define CS2_DEVID_WIRELESS 0xFFE0
#define CS2_DEVID_WIRED    0xFFF0
#define CS2_DEVID_CS2GUI   0xFFFF

/**
* Namen f&uuml; cfg data
*/
#define CS2_CFG_LOCINFO  "lokinfo"
#define CS2_CFG_LOCNAMES "loknamen"
#define CS2_CFG_MAGINFO  "maginfo"
#define CS2_CFG_LOCDB    "lokdb"
#define CS2_CFG_LANG     "lang"
#define CS2_CFG_LDBVER   "ldbver"
#define CS2_CFG_LANGVER  "langver"
#define CS2_CFG_LOKS     "loks"
#define CS2_CFG_MAGS     "mags"
#define CS2_CFG_GBS      "gbs"
#define CS2_CFG_GBS_PAGE "gbs-"
#define CS2_CFG_FS       "fs"
#define CS2_CFG_LOK_STAT "lokstat"
#define CS2_CFG_MAG_STAT "magstat"
#define CS2_CFG_GBS_STAT "gbsstat"
#define CS2_CFG_FS_STAT  "fsstat"
#define CS2_CFG_MS2_CONF "ms2-conf"
#define CS2_CFG_LOKLISTE "lokliste"
#define CS2_CFG_FILES    "files"

#define CS2_CFG_DLC 8

/**
* Namen f&uuml;r cs2 Daeien
*/
#define CS2_FILE_EXTENSION ".cs2"
#define CS2_GLEISBILD_PAGE_SUBDIR "gleisbilder/"

/**
* Namen f&uuml;r cs2 Daeien
*/
#define Ms2CalcHash(Uid) MrCs2CalcHash(Uid)
#define Ms2CalcHashFromFolgenummer(Folgenummer) MrCs2CalcHashFromFolgenummer(Folgenummer)

#define Cs2GetNumParamBytes Cs2GetDlc
#define Ms2GetNumParamBytes Cs2GetNumParamBytes


/**
* Funktionen f&uuml;r Umgang mit CAN Id
*/
unsigned short Cs2CalcHash(unsigned long Uid);
unsigned short Cs2CalcHashFromFolgenummer(unsigned int Folgenummer);
BOOL Cs2IsCs2Msg(unsigned Hash, unsigned Command);
void Cs2DecodeId(unsigned long CanId, unsigned *Hash, unsigned *Response,
                 unsigned *Command, unsigned *Prio);
unsigned long Cs2EncodeId(unsigned short Hash, unsigned Response,
                          unsigned Command, unsigned Prio);
unsigned short Cs2CalcCrc(unsigned char *data, unsigned int length);

/**
* Decode Functionen f&uuml;r System Subcmds
*/
void Cs2DecSysStop(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysGo(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysHalt(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysEmergencyHalt(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysCycleEnd(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysDataProtocoll(unsigned char *CanData, unsigned long *Uid,
                            int *Proto);
void Cs2DecSysDecoderTimeing(unsigned char *CanData, unsigned long *Uid,
                             int *Time);
void Cs2DecSysMfxFastRead(unsigned char *CanData, unsigned long *Uid,
                          int *MfxSid);
void Cs2DecSysTrackProtocoll(unsigned char *CanData, unsigned long *Uid,
                             int *Proto);
void Cs2DecSysResetMfxCounter(unsigned char *CanData, unsigned long *Uid,
                              int *Counter);
void Cs2DecSysOverload(unsigned char *CanData, unsigned long *Uid,
                       int *Channel);
void Cs2DecSysStatus6(unsigned char *CanData, unsigned long *Uid,
                      int *Channel);
void Cs2DecSysStatus7(unsigned char *CanData, unsigned long *Uid,
                      int *Channel, int *Set);
void Cs2DecSysStatus8(unsigned char *CanData, unsigned long *Uid,
                      int *Channel, int *Value);
void Cs2DecSysSystemId5(unsigned char *CanData, unsigned long *Uid);
void Cs2DecSysSystemId7(unsigned char *CanData, unsigned long *Uid,
                       int *Id);
void Cs2DecSysSystemReset(unsigned char *CanData, unsigned long *Uid,
                          int *ResteTarget);
/**
* Encode Functionen f&uuml;r System Subcmds
*/
void Cs2EncSysStop(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysGo(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysHalt(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysEmergencyHalt(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysCycleEnd(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysDataProtocoll(unsigned char *CanData, unsigned long Uid,
                            int Proto);
void Cs2EncSysDecoderTimeing(unsigned char *CanData, unsigned long Uid,
                             int Time);
void Cs2EncSysMfxFastRead(unsigned char *CanData, unsigned long Uid,
                          int MfxSid);
void Cs2EncSysTrackProtocoll(unsigned char *CanData, unsigned long Uid,
                             int Proto);
void Cs2EncSysResetMfxCounter(unsigned char *CanData, unsigned long Uid,
                              int Counter);
void Cs2EncSysOverload(unsigned char *CanData, unsigned long Uid,
                       int Channel);
void Cs2EncSysStatus6(unsigned char *CanData, unsigned long Uid,
                      int Channel);
void Cs2EncSysStatus7(unsigned char *CanData, unsigned long Uid,
                      int Channel, int Set);
void Cs2EncSysStatus8(unsigned char *CanData, unsigned long Uid,
                      int Channel, int Value);
void Cs2EncSysSystemId5(unsigned char *CanData, unsigned long Uid);
void Cs2EncSysSystemId7(unsigned char *CanData, unsigned long Uid,
                       int Id);
void Cs2EncSysSystemReset(unsigned char *CanData, unsigned long Uid,
                          int ResteTarget);
/**
* Decode Functionen f&uuml;r CAN Kommandos
*/
void Cs2DecDiscovery0(unsigned char *CanData);
void Cs2DecDiscovery1(unsigned char *CanData, int *Proto);
void Cs2DecDiscovery5(unsigned char *CanData, unsigned long *Uid,
                      int *Range);
void Cs2DecDiscovery6(unsigned char *CanData, unsigned long *Uid,
                      int *Range, int *Ask);
void Cs2DecBind(unsigned char *CanData, unsigned long *Uid,
                unsigned int *Sid);
void Cs2DecMfxVerify6(unsigned char *CanData, unsigned long *Uid,
                      unsigned int *Sid);
void Cs2DecMfxVerify7(unsigned char *CanData, unsigned long *Uid,
                      unsigned int *Sid, int *Ask);
void Cs2DecSpeed4(unsigned char *CanData, unsigned long *LocId);
void Cs2DecSpeed6(unsigned char *CanData, unsigned long *LocId, int *Speed);
void Cs2DecDirection4(unsigned char *CanData, unsigned long *LocId);
void Cs2DecDirection5(unsigned char *CanData, unsigned long *LocId,
                      int *Direction);
void Cs2DecFunction5(unsigned char *CanData, unsigned long *LocId,
                     int *Function);
void Cs2DecFunction6(unsigned char *CanData, unsigned long *LocId,
                     int *Function, int *Value);
void Cs2DecFunction8(unsigned char *CanData, unsigned long *LocId,
                     int *Function, int *Value, int *FktVal);
void Cs2DecReadConfig6(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex);
void Cs2DecReadConfig7(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex, int *Param);
void Cs2DecWriteConfig(unsigned char *CanData, unsigned long *LocId,
                       int *CvIndex, int *Value, int *CtrlRslt);
void Cs2DecAccSwitch6(unsigned char *CanData, unsigned long *LocId,
                      int *Position, int *Current);
void Cs2DecAccSwitch8(unsigned char *CanData, unsigned long *LocId,
                      int *Position, int *Current, int *TimeFkt);
void Cs2DecS88Poll5(unsigned char *CanData, unsigned long *Uid,
                    int *NumModuls);
void Cs2DecS88Poll7(unsigned char *CanData, unsigned long *Uid,
                    int *Modul, int *State);
void Cs2DecS88Event4(unsigned char *CanData, int *Device, int *Contact);
void Cs2DecS88Event5(unsigned char *CanData, int *Device, int *Contact,
                     int *Param);
void Cs2DecS88Event8(unsigned char *CanData, int *Device, int *Contact,
                     int *StateOld, int *StateNew, int *Time);
void Cs2DecPing0(unsigned char *CanData);
void Cs2DecPing8(unsigned char *CanData, unsigned long *Uid,
                 int *SwVersion, int *DeviceId);
void Cs2DecCanBootldr(unsigned char *CanData, char *Bytes);
void Cs2DecStatus5(unsigned char *CanData, unsigned long *Uid, int *Index);
void Cs2DecStatus6(unsigned char *CanData, unsigned long *Uid, int *Index,
                   int *NumPackets);
void Cs2DecStatus8(unsigned char *CanData, char *Bytes);
#define Cs2DecConfigQuery0(CanMsg)
void Cs2DecConfigQuery(unsigned char *CanData, char *FileName);
void Cs2DecCfgdatStream6(unsigned char *CanData, unsigned long *Length,
                         int *Crc);
void Cs2DecCfgdatStream7(unsigned char *CanData, unsigned long *Length,
                         int *Crc);
void Cs2DecCfgdatStream8(unsigned char *CanData, char *Bytes);
void Cs2DecAutomatic6(unsigned char *CanData, int *DeviceId, int *Func,
                      int *Status, int *Param);
void Cs2DecAutomatic8(unsigned char *CanData, int *DeviceId, int *Func,
                      unsigned long *LocId);
void Cs2DecDebugMessage(unsigned char *CanData, char *Message);
/**
* Encode Functionen f&uuml;r CAN Kommandos
*/
void Cs2EncDiscovery0(unsigned char *CanData);
void Cs2EncDiscovery1(unsigned char *CanData, int Proto);
void Cs2EncDiscovery5(unsigned char *CanData, unsigned long Uid,
                      int Range);
void Cs2EncDiscovery6(unsigned char *CanData, unsigned long Uid,
                      int Range, int Ask);
void Cs2EncBind(unsigned char *CanData, unsigned long Uid,
                unsigned int Sid);
void Cs2EncMfxVerify6(unsigned char *CanData, unsigned long Uid,
                      unsigned int Sid);
void Cs2EncMfxVerify7(unsigned char *CanData, unsigned long Uid,
                      unsigned int Sid, int Ask);
void Cs2EncSpeed4(unsigned char *CanData, unsigned long LocId);
void Cs2EncSpeed6(unsigned char *CanData, unsigned long LocId, int Speed);
void Cs2EncDirection4(unsigned char *CanData, unsigned long LocId);
void Cs2EncDirection5(unsigned char *CanData, unsigned long LocId,
                      int Direction);
void Cs2EncFunction5(unsigned char *CanData, unsigned long LocId,
                     int Function);
void Cs2EncFunction6(unsigned char *CanData, unsigned long LocId,
                     int Function, int Value);
void Cs2EncFunction8(unsigned char *CanData, unsigned long LocId,
                     int Function, int Value, int FktVal);
void Cs2EncReadConfig6(unsigned char *CanData, unsigned long LocId,
                       int CvIndex);
void Cs2EncReadConfig7(unsigned char *CanData, unsigned long LocId,
                       int CvIndex, int Param);
void Cs2EncWriteConfig(unsigned char *CanData, unsigned long LocId,
                       int CvIndex, int Value, int CtrlRslt);
void Cs2EncAccSwitch6(unsigned char *CanData, unsigned long LocId,
                      int Position, int Current);
void Cs2EncAccSwitch8(unsigned char *CanData, unsigned long LocId,
                      int Position, int Current, int TimeFkt);
void Cs2EncS88Poll5(unsigned char *CanData, unsigned long Uid,
                    int NumModuls);
void Cs2EncS88Poll7(unsigned char *CanData, unsigned long Uid,
                    int Modul, int State);
void Cs2EncS88Event4(unsigned char *CanData, int Device, int Contact);
void Cs2EncS88Event5(unsigned char *CanData, int Device, int Contact,
                     int Param);
void Cs2EncS88Event8(unsigned char *CanData, int Device, int Contact,
                     int StateOld, int StateNew, int Time);
void Cs2EncPing0(unsigned char *CanData);
void Cs2EncPing8(unsigned char *CanData, unsigned long Uid,
                 int SwVersion, int DeviceId);
void Cs2EncCanBootldr(unsigned char *CanData, char *Bytes);
void Cs2EncStatus5(unsigned char *CanData, unsigned long Uid, int Index);
void Cs2EncStatus6(unsigned char *CanData, unsigned long Uid, int Index,
                   int NumPackets);
void Cs2EncStatus8(unsigned char *CanData, char *Bytes);
void Cs2EncConfigQuery0(unsigned char *CanData);
void Cs2EncConfigQuery(unsigned char *CanData, char *FileName);
void Cs2EncCfgdatStream6(unsigned char *CanData, unsigned long Length,
                         int Crc);
void Cs2EncCfgdatStream7(unsigned char *CanData, unsigned long Length,
                         int Crc);
void Cs2EncCfgdatStream8(unsigned char *CanData, char *Bytes);
void Cs2EncAutomatic6(unsigned char *CanData, int DeviceId, int Func,
                      int Status, int Param);
void Cs2EncAutomatic8(unsigned char *CanData, int DeviceId, int Func,
                      unsigned long LocId);
void Cs2EncDebugMessage(unsigned char *CanData, char *Message);
/**
* Encoder/Decoder f&uuml;r den Status Info Datenblock
*/
int Cs2StatusInfoEncode(char *Buffer, int NumCfg, int NumStat,
                        unsigned long SerienNummer, char *ArtikelNummer,
                        char *ArtikelName);
void Cs2StatusInfoDecode(char *Buffer, int *NumCfg, int *NumStat,
                         unsigned long *SerienNummer, char *ArtikelNummer,
                         char *ArtikelName);

/* andere Namen fuer MS2 */

/* andere Namen fuer Auswertung MS2 CAN Id */
#define MS2_SHIFT_HASH     CS2_SHIFT_HASH
#define MS2_SHIFT_RESP     CS2_SHIFT_RESP
#define MS2_SHIFT_COMMAND  CS2_SHIFT_COMMAND
#define MS2_SHIFT_PRIO     CS2_SHIFT_PRIO

#define MS2_MASK_HASH    CS2_MASK_HASH
#define MS2_MASK_RESP    CS2_MASK_RESP
#define MS2_MASK_COMMAND CS2_MASK_COMMAND
#define MS2_MASK_PRIO    CS2_MASK_PRIO

#define MS2_SHIFT_HASH_MSB  CS2_SHIFT_HASH_MSB
#define MS2_MASK_HASH_MAGIC CS2_MASK_HASH_MAGIC
#define MS2_HASH_MAGIC      CS2_HASH_MAGIC
#define MS2_MASK_HASH_LOW_BYTE  CS2_MASK_HASH_LOW_BYTE
#define MS2_MASK_HASH_HIGH_BYTE CS2_MASK_HASH_HIGH_BYTE

/* andere Namen fuer prio Feld */
#define MS2_PRIO_1 CS2_PRIO_1
#define MS2_PRIO_2 CS2_PRIO_2
#define MS2_PRIO_3 CS2_PRIO_3
#define MS2_PRIO_4 CS2_PRIO_4

/* andere Namen fuer command field of MS2 can id */
#define MS2_CMD_SYSTEM        CS2_CMD_SYSTEM
#define MS2_CMD_DISCOVERY     CS2_CMD_DISCOVERY
#define MS2_CMD_BIND          CS2_CMD_BIND
#define MS2_CMD_MFX_VERIFY    CS2_CMD_MFX_VERIFY
#define MS2_CMD_SPEED         CS2_CMD_SPEED
#define MS2_CMD_DIRECTION     CS2_CMD_DIRECTION
#define MS2_CMD_FUNCTION      CS2_CMD_FUNCTION
#define MS2_CMD_READ_CONFIG   CS2_CMD_READ_CONFIG
#define MS2_CMD_WRITE_CONFIG  CS2_CMD_WRITE_CONFIG
#define MS2_CMD_ACC_SWITCH    CS2_CMD_ACC_SWITCH
#define MS2_CMD_ACC_CONF      CS2_CMD_ACC_CONF
#define MS2_CMD_S88_POLL      CS2_CMD_S88_POLL
#define MS2_CMD_S88_EVENT     CS2_CMD_S88_EVENT
#define MS2_CMD_SX1_EVENT     CS2_CMD_SX1_EVENT
#define MS2_CMD_PING          CS2_CMD_PING
#define MS2_CMD_OFFER_UPDATE  CS2_CMD_OFFER_UPDATE
#define MS2_CMD_READ_CFG_DATA CS2_CMD_READ_CFG_DATA
#define MS2_CMD_BOOTLDR_CAN   CS2_CMD_BOOTLDR_CAN
#define MS2_CMD_BOOTLDR_TRACK CS2_CMD_BOOTLDR_TRACK
#define MS2_CMD_STATUS        CS2_CMD_STATUS
#define MS2_CMD_CONFIG_QUERY  CS2_CMD_CONFIG_QUERY
#define MS2_CMD_CFGDAT_STREAM CS2_CMD_CFGDAT_STREAM
#define MS2_CMD_CONNECT_6021  CS2_CMD_CONNECT_6021
#define MS2_CMD_AUTOMATIC     CS2_CMD_AUTOMATIC

/* andere Namen fuer sub commands of MS2 system command (data byte 4) */
#define MS2_SUBCMD_SYSTEM_STOP              CS2_SUBCMD_SYSTEM_STOP
#define MS2_SUBCMD_SYSTEM_GO                CS2_SUBCMD_SYSTEM_GO
#define MS2_SUBCMD_SYSTEM_HALT              CS2_SUBCMD_SYSTEM_HALT
#define MS2_SUBCMD_SYSTEM_EMERGENCY_HALT    CS2_SUBCMD_SYSTEM_EMERGENCY_HALT
#define MS2_SUBCMD_SYSTEM_CYCLE_END         CS2_SUBCMD_SYSTEM_CYCLE_END
#define MS2_SUBCMD_SYSTEM_DATA_PROTOCOLL    CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL
#define MS2_SUBCMD_SYSTEM_DECODER_TIMEING   CS2_SUBCMD_SYSTEM_DECODER_TIMEING
#define MS2_SUBCMD_SYSTEM_MFX_FAST_READ     CS2_SUBCMD_SYSTEM_MFX_FAST_READ
#define MS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL   CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL
#define MS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER
#define MS2_SUBCMD_SYSTEM_OVERLOAD          CS2_SUBCMD_SYSTEM_OVERLOAD
#define MS2_SUBCMD_SYSTEM_STATUS            CS2_SUBCMD_SYSTEM_STATUS
#define MS2_SUBCMD_SYSTEM_SYSTEM_ID         CS2_SUBCMD_SYSTEM_SYSTEM_ID
#define MS2_SUBCMD_SYSTEM_MFX_SEEK          CS2_SUBCMD_SYSTEM_MFX_SEEK
#define MS2_SUBCMD_SYSTEM_SYSTEM_RESET      CS2_SUBCMD_SYSTEM_SYSTEM_RESET

/* andere namen fuer Lok Datenprotokoll */
#define MS2_DATA_PROTO_MM2_2040   0
#define MS2_DATA_PROTO_MM2_LOK_20 1
#define MS2_DATA_PROTO_MM2_FKT_40 2
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define MS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define MS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/* andere Namen fuer uids fue MS3 */
#define MS2_UID_BROADCAST CS2_UID_BROADCAST

/* andere name fuer Lok Datenprotokoll (MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL) */
#define MS2_DATA_PROTO_MM2_2040   0
#define MS2_DATA_PROTO_MM2_LOK_20 1
#define MS2_DATA_PROTO_MM2_FKT_40 2
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define MS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define MS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define MS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/* andere namen fuer track protocoll */
#define MS2_TRACK_PROTO_MM2  0x01
#define MS2_TRACK_PROTO_MFX  0x02
#define MS2_TRACK_PROTO_DCC  0x04
#define MS2_TRACK_PROTO_RES1 0x08
#define MS2_TRACK_PROTO_RES2 0x10
#define MS2_TRACK_PROTO_RES3 0x20
#define MS2_TRACK_PROTO_RES4 0x40
#define MS2_TRACK_PROTO_RES5 0x80

/* andere namen fuer range/protocll feld von lok discovery */
#define MS2_DISCOVERY_MFX_1 CS2_DISCOVERY_MFX_1
#define MS2_DISCOVERY_MFX_2 CS2_DISCOVERY_MFX_2
#define MS2_DISCOVERY_MM2_1 CS2_DISCOVERY_MM2_1
#define MS2_DISCOVERY_MM2_2 CS2_DISCOVERY_MM2_2
#define MS2_DISCOVERY_DCC_1 CS2_DISCOVERY_DCC_1
#define MS2_DISCOVERY_DCC_2 CS2_DISCOVERY_DCC_2
#define MS2_DISCOVERY_DCC_3 CS2_DISCOVERY_DCC_3
#define MS2_DISCOVERY_SX1_1 CS2_DISCOVERY_SX1_1
#define MS2_DISCOVERY_SX1_2 CS2_DISCOVERY_SX1_2
#define MS2_DISCOVERY_MFX_3 CS2_DISCOVERY_MFX_3
#define MS2_DISCOVERY_MFX_4 CS2_DISCOVERY_MFX_4
#define MS2_DISCOVERY_MM2_3 CS2_DISCOVERY_MM2_3
#define MS2_DISCOVERY_DCC_4 CS2_DISCOVERY_DCC_4
#define MS2_DISCOVERY_DCC_5 CS2_DISCOVERY_DCC_5
#define MS2_DISCOVERY_SX1_3 CS2_DISCOVERY_SX1_3
#define MS2_DISCOVERY_SX1_4 CS2_DISCOVERY_SX1_4

/* andere namen fuer cv address for SX1 */
#define MS2_SX1_CV_ADDRESS      CS2_SX1_CV_ADDRESS
#define MS2_SX1_CV_MAXSPEED     CS2_SX1_CV_MAXSPEED
#define MS2_SX1_CV_ACCELERATION CS2_SX1_CV_ACCELERATION
#define MS2_SX1_CV_PULSEWIDTH   CS2_SX1_CV_PULSEWIDTH
#define MS2_SX1_CV_STOPPART     CS2_SX1_CV_STOPPART

/* andere namen fuer value for position in switch acc */
#define MS2_POSITION_RED    CS2_POSITION_RED
#define MS2_POSITION_GREEN  CS2_POSITION_GREEN
#define MS2_POSITION_YELLOW CS2_POSITION_YELLOW
#define MS2_POSITION_WHITE  CS2_POSITION_WHITE

/* andere namen fuer device if for ping */
#define MS2_DEVID_GFP      CS2_DEVID_GFP
#define MS2_DEVID_GLEISBOX CS2_DEVID_GLEISBOX
#define MS2_DEVID_CONNECT  CS2_DEVID_CONNECT
#define MS2_DEVID_MS2      CS2_DEVID_MS2
#define MS2_DEVID_CS2      CS2_DEVID_CS2
#define MS2_DEVID_WIRELESS CS2_DEVID_WIRELESS
#define MS2_DEVID_CS2GUI   CS2_DEVID_CS2GUI

/* andere namen fuerr cfg data */
#define MS2_CFG_LOCINFO  CS2_CFG_LOCINFO
#define MS2_CFG_LOCNAMES CS2_CFG_LOCNAMES
#define MS2_CFG_LOCDB    CS2_CFG_LOCDB

/* andere Namen fuer decode funtions */
#define Ms2DecSysStop            Cs2DecSysStop
#define Ms2DecSysGo              Cs2DecSysGo
#define Ms2DecSysHalt            Cs2DecSysHalt
#define Ms2DecSysEmergencyHalt   Cs2DecSysEmergencyHalt
#define Ms2DecSysCycleEnd        Cs2DecSysCycleEnd
#define Ms2DecSysDataProtocoll   Cs2DecSysDataProtocoll
#define Ms2DecSysDecoderTimeing  Cs2DecSysDecoderTimeing
#define Ms2DecSysMfxFastRead     Cs2DecSysMfxFastRead
#define Ms2DecSysTrackProtocoll  Cs2DecSysTrackProtocoll
#define Ms2DecSysResetMfxCounter Cs2DecSysResetMfxCounter
#define Ms2DecSysOverload        Cs2DecSysOverload
#define Ms2DecSysStatus6         Cs2DecSysStatus6
#define Ms2DecSysStatus7         Cs2DecSysStatus7
#define Ms2DecSysStatus8         Cs2DecSysStatus8
#define Ms2DecSysSystemId5       Cs2DecSysSystemId5
#define Ms2DecSysSystemId7       Cs2DecSysSystemId7
#define Ms2DecSysSystemReset     Cs2DecSysSystemReset

#define Ms2EncSysStop            Cs2EncSysStop
#define Ms2EncSysGo              Cs2EncSysGo
#define Ms2EncSysHalt            Cs2EncSysHalt
#define Ms2EncSysEmergencyHalt   Cs2EncSysEmergencyHalt
#define Ms2EncSysCycleEnd        Cs2EncSysCycleEnd
#define Ms2EncSysDataProtocoll   Cs2EDSysDataProtocoll
#define Ms2EncSysDecoderTimeing  Cs2EnecSysDecoderTimeing
#define Ms2EncSysMfxFastRead     Cs2EncSysMfxFastRead
#define Ms2EncSysTrackProtocoll  Cs2EncSysTrackProtocoll
#define Ms2EncSysResetMfxCounter Cs2EncSysResetMfxCounter
#define Ms2EncSysOverload        Cs2EncSysOverload
#define Ms2EncSysStatus6         Cs2EncSysStatus6
#define Ms2EncSysStatus7         Cs2EncSysStatus7
#define Ms2EncSysStatus8         Cs2EncSysStatus8
#define Ms2EncSysSystemId5       Cs2EncSysSystemId5
#define Ms2EncSysSystemId7       Cs2EncSysSystemId7
#define Ms2EncSysSystemReset     Cs2EncSysSystemReset

#define Ms2DecDiscovery0    Cs2DecDiscovery0
#define Ms2DecDiscovery1    Cs2DecDiscovery1
#define Ms2DecDiscovery5    Cs2DecDiscovery5
#define Ms2DecDiscovery6    Cs2DecDiscovery6
#define Ms2DecBind          Cs2DecBind
#define Ms2DecMfxVerify6    Cs2DecMfxVerify6
#define Ms2DecMfxVerify7    Cs2DecMfxVerify7
#define Ms2DecSpeed4        Cs2DecSpeed4
#define Ms2DecSpeed6        Cs2DecSpeed6
#define Ms2DecDirection4    Cs2DecDirection4
#define Ms2DecDirection5    Cs2DecDirection5
#define Ms2DecFunction5     Cs2DecFunction5
#define Ms2DecFunction6     Cs2DecFunction6
#define Ms2DecFunction8     Cs2DecFunction8
#define Ms2DecReadConfig6   Cs2DecReadConfig6
#define Ms2DecReadConfig7   Cs2DecReadConfig7
#define Ms2DecWriteConfig   Cs2DecWriteConfig
#define Ms2DecAccSwitch6    Cs2DecAccSwitch6
#define Ms2DecAccSwitch8    Cs2DecAccSwitch8
#define Ms2DecS88Poll5      Cs2DecS88Poll5
#define Ms2DecS88Poll7      Cs2DecS88Poll7
#define Ms2DecS88Event4     Cs2DecS88Event4
#define Ms2DecS88Event5     Cs2DecS88Event5
#define Ms2DecS88Event8     Cs2DecS88Event8
#define Ms2DecPing0         Cs2DecPing0
#define Ms2DecPing8         Cs2DecPing8
#define Ms2DecCanBootldr    Cs2DecCanBootldr
#define Ms2DecStatus5       Cs2DecStatus5
#define Ms2DecStatus6       Cs2DecStatus6
#define Ms2DecStatus8       Cs2DecStatus8
#define Ms2DecConfigQuery0  Cs2DecConfigQuery0
#define Ms2DecConfigQuery   Cs2DecConfigQuery
#define Ms2DecCfgdatStream6 Cs2DecCfgdatStream6
#define Ms2DecCfgdatStream7 Cs2DecCfgdatStream7
#define Ms2DecCfgdatStream8 Cs2DecCfgdatStream8
#define Ms2DecAutomatic6    Cs2DecAutomatic6
#define Ms2DecAutomatic8    Cs2DecAutomatic8
#define Ms2DecDebugMessage  Cs2DecDebugMessage

#define Ms2EncDiscovery0    Cs2EncDiscovery0
#define Ms2EncDiscovery1    Cs2EncDiscovery1
#define Ms2EncDiscovery5    Cs2EncDiscovery5
#define Ms2EncDiscovery6    Cs2EncDiscovery6
#define Ms2EncBind          Cs2EncBind
#define Ms2EncMfxVerify6    Cs2EncMfxVerify6
#define Ms2EncMfxVerify7    Cs2EncMfxVerify7
#define Ms2EncSpeed4        Cs2EncSpeed4
#define Ms2EncSpeed6        Cs2EncSpeed6
#define Ms2EncDirection4    Cs2EncDirection4
#define Ms2EncDirection5    Cs2EncDirection5
#define Ms2EncFunction5     Cs2EncFunction5
#define Ms2EncFunction6     Cs2EncFunction6
#define Ms2EncFunction8     Cs2EncFunction8
#define Ms2EncReadConfig6   Cs2EncReadConfig6
#define Ms2EncReadConfig7   Cs2EncReadConfig7
#define Ms2EncWriteConfig   Cs2EncWriteConfig
#define Ms2EncAccSwitch6    Cs2EncAccSwitch6
#define Ms2EncAccSwitch8    Cs2EncAccSwitch8
#define Ms2EncS88Poll5      Cs2EncS88Poll5
#define Ms2EncS88Poll7      Cs2EncS88Poll7
#define Ms2EncS88Event4     Cs2EncS88Event4
#define Ms2EncS88Event5     Cs2EncS88Event5
#define Ms2EncS88Event8     Cs2EncS88Event8
#define Ms2EncPing0         Cs2EncPing0
#define Ms2EncPing8         Cs2EncPing8
#define Ms2EncCanBootldr    Cs2EncCanBootldr
#define Ms2EncStatus5       Cs2EncStatus5
#define Ms2EncStatus6       Cs2EncStatus6
#define Ms2EncStatus8       Cs2EncStatus8
#define Ms2EncConfigQuery0  Cs2EncConfigQuery0
#define Ms2EncConfigQuery   Cs2EncConfigQuery
#define Ms2EncCfgdatStream6 Cs2EncCfgdatStream6
#define Ms2EncCfgdatStream7 Cs2EncCfgdatStream7
#define Ms2EncCfgdatStream8 Cs2EncCfgdatStream8
#define Ms2EncAutomatic6    Cs2EncAutomatic6
#define Ms2EncAutomatic8    Cs2EncAutomatic8
#define Ms2EncDebugMessage  Cs2EncDebugMessage

#ifdef __cpluplus
}
#endif

#endif
