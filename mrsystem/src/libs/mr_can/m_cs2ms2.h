#ifndef M_CS2MS2_H
#define M_CS2MS2_H

#include <linux/can.h>
#include <boolean.h>
#include <bitmask.h>

#define MR_CS2_NUM_CAN_BYTES 8

typedef struct {
   unsigned long Id;
   unsigned char Dlc; /* = number of data bytes */
   unsigned char Data[MR_CS2_NUM_CAN_BYTES];
   BOOL IsCs2;
   unsigned Hash;     /* in network order */
   unsigned Response;
   unsigned Command;
   unsigned Prio;
} MrCs2CanDataType;

/* defines fuer Zugriff auf cs2 data type fields */
#define MrCs2IsCs2(CanData)                  (CanData)->IsCs2
#define MrCs2GetCommand(CanData)             (CanData)->Command
#define MrCs2GetSystemSubcmd(CanData)        (CanData)->Data[4]
#define MrCs2SetCommand(CanData,Cmd)         (CanData)->Command=Cmd
#define MrCs2SetSystemSubcmd(CanData,SubCmd) (CanData)->Data[4]=SubCmd
#define MrCs2GetId(CanData)                  (CanData)->Id
#define MrCs2SetId(CanData,id)               (CanData)->Id=id
#define MrCs2GetDlc(CanData)                 (CanData)->Dlc
#define MrCs2SetDlc(CanData,dlc)             (CanData)->Dlc=dlc
#define MrCs2GetIsCs2(CanData)               (CanData)->IsCs2
#define MrCs2SetIsCs2(CanData,is_cs2)        (CanData)->IsCs2=is_cs2
#define MrCs2GetHash(CanData)                (CanData)->Hash
#define MrCs2SetHash(CanData,hash)           (CanData)->Hash=hash
#define MrCs2GetResponse(CanData)            (CanData)->Response
#define MrCs2SetResponse(CanData,response)   (CanData)->Response=response
#define MrCs2GetPrio(CanData)                (CanData)->Prio
#define MrCs2SetPrio(CanData,prio)           (CanData)->Prio=prio
#define MrCs2GetData(CanData)                (CanData)->Data


/* Define fuer Auswertung CS2 CAN Id */
#define MR_CS2_SHIFT_HASH     0
#define MR_CS2_SHIFT_RESP    16
#define MR_CS2_SHIFT_COMMAND 17
#define MR_CS2_SHIFT_PRIO    25

#define MR_CS2_MASK_HASH    BIT_MASK( 0, 15)
#define MR_CS2_MASK_RESP    BIT_MASK(16, 16)
#define MR_CS2_MASK_COMMAND BIT_MASK(17, 24)
#define MR_CS2_MASK_PRIO    BIT_MASK(25, 29)

#define MR_CS2_SHIFT_HASH_MSB  3
#define MR_CS2_MASK_HASH_MAGIC BIT_MASK(7,9)
#define MR_CS2_HASH_MAGIC      BIT_MASK(8,9)
#define MR_CS2_MASK_HASH_LOW_BYTE  BIT_MASK(0, 7)
#define MR_CS2_MASK_HASH_HIGH_BYTE BIT_MASK(7,15)

/* Define fuer Folgenummer */
#define MR_CS2_FOLGENUMMER_LOW  BIT_MASK( 0, 6)
#define MR_CS2_FOLGENUMMER_HIGH BIT_MASK(10,15)

/* defines fo prio field */
#ifdef MR_CS2_USE_PRIO
#define MR_CS2_PRIO_0 0 /* not prio set */
#define MR_CS2_PRIO_1 1 /* Stopp / Go / Kurzschluss-Meldungen */
#define MR_CS2_PRIO_2 2 /* Rueckmeldungen */
#define MR_CS2_PRIO_3 3 /* Lok anhalten */
#define MR_CS2_PRIO_4 4 /* Lok / Zubehoerbefehle */
#else
#define MR_CS2_PRIO_0 0 /* not prio set */
#define MR_CS2_PRIO_1 0 /* Stopp / Go / Kurzschluss-Meldungen */
#define MR_CS2_PRIO_2 0 /* Rueckmeldungen */
#define MR_CS2_PRIO_3 0 /* Lok anhalten */
#define MR_CS2_PRIO_4 0 /* Lok / Zubehoerbefehle */
#endif

/* defines for command field of CS2 can id */
#define MR_CS2_CMD_SYSTEM        0x00
#define MR_CS2_CMD_DISCOVERY     0x01
#define MR_CS2_CMD_BIND          0x02
#define MR_CS2_CMD_MFX_VERIFY    0x03
#define MR_CS2_CMD_SPEED         0x04
#define MR_CS2_CMD_DIRECTION     0x05
#define MR_CS2_CMD_FUNCTION      0x06
#define MR_CS2_CMD_READ_CONFIG   0x07
#define MR_CS2_CMD_WRITE_CONFIG  0x08
#define MR_CS2_CMD_ACC_SWITCH    0x0B
#define MR_CS2_CMD_ACC_CONF      0x0C
#define MR_CS2_CMD_S88_POLL      0x10
#define MR_CS2_CMD_S88_EVENT     0x11
#define MR_CS2_CMD_SX1_EVENT     0x12
#define MR_CS2_CMD_PING          0x18
#define MR_CS2_CMD_OFFER_UPDATE  0x19
#define MR_CS2_CMD_READ_CFG_DATA 0x1A
#define MR_CS2_CMD_BOOTLDR_CAN   0x1B
#define MR_CS2_CMD_BOOTLDR_TRACK 0x1C
#define MR_CS2_CMD_STATUS        0x1D
#define MR_CS2_CMD_CONFIG_QUERY  0x20
#define MR_CS2_CMD_CFGDAT_STREAM 0x21
#define MR_CS2_CMD_CONNECT_6021  0x22
#define MR_CS2_CMD_AUTOMATIC     0x30

/* defines for sub commands of CS2 system command (data byte 4) */
#define MR_CS2_SUBCMD_SYSTEM_STOP              0x00
#define MR_CS2_SUBCMD_SYSTEM_GO                0x01
#define MR_CS2_SUBCMD_SYSTEM_HALT              0x02
#define MR_CS2_SUBCMD_SYSTEM_EMERGENCY_HALT    0x03
#define MR_CS2_SUBCMD_SYSTEM_CYCLE_END         0x04
#define MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL    0x05
#define MR_CS2_SUBCMD_SYSTEM_DECODER_TIMEING   0x06
#define MR_CS2_SUBCMD_SYSTEM_MFX_FAST_READ     0x07
#define MR_CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL   0x08
#define MR_CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER 0x09
#define MR_CS2_SUBCMD_SYSTEM_OVERLOAD          0x0A
#define MR_CS2_SUBCMD_SYSTEM_STATUS            0x0B
#define MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID         0x0C
#define MR_CS2_SUBCMD_SYSTEM_MFX_SEEK          0x30
#define MR_CS2_SUBCMD_SYSTEM_SYSTEM_RESET      0x80

/* defines for uids in CS2 system */
#define MR_CS2_UID_BROADCAST 0x00000000l

/* defines fuer Lok Datenprotokoll (MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL) */
#define MR_CS2_DATA_PROTO_MM2_2040   0
#define MR_CS2_DATA_PROTO_MM2_LOK_20 1
#define MR_CS2_DATA_PROTO_MM2_FKT_40 2
#define MR_CS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define MR_CS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define MR_CS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define MR_CS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define MR_CS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/* bits for bitmask of track protocoll (MR_CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL) */
#define MR_CS2_TRACK_PROTO_MM2  0x01
#define MR_CS2_TRACK_PROTO_MFX  0x02
#define MR_CS2_TRACK_PROTO_DCC  0x04
#define MR_CS2_TRACK_PROTO_RES1 0x08
#define MR_CS2_TRACK_PROTO_RES2 0x10
#define MR_CS2_TRACK_PROTO_RES3 0x20
#define MR_CS2_TRACK_PROTO_RES4 0x40
#define MR_CS2_TRACK_PROTO_RES5 0x80

/* defines fuer range/protocll feld von lok discovery */
#define MR_CS2_DISCOVERY_MFX_1   0
#define MR_CS2_DISCOVERY_MFX_2  32
#define MR_CS2_DISCOVERY_MM2_1  33
#define MR_CS2_DISCOVERY_MM2_2  34
#define MR_CS2_DISCOVERY_DCC_1  35
#define MR_CS2_DISCOVERY_DCC_2  36
#define MR_CS2_DISCOVERY_DCC_3  37
#define MR_CS2_DISCOVERY_SX1_1  38
#define MR_CS2_DISCOVERY_SX1_2  39
#define MR_CS2_DISCOVERY_MFX_3  64
#define MR_CS2_DISCOVERY_MFX_4  96
#define MR_CS2_DISCOVERY_MM2_3  98
#define MR_CS2_DISCOVERY_DCC_4  99
#define MR_CS2_DISCOVERY_DCC_5 100
#define MR_CS2_DISCOVERY_SX1_3 101
#define MR_CS2_DISCOVERY_SX1_4 102

/* cv address for SX1 */
#define MR_CS2_SX1_CV_ADDRESS      1
#define MR_CS2_SX1_CV_MAXSPEED     2
#define MR_CS2_SX1_CV_ACCELERATION 3
#define MR_CS2_SX1_CV_PULSEWIDTH   4
#define MR_CS2_SX1_CV_STOPPART     5

/* value for position in switch acc */
#define MR_CS2_POSITION_RED    0x00
#define MR_CS2_POSITION_GREEN  0x01
#define MR_CS2_POSITION_YELLOW 0x02
#define MR_CS2_POSITION_WHITE  0x03

/* device if for ping */
#define MR_CS2_DEVID_GFP      0x0000
#define MR_CS2_DEVID_GLEISBOX 0x0010
#define MR_CS2_DEVID_CONNECT  0x0020
#define MR_CS2_DEVID_MS2_1    0x0030
#define MR_CS2_DEVID_MS2_2    0x0032
#define MR_CS2_DEVID_CS2      0xEEEE
#define MR_CS2_DEVID_WIRELESS 0xFFE0
#define MR_CS2_DEVID_WIRED    0xFFF0
#define MR_CS2_DEVID_CS2GUI   0xFFFF

/* names for cfg data */
#define MR_CS2_CFG_LOCINFO  "lokinfo"
#define MR_CS2_CFG_LOCNAMES "loknamen"
#define MR_CS2_CFG_MAGINFO  "maginfo"
#define MR_CS2_CFG_LOCDB    "lokdb"
#define MR_CS2_CFG_LANG     "lang"
#define MR_CS2_CFG_LDBVER   "ldbver"
#define MR_CS2_CFG_LANGVER  "langver"
#define MR_CS2_CFG_LOKS     "loks"
#define MR_CS2_CFG_MAGS     "mags"
#define MR_CS2_CFG_GBS      "gbs"
#define MR_CS2_CFG_GBS_PAGE "gbs-"
#define MR_CS2_CFG_FS       "fs"
#define MR_CS2_CFG_LOK_STAT "lokstat"
#define MR_CS2_CFG_MAG_STAT "magstat"
#define MR_CS2_CFG_GBS_STAT "gbsstat"
#define MR_CS2_CFG_FS_STAT  "fsstat"
#define MR_CS2_CFG_DLC 8

/* names for cs2 files */
#define MR_CS2_FILE_EXTENSION ".cs2"
#define MR_CS2_GLEISBILD_PAGE_SUBDIR "gleisbilder/"

/* Namen fuer MS2 CAN Auswertung */
#define MrMs2Decode(CanMsg,CanFrame) MrCs2Decode(CanMsg,CanFrame)
#define MrMs2Encode(CanMsg,CanFrame) MrCs2Encode(CanMsg,CanFrame)
#define MrMs2DumpCanMsg MrCs2DumpCanMsg
#define MrMs2Trace(CanMsg) MrCs2Trace(CanMsg)
#define MrMs2CalcHash(Uid) MrCs2CalcHash(Uid)
#define MrMs2CalcHashFromFolgenummer(Folgenummer) MrCs2CalcHashFromFolgenummer(Folgenummer)

#define MrCs2GetNumParamBytes MrCs2GetDlc
#define MrMs2GetNumParamBytes MrCs2GetNumParamBytes


void MrCs2Decode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame);
void MrCs2Encode(MrCs2CanDataType *CanMsg, struct can_frame *CanFrame);
void MrCs2DumpCanMsg(MrCs2CanDataType *CanMsg, char *Info);
void MrCs2Trace(MrCs2CanDataType *CanMsg);
unsigned MrCs2CalcHash(unsigned long Uid);
unsigned MrCs2CalcHashFromFolgenummer(unsigned Folgenummer);
BOOL MrCs2IsCs2Msg(unsigned Hash, unsigned Command);
void MrCs2DecodeId(unsigned long CanId, unsigned *Hash, unsigned *Response,
                   unsigned *Command, unsigned *Prio);
unsigned long MrCs2EncodeId(unsigned short Hash, unsigned Response,
                            unsigned Command, unsigned Prio);
unsigned short MrCs2CalcCrc(unsigned char *data, unsigned int length);

/* decode functions for system subcmds */
void MrCs2DecSysStop(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysGo(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysHalt(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysEmergencyHalt(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysCycleEnd(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysDataProtocoll(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                              int *Proto);
void MrCs2DecSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Time);
void MrCs2DecSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *MfxSid);
void MrCs2DecSysTrackProtocoll(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                               int *Proto);
void MrCs2DecSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                                int *Counter);
void MrCs2DecSysOverload(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Channel);
void MrCs2DecSysStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel);
void MrCs2DecSysStatus7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Set);
void MrCs2DecSysStatus8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Channel, int *Value);
void MrCs2DecSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long *Uid);
void MrCs2DecSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                         int *Id);
void MrCs2DecSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                            int *ResteTarget);
/* encode functions for system subcmds */
void MrCs2EncSysStop(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysGo(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysHalt(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysEmergencyHalt(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysCycleEnd(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysDataProtocoll(MrCs2CanDataType *CanMsg, unsigned long Uid,
                              int Proto);
void MrCs2EncSysDecoderTimeing(MrCs2CanDataType *CanMsg, unsigned long Uid,
                               int Time);
void MrCs2EncSysMfxFastRead(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int MfxSid);
void MrCs2EncSysTrackProtocoll(MrCs2CanDataType *CanMsg, unsigned long Uid,
                               int Proto);
void MrCs2EncSysResetMfxCounter(MrCs2CanDataType *CanMsg, unsigned long Uid,
                                int Counter);
void MrCs2EncSysOverload(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Channel);
void MrCs2EncSysStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel);
void MrCs2EncSysStatus7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Set);
void MrCs2EncSysStatus8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Channel, int Value);
void MrCs2EncSysSystemId5(MrCs2CanDataType *CanMsg, unsigned long Uid);
void MrCs2EncSysSystemId7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                         int Id);
void MrCs2EncSysSystemReset(MrCs2CanDataType *CanMsg, unsigned long Uid,
                            int ResteTarget);
/* decode functions for cmds */
void MrCs2DecDiscovery0(MrCs2CanDataType *CanMsg);
void MrCs2DecDiscovery1(MrCs2CanDataType *CanMsg, int *Proto);
void MrCs2DecDiscovery5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range);
void MrCs2DecDiscovery6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        int *Range, int *Ask);
void MrCs2DecBind(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                  unsigned int *Sid);
void MrCs2DecMfxVerify6(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        unsigned int *Sid);
void MrCs2DecMfxVerify7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                        unsigned int *Sid, int *Ask);
void MrCs2DecSpeed4(MrCs2CanDataType *CanMsg, unsigned long *LocId);
void MrCs2DecSpeed6(MrCs2CanDataType *CanMsg, unsigned long *LocId, int *Speed);
void MrCs2DecDirection4(MrCs2CanDataType *CanMsg, unsigned long *LocId);
void MrCs2DecDirection5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Direction);
void MrCs2DecFunction5(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function);
void MrCs2DecFunction6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value);
void MrCs2DecFunction8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                       int *Function, int *Value, int *FktVal);
void MrCs2DecReadConfig6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex);
void MrCs2DecReadConfig7(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Param);
void MrCs2DecWriteConfig(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                         int *CvIndex, int *Value, int *CtrlRslt);
void MrCs2DecAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current);
void MrCs2DecAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long *LocId,
                        int *Position, int *Current, int *TimeFkt);
void MrCs2DecS88Poll5(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *NumModuls);
void MrCs2DecS88Poll7(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                      int *Modul, int *State);
void MrCs2DecS88Event4(MrCs2CanDataType *CanMsg, int *Device, int *Contact);
void MrCs2DecS88Event5(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *Param);
void MrCs2DecS88Event8(MrCs2CanDataType *CanMsg, int *Device, int *Contact,
                       int *StateOld, int *StateNew, int *Time);
void MrCs2DecPing0(MrCs2CanDataType *CanMsg);
void MrCs2DecPing8(MrCs2CanDataType *CanMsg, unsigned long *Uid,
                   int *SwVersion, int *DeviceId);
void MrCs2DecCanBootldr(MrCs2CanDataType *CanMsg, char *Bytes);
void MrCs2DecStatus5(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index);
void MrCs2DecStatus6(MrCs2CanDataType *CanMsg, unsigned long *Uid, int *Index,
                     int *NumPackets);
void MrCs2DecStatus8(MrCs2CanDataType *CanMsg, char *Bytes);
#define MrCs2DecConfigQuery0(CanMsg)
void MrCs2DecConfigQuery(MrCs2CanDataType *CanMsg, char *FileName);
void MrCs2DecCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc);
void MrCs2DecCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long *Length,
                           int *Crc);
void MrCs2DecCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes);
void MrCs2DecAutomatic6(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        int *Status, int *Param);
void MrCs2DecAutomatic8(MrCs2CanDataType *CanMsg, int *DeviceId, int *Func,
                        unsigned long *LocId);
/* encode functions for cmds */
void MrCs2EncDiscovery0(MrCs2CanDataType *CanMsg);
void MrCs2EncDiscovery1(MrCs2CanDataType *CanMsg, int Proto);
void MrCs2EncDiscovery5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range);
void MrCs2EncDiscovery6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        int Range, int Ask);
void MrCs2EncBind(MrCs2CanDataType *CanMsg, unsigned long Uid,
                  unsigned int Sid);
void MrCs2EncMfxVerify6(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid);
void MrCs2EncMfxVerify7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                        unsigned int Sid, int Ask);
void MrCs2EncSpeed4(MrCs2CanDataType *CanMsg, unsigned long LocId);
void MrCs2EncSpeed6(MrCs2CanDataType *CanMsg, unsigned long LocId, int Speed);
void MrCs2EncDirection4(MrCs2CanDataType *CanMsg, unsigned long LocId);
void MrCs2EncDirection5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Direction);
void MrCs2EncFunction5(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function);
void MrCs2EncFunction6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value);
void MrCs2EncFunction8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                       int Function, int Value, int FktVal);
void MrCs2EncReadConfig6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex);
void MrCs2EncReadConfig7(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Param);
void MrCs2EncWriteConfig(MrCs2CanDataType *CanMsg, unsigned long LocId,
                         int CvIndex, int Value, int CtrlRslt);
void MrCs2EncAccSwitch6(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current);
void MrCs2EncAccSwitch8(MrCs2CanDataType *CanMsg, unsigned long LocId,
                        int Position, int Current, int TimeFkt);
void MrCs2EncS88Poll5(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int NumModuls);
void MrCs2EncS88Poll7(MrCs2CanDataType *CanMsg, unsigned long Uid,
                      int Modul, int State);
void MrCs2EncS88Event4(MrCs2CanDataType *CanMsg, int Device, int Contact);
void MrCs2EncS88Event5(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int Param);
void MrCs2EncS88Event8(MrCs2CanDataType *CanMsg, int Device, int Contact,
                       int StateOld, int StateNew, int Time);
void MrCs2EncPing0(MrCs2CanDataType *CanMsg);
void MrCs2EncPing8(MrCs2CanDataType *CanMsg, unsigned long Uid,
                   int SwVersion, int DeviceId);
void MrCs2EncCanBootldr(MrCs2CanDataType *CanMsg, char *Bytes);
void MrCs2EncStatus5(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index);
void MrCs2EncStatus6(MrCs2CanDataType *CanMsg, unsigned long Uid, int Index,
                     int NumPackets);
void MrCs2EncStatus8(MrCs2CanDataType *CanMsg, char *Bytes);
void MrCs2EncConfigQuery0(MrCs2CanDataType *CanMsg);
void MrCs2EncConfigQuery(MrCs2CanDataType *CanMsg, char *FileName);
void MrCs2EncCfgdatStream6(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc);
void MrCs2EncCfgdatStream7(MrCs2CanDataType *CanMsg, unsigned long Length,
                           int Crc);
void MrCs2EncCfgdatStream8(MrCs2CanDataType *CanMsg, char *Bytes);
void MrCs2EncAutomatic6(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        int Status, int Param);
void MrCs2EncAutomatic8(MrCs2CanDataType *CanMsg, int DeviceId, int Func,
                        unsigned long LocId);

int MrCs2StatusInfoEncode(char *Buffer, int NumCfg, int NumStat,
                          unsigned long SerienNummer, char *ArtikelNummer,
                          char *ArtikelName);
void MrCs2StatusInfoDecode(char *Buffer, int *NumCfg, int *NumStat,
                           unsigned long *SerienNummer, char *ArtikelNummer,
                           char *ArtikelName);

/* andere Namen fuer MS2 */

/* own name for ms2 data type, but same struct than cs2 */
#define MrMs2CanDataType MrCs2CanDataType

/* andere Name fuer Zugriff auf ms2 data type fields */
#define MrMs2IsMs2           MrCs2IsCs2
#define MrMs2GetCommand      MrCs2GetCommand
#define MrMs2GetSystemSubcmd MrCs2GetSystemSubcmd
#define MrMs2SetCommand      MrCs2SetCommand
#define MrMs2SetSystemSubcmd MrCs2SetSystemSubcmd
#define MrMs2GetDlc          MrCs2GetDlc
#define MrMs2SetDlc          MrCs2SetDlc

#define MrMs2CalcCrc MrCs2CalcCrc

/* andere Namen fuer Auswertung MS2 CAN Id */
#define MR_MS2_SHIFT_HASH     MR_CS2_SHIFT_HASH
#define MR_MS2_SHIFT_RESP     MR_CS2_SHIFT_RESP
#define MR_MS2_SHIFT_COMMAND  MR_CS2_SHIFT_COMMAND
#define MR_MS2_SHIFT_PRIO     MR_CS2_SHIFT_PRIO

#define MR_MS2_MASK_HASH    MR_CS2_MASK_HASH
#define MR_MS2_MASK_RESP    MR_CS2_MASK_RESP
#define MR_MS2_MASK_COMMAND MR_CS2_MASK_COMMAND
#define MR_MS2_MASK_PRIO    MR_CS2_MASK_PRIO

#define MR_MS2_SHIFT_HASH_MSB  MR_CS2_SHIFT_HASH_MSB
#define MR_MS2_MASK_HASH_MAGIC MR_CS2_MASK_HASH_MAGIC
#define MR_MS2_HASH_MAGIC      MR_CS2_HASH_MAGIC
#define MR_MS2_MASK_HASH_LOW_BYTE  MR_CS2_MASK_HASH_LOW_BYTE
#define MR_MS2_MASK_HASH_HIGH_BYTE MR_CS2_MASK_HASH_HIGH_BYTE

/* andere Namen fuer prio Feld */
#define MR_MS2_PRIO_1 MR_CS2_PRIO_1
#define MR_MS2_PRIO_2 MR_CS2_PRIO_2
#define MR_MS2_PRIO_3 MR_CS2_PRIO_3
#define MR_MS2_PRIO_4 MR_CS2_PRIO_4

/* andere Namen fuer command field of MS2 can id */
#define MR_MS2_CMD_SYSTEM        MR_CS2_CMD_SYSTEM
#define MR_MS2_CMD_DISCOVERY     MR_CS2_CMD_DISCOVERY
#define MR_MS2_CMD_BIND          MR_CS2_CMD_BIND
#define MR_MS2_CMD_MFX_VERIFY    MR_CS2_CMD_MFX_VERIFY
#define MR_MS2_CMD_SPEED         MR_CS2_CMD_SPEED
#define MR_MS2_CMD_DIRECTION     MR_CS2_CMD_DIRECTION
#define MR_MS2_CMD_FUNCTION      MR_CS2_CMD_FUNCTION
#define MR_MS2_CMD_READ_CONFIG   MR_CS2_CMD_READ_CONFIG
#define MR_MS2_CMD_WRITE_CONFIG  MR_CS2_CMD_WRITE_CONFIG
#define MR_MS2_CMD_ACC_SWITCH    MR_CS2_CMD_ACC_SWITCH
#define MR_MS2_CMD_ACC_CONF      MR_CS2_CMD_ACC_CONF
#define MR_MS2_CMD_S88_POLL      MR_CS2_CMD_S88_POLL
#define MR_MS2_CMD_S88_EVENT     MR_CS2_CMD_S88_EVENT
#define MR_MS2_CMD_SX1_EVENT     MR_CS2_CMD_SX1_EVENT
#define MR_MS2_CMD_PING          MR_CS2_CMD_PING
#define MR_MS2_CMD_OFFER_UPDATE  MR_CS2_CMD_OFFER_UPDATE
#define MR_MS2_CMD_READ_CFG_DATA MR_CS2_CMD_READ_CFG_DATA
#define MR_MS2_CMD_BOOTLDR_CAN   MR_CS2_CMD_BOOTLDR_CAN
#define MR_MS2_CMD_BOOTLDR_TRACK MR_CS2_CMD_BOOTLDR_TRACK
#define MR_MS2_CMD_STATUS        MR_CS2_CMD_STATUS
#define MR_MS2_CMD_CONFIG_QUERY  MR_CS2_CMD_CONFIG_QUERY
#define MR_MS2_CMD_CFGDAT_STREAM MR_CS2_CMD_CFGDAT_STREAM
#define MR_MS2_CMD_CONNECT_6021  MR_CS2_CMD_CONNECT_6021
#define MR_MS2_CMD_AUTOMATI   1  MR_CS2_CMD_AUTOMATIC

/* andere Namen fuer sub commands of MS2 system command (data byte 4) */
#define MR_MS2_SUBCMD_SYSTEM_STOP              MR_CS2_SUBCMD_SYSTEM_STOP
#define MR_MS2_SUBCMD_SYSTEM_GO                MR_CS2_SUBCMD_SYSTEM_GO
#define MR_MS2_SUBCMD_SYSTEM_HALT              MR_CS2_SUBCMD_SYSTEM_HALT
#define MR_MS2_SUBCMD_SYSTEM_EMERGENCY_HALT    MR_CS2_SUBCMD_SYSTEM_EMERGENCY_HALT
#define MR_MS2_SUBCMD_SYSTEM_CYCLE_END         MR_CS2_SUBCMD_SYSTEM_CYCLE_END
#define MR_MS2_SUBCMD_SYSTEM_DATA_PROTOCOLL    MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL
#define MR_MS2_SUBCMD_SYSTEM_DECODER_TIMEING   MR_CS2_SUBCMD_SYSTEM_DECODER_TIMEING
#define MR_MS2_SUBCMD_SYSTEM_MFX_FAST_READ     MR_CS2_SUBCMD_SYSTEM_MFX_FAST_READ
#define MR_MS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL   MR_CS2_SUBCMD_SYSTEM_TRACK_PROTOCOLL
#define MR_MS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER MR_CS2_SUBCMD_SYSTEM_RESET_MFX_COUNTER
#define MR_MS2_SUBCMD_SYSTEM_OVERLOAD          MR_CS2_SUBCMD_SYSTEM_OVERLOAD
#define MR_MS2_SUBCMD_SYSTEM_STATUS            MR_CS2_SUBCMD_SYSTEM_STATUS
#define MR_MS2_SUBCMD_SYSTEM_SYSTEM_ID         MR_CS2_SUBCMD_SYSTEM_SYSTEM_ID
#define MR_MS2_SUBCMD_SYSTEM_MFX_SEEK          MR_CS2_SUBCMD_SYSTEM_MFX_SEEK
#define MR_MS2_SUBCMD_SYSTEM_SYSTEM_RESET      MR_CS2_SUBCMD_SYSTEM_SYSTEM_RESET

/* andere namen fuer Lok Datenprotokoll */
#define MR_MS2_DATA_PROTO_MM2_2040   0
#define MR_MS2_DATA_PROTO_MM2_LOK_20 1
#define MR_MS2_DATA_PROTO_MM2_FKT_40 2
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define MR_MS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define MR_MS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/* andere Namen fuer uids fue MS3 */
#define MR_MS2_UID_BROADCAST MR_CS2_UID_BROADCAST

/* andere name fuer Lok Datenprotokoll (MR_CS2_SUBCMD_SYSTEM_DATA_PROTOCOLL) */
#define MR_MS2_DATA_PROTO_MM2_2040   0
#define MR_MS2_DATA_PROTO_MM2_LOK_20 1
#define MR_MS2_DATA_PROTO_MM2_FKT_40 2
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_28  0
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_14  1
#define MR_MS2_DATA_PROTO_DCC_SHORT_ADDR_126 2
#define MR_MS2_DATA_PROTO_DCC_LONG_ADDR_28   3
#define MR_MS2_DATA_PROTO_DCC_LONG_ADDR_126  4

/* andere namen fuer track protocoll */
#define MR_MS2_TRACK_PROTO_MM2  0x01
#define MR_MS2_TRACK_PROTO_MFX  0x02
#define MR_MS2_TRACK_PROTO_DCC  0x04
#define MR_MS2_TRACK_PROTO_RES1 0x08
#define MR_MS2_TRACK_PROTO_RES2 0x10
#define MR_MS2_TRACK_PROTO_RES3 0x20
#define MR_MS2_TRACK_PROTO_RES4 0x40
#define MR_MS2_TRACK_PROTO_RES5 0x80

/* andere namen fuer range/protocll feld von lok discovery */
#define MR_MS2_DISCOVERY_MFX_1 MR_CS2_DISCOVERY_MFX_1
#define MR_MS2_DISCOVERY_MFX_2 MR_CS2_DISCOVERY_MFX_2
#define MR_MS2_DISCOVERY_MM2_1 MR_CS2_DISCOVERY_MM2_1
#define MR_MS2_DISCOVERY_MM2_2 MR_CS2_DISCOVERY_MM2_2
#define MR_MS2_DISCOVERY_DCC_1 MR_CS2_DISCOVERY_DCC_1
#define MR_MS2_DISCOVERY_DCC_2 MR_CS2_DISCOVERY_DCC_2
#define MR_MS2_DISCOVERY_DCC_3 MR_CS2_DISCOVERY_DCC_3
#define MR_MS2_DISCOVERY_SX1_1 MR_CS2_DISCOVERY_SX1_1
#define MR_MS2_DISCOVERY_SX1_2 MR_CS2_DISCOVERY_SX1_2
#define MR_MS2_DISCOVERY_MFX_3 MR_CS2_DISCOVERY_MFX_3
#define MR_MS2_DISCOVERY_MFX_4 MR_CS2_DISCOVERY_MFX_4
#define MR_MS2_DISCOVERY_MM2_3 MR_CS2_DISCOVERY_MM2_3
#define MR_MS2_DISCOVERY_DCC_4 MR_CS2_DISCOVERY_DCC_4
#define MR_MS2_DISCOVERY_DCC_5 MR_CS2_DISCOVERY_DCC_5
#define MR_MS2_DISCOVERY_SX1_3 MR_CS2_DISCOVERY_SX1_3
#define MR_MS2_DISCOVERY_SX1_4 MR_CS2_DISCOVERY_SX1_4

/* andere namen fuer cv address for SX1 */
#define MR_MS2_SX1_CV_ADDRESS      MR_CS2_SX1_CV_ADDRESS
#define MR_MS2_SX1_CV_MAXSPEED     MR_CS2_SX1_CV_MAXSPEED
#define MR_MS2_SX1_CV_ACCELERATION MR_CS2_SX1_CV_ACCELERATION
#define MR_MS2_SX1_CV_PULSEWIDTH   MR_CS2_SX1_CV_PULSEWIDTH
#define MR_MS2_SX1_CV_STOPPART     MR_CS2_SX1_CV_STOPPART

/* andere namen fuer value for position in switch acc */
#define MR_MS2_POSITION_RED    MR_CS2_POSITION_RED
#define MR_MS2_POSITION_GREEN  MR_CS2_POSITION_GREEN
#define MR_MS2_POSITION_YELLOW MR_CS2_POSITION_YELLOW
#define MR_MS2_POSITION_WHITE  MR_CS2_POSITION_WHITE

/* andere namen fuer device if for ping */
#define MR_MS2_DEVID_GFP      MR_CS2_DEVID_GFP
#define MR_MS2_DEVID_GLEISBOX MR_CS2_DEVID_GLEISBOX
#define MR_MS2_DEVID_CONNECT  MR_CS2_DEVID_CONNECT
#define MR_MS2_DEVID_MS2      MR_CS2_DEVID_MS2
#define MR_MS2_DEVID_CS2      MR_CS2_DEVID_CS2
#define MR_MS2_DEVID_WIRELESS MR_CS2_DEVID_WIRELESS
#define MR_MS2_DEVID_CS2GUI   MR_CS2_DEVID_CS2GUI

/* andere namen fuerr cfg data */
#define MR_MS2_CFG_LOCINFO  MR_CS2_CFG_LOCINFO
#define MR_MS2_CFG_LOCNAMES MR_CS2_CFG_LOCNAMES
#define MR_MS2_CFG_LOCDB    MR_CS2_CFG_LOCDB

/* andere Namen fuer decode funtions */
#define MrMs2DecSysStop            MrCs2DecSysStop
#define MrMs2DecSysGo              MrCs2DecSysGo
#define MrMs2DecSysHalt            MrCs2DecSysHalt
#define MrMs2DecSysEmergencyHalt   MrCs2DecSysEmergencyHalt
#define MrMs2DecSysCycleEnd        MrCs2DecSysCycleEnd
#define MrMs2DecSysDataProtocoll   MrCs2DecSysDataProtocoll
#define MrMs2DecSysDecoderTimeing  MrCs2DecSysDecoderTimeing
#define MrMs2DecSysMfxFastRead     MrCs2DecSysMfxFastRead
#define MrMs2DecSysTrackProtocoll  MrCs2DecSysTrackProtocoll
#define MrMs2DecSysResetMfxCounter MrCs2DecSysResetMfxCounter
#define MrMs2DecSysOverload        MrCs2DecSysOverload
#define MrMs2DecSysStatus6         MrCs2DecSysStatus6
#define MrMs2DecSysStatus7         MrCs2DecSysStatus7
#define MrMs2DecSysStatus8         MrCs2DecSysStatus8
#define MrMs2DecSysSystemId5       MrCs2DecSysSystemId5
#define MrMs2DecSysSystemId7       MrCs2DecSysSystemId7
#define MrMs2DecSysSystemReset     MrCs2DecSysSystemReset

#define MrMs2EncSysStop            MrCs2EncSysStop
#define MrMs2EncSysGo              MrCs2EncSysGo
#define MrMs2EncSysHalt            MrCs2EncSysHalt
#define MrMs2EncSysEmergencyHalt   MrCs2EncSysEmergencyHalt
#define MrMs2EncSysCycleEnd        MrCs2EncSysCycleEnd
#define MrMs2EncSysDataProtocoll   MrCs2EDSysDataProtocoll
#define MrMs2EncSysDecoderTimeing  MrCs2EnecSysDecoderTimeing
#define MrMs2EncSysMfxFastRead     MrCs2EncSysMfxFastRead
#define MrMs2EncSysTrackProtocoll  MrCs2EncSysTrackProtocoll
#define MrMs2EncSysResetMfxCounter MrCs2EncSysResetMfxCounter
#define MrMs2EncSysOverload        MrCs2EncSysOverload
#define MrMs2EncSysStatus6         MrCs2EncSysStatus6
#define MrMs2EncSysStatus7         MrCs2EncSysStatus7
#define MrMs2EncSysStatus8         MrCs2EncSysStatus8
#define MrMs2EncSysSystemId5       MrCs2EncSysSystemId5
#define MrMs2EncSysSystemId7       MrCs2EncSysSystemId7
#define MrMs2EncSysSystemReset     MrCs2EncSysSystemReset

#define MrMs2DecDiscovery0    MrCs2DecDiscovery0
#define MrMs2DecDiscovery1    MrCs2DecDiscovery1
#define MrMs2DecDiscovery5    MrCs2DecDiscovery5
#define MrMs2DecDiscovery6    MrCs2DecDiscovery6
#define MrMs2DecBind          MrCs2DecBind
#define MrMs2DecMfxVerify6    MrCs2DecMfxVerify6
#define MrMs2DecMfxVerify7    MrCs2DecMfxVerify7
#define MrMs2DecSpeed4        MrCs2DecSpeed4
#define MrMs2DecSpeed6        MrCs2DecSpeed6
#define MrMs2DecDirection4    MrCs2DecDirection4
#define MrMs2DecDirection5    MrCs2DecDirection5
#define MrMs2DecFunction5     MrCs2DecFunction5
#define MrMs2DecFunction6     MrCs2DecFunction6
#define MrMs2DecFunction8     MrCs2DecFunction8
#define MrMs2DecReadConfig6   MrCs2DecReadConfig6
#define MrMs2DecReadConfig7   MrCs2DecReadConfig7
#define MrMs2DecWriteConfig   MrCs2DecWriteConfig
#define MrMs2DecAccSwitch6    MrCs2DecAccSwitch6
#define MrMs2DecAccSwitch8    MrCs2DecAccSwitch8
#define MrMs2DecS88Poll5      MrCs2DecS88Poll5
#define MrMs2DecS88Poll7      MrCs2DecS88Poll7
#define MrMs2DecS88Event4     MrCs2DecS88Event4
#define MrMs2DecS88Event5     MrCs2DecS88Event5
#define MrMs2DecS88Event8     MrCs2DecS88Event8
#define MrMs2DecPing0         MrCs2DecPing0
#define MrMs2DecPing8         MrCs2DecPing8
#define MrMs2DecCanBootldr    MrCs2DecCanBootldr
#define MrMs2DecStatus5       MrCs2DecStatus5
#define MrMs2DecStatus6       MrCs2DecStatus6
#define MrMs2DecStatus8       MrCs2DecStatus8
#define MrMs2DecConfigQuery0  MrCs2DecConfigQuery0
#define MrMs2DecConfigQuery   MrCs2DecConfigQuery
#define MrMs2DecCfgdatStream6 MrCs2DecCfgdatStream6
#define MrMs2DecCfgdatStream7 MrCs2DecCfgdatStream7
#define MrMs2DecCfgdatStream8 MrCs2DecCfgdatStream8
#define MrMs2DecAutomatic6    MrCs2DecAutomatic6
#define MrMs2DecAutomatic8    MrCs2DecAutomatic8

#define MrMs2EncDiscovery0    MrCs2EncDiscovery0
#define MrMs2EncDiscovery1    MrCs2EncDiscovery1
#define MrMs2EncDiscovery5    MrCs2EncDiscovery5
#define MrMs2EncDiscovery6    MrCs2EncDiscovery6
#define MrMs2EncBind          MrCs2EncBind
#define MrMs2EncMfxVerify6    MrCs2EncMfxVerify6
#define MrMs2EncMfxVerify7    MrCs2EncMfxVerify7
#define MrMs2EncSpeed4        MrCs2EncSpeed4
#define MrMs2EncSpeed6        MrCs2EncSpeed6
#define MrMs2EncDirection4    MrCs2EncDirection4
#define MrMs2EncDirection5    MrCs2EncDirection5
#define MrMs2EncFunction5     MrCs2EncFunction5
#define MrMs2EncFunction6     MrCs2EncFunction6
#define MrMs2EncFunction8     MrCs2EncFunction8
#define MrMs2EncReadConfig6   MrCs2EncReadConfig6
#define MrMs2EncReadConfig7   MrCs2EncReadConfig7
#define MrMs2EncWriteConfig   MrCs2EncWriteConfig
#define MrMs2EncAccSwitch6    MrCs2EncAccSwitch6
#define MrMs2EncAccSwitch8    MrCs2EncAccSwitch8
#define MrMs2EncS88Poll5      MrCs2EncS88Poll5
#define MrMs2EncS88Poll7      MrCs2EncS88Poll7
#define MrMs2EncS88Event4     MrCs2EncS88Event4
#define MrMs2EncS88Event5     MrCs2EncS88Event5
#define MrMs2EncS88Event8     MrCs2EncS88Event8
#define MrMs2EncPing0         MrCs2EncPing0
#define MrMs2EncPing8         MrCs2EncPing8
#define MrMs2EncCanBootldr    MrCs2EncCanBootldr
#define MrMs2EncStatus5       MrCs2EncStatus5
#define MrMs2EncStatus6       MrCs2EncStatus6
#define MrMs2EncStatus8       MrCs2EncStatus8
#define MrMs2EncConfigQuery0  MrCs2EncConfigQuery0
#define MrMs2EncConfigQuery   MrCs2EncConfigQuery
#define MrMs2EncCfgdatStream6 MrCs2EncCfgdatStream6
#define MrMs2EncCfgdatStream7 MrCs2EncCfgdatStream7
#define MrMs2EncCfgdatStream8 MrCs2EncCfgdatStream8
#define MrMs2EncAutomatic6    MrCs2EncAutomatic6
#define MrMs2EncAutomatic8    MrCs2EncAutomatic8

#endif
