#ifndef SRCP_H
#define SRCP_H

/**
* @mainpage srcp
*
* Encoder/Decoder f&uuml;r das SRCP (Simple Railroad Command Protocol) Protokoll
*
* @author Michael Bernstein
*/

#include <boolean.h>

/** @file */

#ifdef __cplusplus
extern "C" {
#endif

/* SRCP V7 */

#define SRCP_V7_TARGET_GL_GENERIC_LOCO      1
#define SRCP_V7_TARGET_GA_GENERIC_ACCESSORY 2
#define SRCP_V7_TARGET_FB_FEEDBACK          3
#define SRCP_V7_TARGET_TIME                 4
#define SRCP_V7_TARGET_POWER                5
#define SRCP_V7_TARGET_UNKNOWN              6
#define SRCP_V7_TARGET_ERROR                7

#define SRCP_V7_ERRNO_ERROR_NOT_SUPPORTED  -1
#define SRCP_V7_ERRNO_ERROR_NO_INFORMATION -2
#define SRCP_V7_ERRNO_ERROR_TIMEOUT        -3

#define SRCP_V7_STR_ANTWORT_INFO "INFO %d"

typedef struct {
   union {
      struct {
         char Protocol[10];
         char Type[5];
         int Addr;
         int Value;
         int Direction;
         int V;
         int VMax;
         int NumF;
         int f[16];
      } GlParams;
      struct {
         char Protocol[10];
         char Type[5];
         int Addr;
         int Port;
         int Value;
         int Delay;
      } GaParams;
      struct {
         char ModuleType[5];
         int Addr;
         int Value;
         int Timeout;
      } FbParams;
      struct {
         int JulDay;
         int Hour;
         int Minute;
         int Second;
         int Fx;
         int Fy;
      } TimeParams;
      struct {
         BOOL PowerOn;
      } PowerParams;
   } TargetParms;
} SrcpV7ParamType;

int SrcpV7EncAnswer(char *Buf, int Error);
int SrcpV7DecSet(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecGet(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecInit(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecWait(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecTerm(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecVerify(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecWrite(char *Line, int Length, SrcpV7ParamType *Params);
int SrcpV7DecRead(char *Line, int Length, SrcpV7ParamType *Params);


/* SRCP > V7 */

#define SRCP_HANDSHAKE_OK           0
#define SRCP_HANDSHAKE_MODE_INFO    1
#define SRCP_HANDSHAKE_MODE_COMMAND 2
#define SRCP_HANDSHAKE_ERROR        3

#define SRCP_CMD_ERROR   0
#define SRCP_CMD_SET     1
#define SRCP_CMD_GET     2
#define SRCP_CMD_WAIT    3
#define SRCP_CMD_INIT    4
#define SRCP_CMD_TERM    5
#define SRCP_CMD_VERIFY  6
#define SRCP_CMD_RESET   7
#define SRCP_CMD_GO      8
#define SRCP_CMD_WRITE   9
#define SRCP_CMD_READ   10

#define SRCP_V8_DEVICE_GROUP_GL_GENERIC_LOCO      1
#define SRCP_V8_DEVICE_GROUP_GA_GENERIC_ACCESSORY 2
#define SRCP_V8_DEVICE_GROUP_FB_FEEDBACK_SENSOR   3
#define SRCP_V8_DEVICE_GROUP_TIME                 4
#define SRCP_V8_DEVICE_GROUP_POWER                5
#define SRCP_V8_DEVICE_GROUP_UNKNOWN              6
#define SRCP_V8_DEVICE_GROUP_ERROR                7
#define SRCP_V8_DEVICE_GROUP_GM_GENERIC_MESSAGE   8
#define SRCP_V8_DEVICE_GROUP_LOCK                 9
#define SRCP_V8_DEVICE_GROUP_SERVER               10
#define SRCP_V8_DEVICE_GROUP_SESSION              11
#define SRCP_V8_DEVICE_GROUP_SM_SERVICE_MODE      12
#define SRCP_V8_DEVICE_GROUP_DESCRIPTION          13

#define SRCP_ANTWORT_HANDSHAKE_GO_OK       0
#define SRCP_ANTWORT_HANDSHAKE_PROTO_OK    1
#define SRCP_ANTWORT_HANDSHAKE_CONMODE_OK  2
#define SRCP_ANTWORT_HANDSHAKE_PROTO_ERR   3
#define SRCP_ANTWORT_HANDSHAKE_CONMODE_ERR 4
#define SRCP_ANTWORT_HANDSHAKE_INSUFFD_ERR 5
#define SRCP_ANTWORT_HANDSHAKE_NORES_ERR   6

#define SRCP_ANTWORT_COMMAND_INFO_OK     0
#define SRCP_ANTWORT_COMMAND_INFOINIT_OK 1
#define SRCP_ANTWORT_COMMAND_INFOTERM_OK 2

#define SRCP_ANTWORT_COMMAND_OK           0
#define SRCP_ANTWORT_COMMAND_UNKNOWN_ERR  1
#define SRCP_ANTWORT_COMMAND_UNVAL_ERR    2
#define SRCP_ANTWORT_COMMAND_WRVAL_ERR    3
#define SRCP_ANTWORT_COMMAND_DEVLOCK      4
#define SRCP_ANTWORT_COMMAND_FORBID_ERR   5
#define SRCP_ANTWORT_COMMAND_NODATA_ERR   6
#define SRCP_ANTWORT_COMMAND_TIMEOUT      7
#define SRCP_ANTWORT_COMMAND_TOLONG_ERR   8
#define SRCP_ANTWORT_COMMAND_TOSHORT_ERR  9
#define SRCP_ANTWORT_COMMAND_UNPROT_ERR  10
#define SRCP_ANTWORT_COMMAND_UNDEV_ERR   11
#define SRCP_ANTWORT_COMMAND_UNGRP_ERR   12
#define SRCP_ANTWORT_COMMAND_UNOP_ERR    13
#define SRCP_ANTWORT_COMMAND_REINIT_ERR  14
#define SRCP_ANTWORT_COMMAND_UNSUPP_ERR  15
#define SRCP_ANTWORT_COMMAND_ERR         16

#define SRCP_ERRNO_ANTWORT_HANDSHAKE_GO_OK       200
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_PROTO_OK    201
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_CONMODE_OK  202
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_PROTO_ERR   400
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_CONMODE_ERR 401
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_INSUFFD_ERR 402
#define SRCP_ERRNO_ANTWORT_HANDSHAKE_NORES_ERR   500
#define SRCP_ERRNO_ANTWORT_COMMAND_INFO_OK     100
#define SRCP_ERRNO_ANTWORT_COMMAND_INFOINIT_OK 101
#define SRCP_ERRNO_ANTWORT_COMMAND_INFOTERM_OK 102
#define SRCP_ERRNO_ANTWORT_COMMAND_OK          200
#define SRCP_ERRNO_ANTWORT_COMMAND_UNKNOWN_ERR 410
#define SRCP_ERRNO_ANTWORT_COMMAND_UNVAL_ERR   411
#define SRCP_ERRNO_ANTWORT_COMMAND_WRVAL_ERR   412
#define SRCP_ERRNO_ANTWORT_COMMAND_DEVLOCK     414
#define SRCP_ERRNO_ANTWORT_COMMAND_FORBID_ERR  415
#define SRCP_ERRNO_ANTWORT_COMMAND_NODATA_ERR  416
#define SRCP_ERRNO_ANTWORT_COMMAND_TIMEOUT     417
#define SRCP_ERRNO_ANTWORT_COMMAND_TOLONG_ERR  418
#define SRCP_ERRNO_ANTWORT_COMMAND_TOSHORT_ERR 419
#define SRCP_ERRNO_ANTWORT_COMMAND_UNPROT_ERR  420
#define SRCP_ERRNO_ANTWORT_COMMAND_UNDEV_ERR   421
#define SRCP_ERRNO_ANTWORT_COMMAND_UNGRP_ERR   422
#define SRCP_ERRNO_ANTWORT_COMMAND_UNOP_ERR    423
#define SRCP_ERRNO_ANTWORT_COMMAND_REINIT_ERR  424
#define SRCP_ERRNO_ANTWORT_COMMAND_UNSUPP_ERR  425
#define SRCP_ERRNO_ANTWORT_COMMAND_ERR         499

#define SRCP_STR_ANTWORT_HANDSHAKE_GO_OK       "200 OK GO %s"
#define SRCP_STR_ANTWORT_HANDSHAKE_PROTO_OK    "201 OK PROTOCOL SRCP"
#define SRCP_STR_ANTWORT_HANDSHAKE_CONMODE_OK  "202 OK CONNECTIONMODE"
#define SRCP_STR_ANTWORT_HANDSHAKE_PROTO_ERR   "400 ERROR unsupported protocol"
#define SRCP_STR_ANTWORT_HANDSHAKE_CONMODE_ERR "401 ERROR unsupported connection mode"
#define SRCP_STR_ANTWORT_HANDSHAKE_INSUFFD_ERR "402 ERROR insufficient data"
#define SRCP_STR_ANTWORT_HANDSHAKE_NORES_ERR   "500 ERROR out of resources"
#define SRCP_STR_ANTWORT_COMMAND_INFO_OK     "100 INFO %s"
#define SRCP_STR_ANTWORT_COMMAND_INFOINIT_OK "101 INFO %s"
#define SRCP_STR_ANTWORT_COMMAND_INFOTERM_OK "102 INFO %s"
#define SRCP_STR_ANTWORT_COMMAND_OK          "200 OK"
#define SRCP_STR_ANTWORT_COMMAND_UNKNOWN_ERR "410 ERROR unknown command"
#define SRCP_STR_ANTWORT_COMMAND_UNVAL_ERR   "411 ERROR unknown value"
#define SRCP_STR_ANTWORT_COMMAND_WRVAL_ERR   "412 ERROR wrong value"
#define SRCP_STR_ANTWORT_COMMAND_DEVLOCK     "414 ERROR device locked"
#define SRCP_STR_ANTWORT_COMMAND_FORBID_ERR  "415 ERROR forbidden"
#define SRCP_STR_ANTWORT_COMMAND_NODATA_ERR  "416 ERROR no data"
#define SRCP_STR_ANTWORT_COMMAND_TIMEOUT     "417 ERROR timeout"
#define SRCP_STR_ANTWORT_COMMAND_TOLONG_ERR  "418 ERROR list too long"
#define SRCP_STR_ANTWORT_COMMAND_TOSHORT_ERR "419 ERROR list too short"
#define SRCP_STR_ANTWORT_COMMAND_UNPROT_ERR  "420 ERROR unsupported device protocol"
#define SRCP_STR_ANTWORT_COMMAND_UNDEV_ERR   "421 ERROR unsupported device"
#define SRCP_STR_ANTWORT_COMMAND_UNGRP_ERR   "422 ERROR unsupported device group"
#define SRCP_STR_ANTWORT_COMMAND_UNOP_ERR    "423 ERROR unsupported operation"
#define SRCP_STR_ANTWORT_COMMAND_REINIT_ERR  "424 ERROR device reinitialized"
#define SRCP_STR_ANTWORT_COMMAND_UNSUPP_ERR  "425 ERROR not supported"
#define SRCP_STR_ANTWORT_COMMAND_ERR         "499 ERROR unspecified error"


typedef struct {
   int Bus;
   union {
      struct {
         int Addr;
         int DriveMode;
         int V;
         int VMax;
         int NumF;
         int f[16];
      } GlParams;
      struct {
         int Addr;
         int Port;
         int Value;
         int Delay;
      } GaParams;
      struct {
         int Addr;
         int Value;
         int Timeout;
      } FbParams;
      struct {
         int SendTo;
         int ReplyTo;
      } GmParams;
      struct {
         int DecoderAddress;
      } SmParams;
      struct {
         int JulDay;
         int Hour;
         int Minute;
         int Second;
      } TimeParams;
      struct {
         int DeviceGroup;
         int Addr;
         int Duration;
      } LockParams;
      struct {
         BOOL PowerOn;
      } PowerParams;
      struct {
         int SessionId;
      } SessionParams;
   } DeviceGroupParms;
} SrcpV8ParamType;


int SrcpEncAnswerHandshake(char *Buf, int Answer, char *Param);
#define SrcpEncAnswerHandshakeGoOk(Buf,Param)       SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_GO_OK,Param)
#define SrcpEncAnswerHandshakeProtoOk(Buf,Param)    SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_PROTO_OK,(char *)NULL)
#define SrcpEncAnswerHandshakeConmodeOk(Buf,Param)  SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_CONMODE_OK,(char *)NULL)
#define SrcpEncAnswerHandshakeProtoErr(Buf,Param)   SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_PROTO_ERR,(char *)NULL)
#define SrcpEncAnswerHandshakeConmodeErr(Buf,Param) SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_CONMODE_ERR,(char *)NULL)
#define SrcpEncAnswerHandshakeInsuffdErr(Buf,Param) SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_INSUFFD_ERR(char *)NULL)
#define SrcpEncAnswerHandshakeNoresErr(Buf,Param)   SrcpEncAnswerHandshake(Buf,SRCP_ANTWORT_HANDSHAKE_NORES_ERR,(char *)NULL)

int SrcpEncAnswerCommandInfo(char *Buf, int Answer, char *Param);
#define SrcpEncAnswerCommandInfoOk(Buf,Param)     SrcpEncAnswerCommandInfo(Buf,SRCP_ANTWORT_COMMAND_INFO_OK,Param)
#define SrcpEncAnswerCommandInfoinitOk(Buf,Param) SrcpEncAnswerCommandInfo(Buf,SRCP_ANTWORT_COMMAND_INFOINIT_OK,Param)
#define SrcpEncAnswerCommandInfotermOk(Buf,Param) SrcpEncAnswerCommandInfo(Buf,SRCP_ANTWORT_COMMAND_INFOTERM_OK,Param)

int SrcpEncAnswerCommand(char *Buf, int Answer);
#define SrcpEncAnswerCommandOk(Buf)         SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_OK)
#define SrcpEncAnswerCommandUnknownErr(Buf) SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNKNOWN_ERR)
#define SrcpEncAnswerCommandUnvalErr(Buf)   SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNVAL_ERR)
#define SrcpEncAnswerCommandWrvalErr(Buf)   SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_WRVAL_ERR)
#define SrcpEncAnswerCommandDevlock(Buf)    SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_DEVLOCK)
#define SrcpEncAnswerCommandForbidErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_FORBID_ERR)
#define SrcpEncAnswerCommandNodataErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_NODATA_ERR)
#define SrcpEncAnswerCommandTimeout(Buf)    SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_TIMEOUT)
#define SrcpEncAnswerCommandTolongErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_TOLONG_ERR)
#define SrcpEncAnswerCommandToshortErr(Buf) SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_TOSHORT_ERR)
#define SrcpEncAnswerCommandUnprotErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNPROT_ERR)
#define SrcpEncAnswerCommandUndevErr(Buf)   SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNDEV_ERR)
#define SrcpEncAnswerCommandUngrpErr(Buf)   SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNGRP_ERR)
#define SrcpEncAnswerCommandUnopErr(Buf)    SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNOP_ERR)
#define SrcpEncAnswerCommandReinitErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_REINIT_ERR)
#define SrcpEncAnswerCommandUnsuppErr(Buf)  SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_UNSUPP_ERR)
#define SrcpEncAnswerCommandErr(Buf)        SrcpEncAnswerCommand(Buf,SRCP_ANTWORT_COMMAND_ERR)

int SrcpDecHandshake(char *Line, int Length);
int SrcpDecGetCmd(char *Line, int Length);

int SrcpV8DecCheck(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecGet(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecWait(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecInit(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecReset(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecSet(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecTerm(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecVerify(char *Line, int Length, SrcpV8ParamType *Params);
int SrcpV8DecWait(char *Line, int Length, SrcpV8ParamType *Params);

#ifdef __cplusplus
}
#endif

#endif
