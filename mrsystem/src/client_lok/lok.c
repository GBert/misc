#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <map.h>
#include <mr_ipc.h>
#include <cs2parse.h>
#include <write_cs2.h>
#include <cs2.h>
#include "lok.h"

#define MRLOK_CFG_NAME "/etc/mrlok"
#define SELECT_TIMEOUT 100

static BOOL Loop = TRUE;

BOOL GetLokInfo(LokStruct *Data, FILE *CfgFile)
{  char Zeile[256], *p;
   BOOL Ret;

   if (fgets(Zeile, sizeof(Zeile), CfgFile) != (char *)NULL)
   {
      p = strchr(Zeile, ',');
      if (p != (char *)NULL)
      {
         *p = '\0';
         LokSetSpecialLokAddr(Data, atoi(Zeile));
         p++;
         LokSetSpecialLokName(Data, p);
         if (LokGetSpecialLokName(Data)[strlen(LokGetSpecialLokName(Data)) - 1] == '\n')
         {
            LokGetSpecialLokName(Data)[strlen(LokGetSpecialLokName(Data)) - 1] = '\0';
         }
         Ret = TRUE;
      }
      else
      {
         Ret = FALSE;
      }
   }
   else
   {
      Ret = FALSE;
   }
   return(Ret);
}

void GetActionInfo(LokStruct *Data, FILE *CfgFile)
{  char Zeile[256], *Cmd, *p;
   int FktNr;

   if (fgets(Zeile, sizeof(Zeile), CfgFile) != (char *)NULL)
   {
      if (Zeile[0] == 'F')
      {
         p = strchr(Zeile, ',');
         if (p != (char *)NULL)
         {
            *p = '\0';
            FktNr = atoi(Zeile + 1);
            p++;
            Cmd = strdup(p);
            if (Cmd[strlen(Cmd) - 1] == '\n')
            {
               Cmd[strlen(Cmd) - 1] = '\0';
            }
            MapSet(LokGetSpecialActions(Data),
                   (MapKeyType)FktNr, (MapDataType)Cmd);
         }
      }
   }
}

BOOL LoadMrLokCfg(LokStruct *Data)
{  FILE *CfgFile;
   BOOL Ret;

   CfgFile = fopen(MRLOK_CFG_NAME, "r");
   if (CfgFile != (FILE *)NULL)
   {
      Ret = GetLokInfo(Data, CfgFile);
      if (Ret)
      {
         while (!feof(CfgFile))
         {
            GetActionInfo(Data, CfgFile);
         }
      }
      fclose(CfgFile);
   }
   else
   {
      Ret = FALSE;
   }
   return(Ret);
}

static int fktcmp(void *d1, void *d2)
{
   return((int)d1 - (int)d2);
}

LokStruct *LokCreate(void)
{  LokStruct *Data;

   Data = (LokStruct *)malloc(sizeof(LokStruct));
   if (Data != (LokStruct *)NULL)
   {
      LokSetVerbose(Data, FALSE);
      LokSetInterface(Data, (char *)NULL);
      LokSetServerPort(Data, -1);
      LokSetClientSock(Data, -1);
      LokSetLocPath(Data, "/var/www/config/");
      LokSetSpecialActions(Data, MapCreate());
      if (LokGetSpecialActions(Data) != (Map *)NULL)
      {
         MapInit(LokGetSpecialActions(Data), (CmpFkt)fktcmp,
                 (MapKeyDelCbFkt)NULL, (MapDataDelCbFkt)free);
         if (!LoadMrLokCfg(Data))
         {
            MapDestroy(LokGetSpecialActions(Data));
            free(Data);
            Data = (LokStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         Data = (LokStruct *)NULL;
      }
   }
   return(Data);
}

void LokDestroy(LokStruct *Data)
{
   if (LokGetVerbose(Data))
      puts("destroy lok");
   MapDestroy(LokGetSpecialActions(Data));
   free(Data);
}

BOOL ParseLokomotiveCs2(LokStruct *Data, char *Buf, int Len)
{  Cs2parser *LokParser;
   int LineInfo;
   int LokAddr;
   char LokName[17];
   BOOL Ret;

   Ret = FALSE;
   LokParser = Cs2pCreate();
   Cs2pInit(LokParser, PARSER_TYPE_LOK_CS2, Buf, Len);
   Cs2pSetVerbose(LokParser, FALSE);
   do {
      LineInfo = Cs2pParse(LokParser);
      switch (LineInfo)
      {
         case PARSER_ERROR:
            break;
         case PARSER_EOF:
            Ret = Ret || ((LokGetSpecialLokAddr(Data) == LokAddr) &&
                          (strcmp(LokGetSpecialLokName(Data), LokName) == 0));
            break;
         case PARSER_PARAGRAPH:
            switch (Cs2pGetSubType(LokParser))
            {
               default:
                  break;
            }
            break;
         case PARSER_VALUE:
            switch (Cs2pGetSubType(LokParser))
            {
               case PARSER_VALUE_LOKOMOTIVE:
                  Ret = Ret || ((LokGetSpecialLokAddr(Data) == LokAddr) &&
                                (strcmp(LokGetSpecialLokName(Data), LokName) == 0));
                  break;
               case PARSER_VALUE_NAME:
                  strcpy(LokName, Cs2pGetValue(LokParser));
                  break;
               case PARSER_VALUE_ADRESSE:
                  LokAddr = strtoul(Cs2pGetValue(LokParser), NULL, 0);
                  break;
               default:
                  break;
            }
            break;
      }
   } while ((LineInfo != PARSER_EOF) && (LineInfo != PARSER_ERROR));
   Cs2pExit(LokParser);
   Cs2pDestroy(LokParser);
   return(Ret);
}

BOOL CheckSpecialLok(LokStruct *Data)
{  FILE *LokCs2Stream;
   struct stat attribut;
   char *LokFileName, *LokFileContent;
   BOOL Ret;

   Ret = FALSE;
   LokFileName = (char *)malloc(strlen(LokGetLocPath(Data)) + 
                                strlen(CS2_FILE_STRING_LOKOMOTIVE) + 2);
   if (LokFileName != (char *)NULL)
   {
      strcpy(LokFileName, LokGetLocPath(Data));
      if (LokFileName[strlen(LokFileName) - 1] != '/')
         strcat(LokFileName, "/");
      strcat(LokFileName, CS2_FILE_STRING_LOKOMOTIVE);
      if (stat(LokFileName, &attribut) == 0)
      {
         if (attribut.st_size > 0)
         {
            LokFileContent = (char *)malloc(attribut.st_size);
            if (LokFileContent != (char *)NULL)
            {
               LokCs2Stream = fopen(LokFileName, "r");
               if (LokCs2Stream != NULL)
               {
                  fread(LokFileContent, 1, attribut.st_size, LokCs2Stream);
                  Ret = ParseLokomotiveCs2(Data, LokFileContent, 
                                           attribut.st_size);
                  Cs2Close(LokCs2Stream);
               }
               free(LokFileContent);
            }
         }
      }
      free(LokFileName);
   }
   return(Ret);
}

void LokInit(LokStruct *Data, BOOL Verbose, char *Interface, char *Addr,
             int Port, char *LocPath)
{
   LokSetVerbose(Data, Verbose);
   LokSetInterface(Data, Interface);
   LokSetAddress(Data, Addr);
   LokSetServerPort(Data, Port);
   LokSetClientSock(Data, -1);
   LokSetLocPath(Data, LocPath);
   LokSetHaveSpecialLok(Data, CheckSpecialLok(Data));
}

static void SigHandler(int sig)
{
   Loop = FALSE;
}

static BOOL Start(LokStruct *Data)
{  struct sigaction SigStruct;

   if ((strlen(LokGetInterface(Data)) > 0) &&
       ((strlen(LokGetAddress(Data)) == 0) ||
        (strcmp(LokGetAddress(Data), "0.0.0.0") == 0)))
   {
      LokSetClientSock(Data,
                       MrIpcConnectIf(LokGetInterface(Data),
                                      LokGetServerPort(Data)));
   }
   else
   {
      LokSetClientSock(Data,
                       MrIpcConnect(LokGetAddress(Data),
                                    LokGetServerPort(Data)));
   }
   if (LokGetClientSock(Data) >= 0)
   {
      if (LokGetVerbose(Data))
         puts("ready for incoming comands");
      SigStruct.sa_handler = SigHandler;
      sigemptyset(&SigStruct.sa_mask);
      SigStruct.sa_flags = 0;
      sigaction(SIGINT, &SigStruct, NULL);
      sigaction(SIGQUIT, &SigStruct, NULL);
      sigaction(SIGTERM, &SigStruct, NULL);
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}

static void Stop(LokStruct *Data)
{
   if (LokGetVerbose(Data))
      puts("stop network client");
   if (LokGetClientSock(Data) >= 0)
   {
      MrIpcClose(LokGetClientSock(Data));
   }
}

static void ProcessSystemData(LokStruct *Data, MrIpcCmdType *CmdFrame)
{  unsigned int FktNr;
   unsigned long Adr;
   SwitchType Switch;
   time_t Now;
   char *Cmd;

   Now = time(NULL);
   if (LokGetVerbose(Data))
      printf("%s> command frame %d\n",
             asctime(localtime(&Now)), MrIpcGetCommand(CmdFrame));
   switch (MrIpcGetCommand(CmdFrame))
   {
      case MrIpcCmdLocomotiveFunction:
         MrIpcCmdGetLocomotiveFkt(CmdFrame, &Adr, &FktNr, &Switch);
         if (LokGetVerbose(Data))
            printf("lokfunktion %d val %d\n", FktNr, Switch);
         switch (Switch)
         {
            case Off:
               break;
            case On:
               if (LokGetHaveSpecialLok(Data) &&
                   (LokGetSpecialLokAddr(Data) == Adr))
               {
                  Cmd = (char *)MapGet(LokGetSpecialActions(Data), (MapKeyType)FktNr);
                  if (Cmd != (char *)NULL)
                  {
                     if (LokGetVerbose(Data))
                        printf("start action nr %ld\n", Adr);
                     system(Cmd);
                  }
               }
               break;
         }
         break;
      default:
         break;
   }
}

static void HandleSystemData(LokStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (LokGetVerbose(Data))
      puts("new data available");
   do {
      MrIpcInit(&CmdFrame);
      RcvReturnValue = MrIpcRecv(LokGetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         if (LokGetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         if (LokGetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         if (LokGetVerbose(Data))
            puts("read new comand frame from socket");
         ProcessSystemData(Data, &CmdFrame);
         MrIpcExit(&CmdFrame);
      }
   } while (RcvReturnValue > 0);
}

void LokRun(LokStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         FD_ZERO(&ReadFds);
         HighFd = 0;
         if (LokGetVerbose(Data))
            printf("add client socket %d\n", LokGetClientSock(Data));
         FD_SET(LokGetClientSock(Data), &ReadFds);
         if (LokGetClientSock(Data) > HighFd)
            HighFd = LokGetClientSock(Data);
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (LokGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (LokGetVerbose(Data))
            printf("select liefert %d\n", RetVal);
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            Now = time(NULL);
            if (LokGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            if (LokGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            if (FD_ISSET(LokGetClientSock(Data), &ReadFds))
            {
               HandleSystemData(Data);
            }
         }
      }
      Stop(Data);
   }
}
