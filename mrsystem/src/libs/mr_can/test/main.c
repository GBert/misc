#include <stdio.h>
#include <mr_can.h>

static void InitVars(unsigned long *sUid, unsigned int *sSid, int *s1, int *s2, int *s3, int *s4, int *s5,
                     unsigned long *eUid, unsigned int *eSid, int *e1, int *e2, int *e3, int *e4, int *e5)
{
   *sUid = *eUid = 4711;
   *sSid = *eSid = 42;
   *s1 = *e1 = 1;
   *s2 = *e2 = 2;
   *s3 = *e3 = 3;
   *s4 = *e4 = 4;
   *s5 = *e5 = 5;
}

static void TestVars(char *FktName,
                     unsigned long sUid, unsigned int sSid, int s1, int s2, int s3, int s4, int s5,
                     unsigned long eUid, unsigned int eSid, int e1, int e2, int e3, int e4, int e5)
{
   if ((sUid != eUid) ||
       (sSid != eSid) ||
       (s1 != e1) ||
       (s2 != e2) ||
       (s3 != e3) ||
       (s4 != e4) ||
       (s5 != e5))
   {
      printf("ERROR detected in %s\n", FktName);
      if (sUid != eUid)
         puts("sUid != eUid");
      if (sSid != eSid)
         puts("sSid != eSid");
      if (s1 != e1)
         puts("s1 != e1");
      if (s2 != e2)
         puts("s2 != e2");
      if (s3 != e3)
         puts("s3 != e3");
      if (s4 != e4)
         puts("s4 != e4");
      if (s5 != e5)
         puts("s5 != e5");
   }
}

int main(void)
{  MrMs2CanDataType CanMsg;
   int s1, s2, s3, s4, s5, e1, e2, e3, e4, e5;
   unsigned long sUid, eUid;
   unsigned int sSid, eSid;

   puts("start test");
   /* test functions for system subcmds */
   puts("test system subcmds");
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysStop(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysStop(&CanMsg, &eUid);
   TestVars("SysStop", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysGo(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysGo(&CanMsg, &eUid);
   TestVars("SysGo", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysHalt(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysHalt(&CanMsg, &eUid);
   TestVars("SysHalt", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysEmergencyHalt(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysEmergencyHalt(&CanMsg, &eUid);
   TestVars("SysEmergencyHalt", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysCycleEnd(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysCycleEnd(&CanMsg, &eUid);
   TestVars("SysCycleEnd", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysDataProtocoll(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysDataProtocoll(&CanMsg, &eUid, &e1);
   TestVars("SysDataProtocoll", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysDecoderTimeing(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysDecoderTimeing(&CanMsg, &eUid, &e1);
   TestVars("SysDecoderTimeing", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysMfxFastRead(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysMfxFastRead(&CanMsg, &eUid, &e1);
   TestVars("SysMfxFastRead", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysTrackProtocoll(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysTrackProtocoll(&CanMsg, &eUid, &e1);
   TestVars("SysTrackProtocoll", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysResetMfxCounter(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysResetMfxCounter(&CanMsg, &eUid, &e1);
   TestVars("SysResetMfxCounter", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysOverload(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysOverload(&CanMsg, &eUid, &e1);
   TestVars("SysOverload", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysStatus6(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysStatus6(&CanMsg, &eUid, &e1);
   TestVars("SysStatus6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysStatus7(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecSysStatus7(&CanMsg, &eUid, &e1, &e2);
   TestVars("SysStatus7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysStatus8(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecSysStatus8(&CanMsg, &eUid, &e1, &e2);
   TestVars("SysStatus8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysSystemId5(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSysSystemId5(&CanMsg, &eUid);
   TestVars("SysSystemId5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysSystemId7(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysSystemId7(&CanMsg, &eUid, &e1);
   TestVars("SysSystemId7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSysSystemReset(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSysSystemReset(&CanMsg, &eUid, &e1);
   TestVars("SysSystemReset", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   /* encode functions for cmds */
   puts("test cmds");
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDiscovery0(&CanMsg);
   MrCs2DecDiscovery0(&CanMsg);
   TestVars("Discovery0", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDiscovery1(&CanMsg, s1);
   e1 = 0;
   MrCs2DecDiscovery1(&CanMsg, &e1);
   TestVars("Discovery1", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDiscovery5(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecDiscovery5(&CanMsg, &eUid, &e1);
   TestVars("Discovery5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDiscovery6(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecDiscovery6(&CanMsg, &eUid, &e1, &e2);
   TestVars("Discovery6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncBind(&CanMsg, sUid, sSid);
   eUid = 0;
   eSid = 0;
   MrCs2DecBind(&CanMsg, &eUid, &eSid);
   TestVars("Bind", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncMfxVerify6(&CanMsg, sUid, sSid);
   eUid = 0;
   eSid = 0;
   MrCs2DecMfxVerify6(&CanMsg, &eUid, &eSid);
   TestVars("MfxVerify6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncMfxVerify7(&CanMsg, sUid, sSid, s1);
   eUid = 0;
   eSid = 0;
   e1 = 0;
   MrCs2DecMfxVerify7(&CanMsg, &eUid, &eSid, &e1);
   TestVars("MfxVerify7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSpeed4(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecSpeed4(&CanMsg, &eUid);
   TestVars("Speed4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncSpeed6(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecSpeed6(&CanMsg, &eUid, &e1);
   TestVars("Speed6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDirection4(&CanMsg, sUid);
   eUid = 0;
   MrCs2DecDirection4(&CanMsg, &eUid);
   TestVars("Direction4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncDirection5(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecDirection5(&CanMsg, &eUid, &e1);
   TestVars("Direction5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncFunction5(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecFunction5(&CanMsg, &eUid, &e1);
   TestVars("Function5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncFunction6(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecFunction6(&CanMsg, &eUid, &e1, &e2);
   TestVars("Function6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncFunction8(&CanMsg, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   MrCs2DecFunction8(&CanMsg, &eUid, &e1, &e2, &e3);
   TestVars("Function8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncReadConfig6(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecReadConfig6(&CanMsg, &eUid, &e1);
   TestVars("ReadConfig6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncReadConfig7(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecReadConfig7(&CanMsg, &eUid, &e1);
   TestVars("ReadConfig7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncWriteConfig(&CanMsg, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   MrCs2DecWriteConfig(&CanMsg, &eUid, &e1, &e2, &e3);
   TestVars("WriteConfig", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncAccSwitch6(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecAccSwitch6(&CanMsg, &eUid, &e1, &e2);
   TestVars("AccSwitch6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncAccSwitch8(&CanMsg, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   MrCs2DecAccSwitch8(&CanMsg, &eUid, &e1, &e2, &e3);
   TestVars("AccSwitch8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncS88Poll5(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecS88Poll5(&CanMsg, &eUid, &e1);
   TestVars("S88Poll5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncS88Poll7(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecS88Poll7(&CanMsg, &eUid, &e1, &e2);
   TestVars("S88Poll7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncS88Event4(&CanMsg, s1, s2);
   e1 = 0;
   e2 = 0;
   MrCs2DecS88Event4(&CanMsg, &e1, &e2);
   TestVars("S88Event4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncS88Event5(&CanMsg, s1, s2, s3);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   MrCs2DecS88Event5(&CanMsg, &e1, &e2, &e3);
   TestVars("S88Event5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncS88Event8(&CanMsg, s1, s2, s3, s4, s5);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   e4 = 0;
   e5 = 0;
   MrCs2DecS88Event8(&CanMsg, &e1, &e2, &e3, &e4, &e5);
   TestVars("S88Event8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncPing0(&CanMsg);
   MrCs2DecPing0(&CanMsg);
   TestVars("Ping0", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncPing8(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecPing8(&CanMsg, &eUid, &e1, &e2);
   TestVars("Ping8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncStatus5(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecStatus5(&CanMsg, &eUid, &e1);
   TestVars("Status5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncStatus6(&CanMsg, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecStatus6(&CanMsg, &eUid, &e1, &e2);
   TestVars("Status6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#if 0
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncConfigQuery(&CanMsg, char *FileName);
   MrCs2DecConfigQuery(&CanMsg, char *FileName);
   TestVars("ConfigQuery", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#endif
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncCfgdatStream6(&CanMsg, sUid, s1);
   eUid = 0;
   e1 = 0;
   MrCs2DecCfgdatStream6(&CanMsg, &eUid, &e1);
   TestVars("CfgdatStream6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#if 0
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncCfgdatStream7(&CanMsg, sUid, s1, char *Byte6);
   eUid = 0;
   e1 = 0;
   MrCs2DecCfgdatStream7(&CanMsg, &eUid, &e1, char *Byte6);
   TestVars("CfgdatStream7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncCfgdatStream8(&CanMsg, char *Bytes);
   MrCs2DecCfgdatStream8(&CanMsg, char *Bytes);
   TestVars("CfgdatStream8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#endif
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncAutomatic6(&CanMsg, s1, s2, s3, s4);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   e4 = 0;
   MrCs2DecAutomatic6(&CanMsg, &e1, &e2, &e3, &e4);
   TestVars("Automatic6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   MrCs2EncAutomatic8(&CanMsg, s1, s2, sUid);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   MrCs2DecAutomatic8(&CanMsg, &e1, &e2, &eUid);
   TestVars("Automatic8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   /* ende */
   puts("end test");
   return(0);
}
