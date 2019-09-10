#include <stdio.h>
#include <cs2.h>

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
{  unsigned char CanData[CS2_NUM_CAN_BYTES];
   int s1, s2, s3, s4, s5, e1, e2, e3, e4, e5;
   unsigned long sUid, eUid;
   unsigned int sSid, eSid;

   puts("start test");
   /* test functions for system subcmds */
   puts("test system subcmds");
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysStop(CanData, sUid);
   eUid = 0;
   Cs2DecSysStop(CanData, &eUid);
   TestVars("SysStop", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysGo(CanData, sUid);
   eUid = 0;
   Cs2DecSysGo(CanData, &eUid);
   TestVars("SysGo", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysHalt(CanData, sUid);
   eUid = 0;
   Cs2DecSysHalt(CanData, &eUid);
   TestVars("SysHalt", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysEmergencyHalt(CanData, sUid);
   eUid = 0;
   Cs2DecSysEmergencyHalt(CanData, &eUid);
   TestVars("SysEmergencyHalt", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysCycleEnd(CanData, sUid);
   eUid = 0;
   Cs2DecSysCycleEnd(CanData, &eUid);
   TestVars("SysCycleEnd", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysDataProtocoll(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysDataProtocoll(CanData, &eUid, &e1);
   TestVars("SysDataProtocoll", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysDecoderTimeing(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysDecoderTimeing(CanData, &eUid, &e1);
   TestVars("SysDecoderTimeing", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysMfxFastRead(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysMfxFastRead(CanData, &eUid, &e1);
   TestVars("SysMfxFastRead", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysTrackProtocoll(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysTrackProtocoll(CanData, &eUid, &e1);
   TestVars("SysTrackProtocoll", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysResetMfxCounter(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysResetMfxCounter(CanData, &eUid, &e1);
   TestVars("SysResetMfxCounter", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysOverload(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysOverload(CanData, &eUid, &e1);
   TestVars("SysOverload", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysStatus6(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysStatus6(CanData, &eUid, &e1);
   TestVars("SysStatus6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysStatus7(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecSysStatus7(CanData, &eUid, &e1, &e2);
   TestVars("SysStatus7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysStatus8(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecSysStatus8(CanData, &eUid, &e1, &e2);
   TestVars("SysStatus8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysSystemId5(CanData, sUid);
   eUid = 0;
   Cs2DecSysSystemId5(CanData, &eUid);
   TestVars("SysSystemId5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysSystemId7(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysSystemId7(CanData, &eUid, &e1);
   TestVars("SysSystemId7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSysSystemReset(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSysSystemReset(CanData, &eUid, &e1);
   TestVars("SysSystemReset", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   /* encode functions for cmds */
   puts("test cmds");
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDiscovery0(CanData);
   Cs2DecDiscovery0(CanData);
   TestVars("Discovery0", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDiscovery1(CanData, s1);
   e1 = 0;
   Cs2DecDiscovery1(CanData, &e1);
   TestVars("Discovery1", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDiscovery5(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecDiscovery5(CanData, &eUid, &e1);
   TestVars("Discovery5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDiscovery6(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecDiscovery6(CanData, &eUid, &e1, &e2);
   TestVars("Discovery6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncBind(CanData, sUid, sSid);
   eUid = 0;
   eSid = 0;
   Cs2DecBind(CanData, &eUid, &eSid);
   TestVars("Bind", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncMfxVerify6(CanData, sUid, sSid);
   eUid = 0;
   eSid = 0;
   Cs2DecMfxVerify6(CanData, &eUid, &eSid);
   TestVars("MfxVerify6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncMfxVerify7(CanData, sUid, sSid, s1);
   eUid = 0;
   eSid = 0;
   e1 = 0;
   Cs2DecMfxVerify7(CanData, &eUid, &eSid, &e1);
   TestVars("MfxVerify7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSpeed4(CanData, sUid);
   eUid = 0;
   Cs2DecSpeed4(CanData, &eUid);
   TestVars("Speed4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncSpeed6(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecSpeed6(CanData, &eUid, &e1);
   TestVars("Speed6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDirection4(CanData, sUid);
   eUid = 0;
   Cs2DecDirection4(CanData, &eUid);
   TestVars("Direction4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncDirection5(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecDirection5(CanData, &eUid, &e1);
   TestVars("Direction5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncFunction5(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecFunction5(CanData, &eUid, &e1);
   TestVars("Function5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncFunction6(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecFunction6(CanData, &eUid, &e1, &e2);
   TestVars("Function6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncFunction8(CanData, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   Cs2DecFunction8(CanData, &eUid, &e1, &e2, &e3);
   TestVars("Function8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncReadConfig6(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecReadConfig6(CanData, &eUid, &e1);
   TestVars("ReadConfig6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncReadConfig7(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecReadConfig7(CanData, &eUid, &e1, &e2);
   TestVars("ReadConfig7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncWriteConfig(CanData, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   Cs2DecWriteConfig(CanData, &eUid, &e1, &e2, &e3);
   TestVars("WriteConfig", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncAccSwitch6(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecAccSwitch6(CanData, &eUid, &e1, &e2);
   TestVars("AccSwitch6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncAccSwitch8(CanData, sUid, s1, s2, s3);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   e3 = 0;
   Cs2DecAccSwitch8(CanData, &eUid, &e1, &e2, &e3);
   TestVars("AccSwitch8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncS88Poll5(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecS88Poll5(CanData, &eUid, &e1);
   TestVars("S88Poll5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncS88Poll7(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecS88Poll7(CanData, &eUid, &e1, &e2);
   TestVars("S88Poll7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncS88Event4(CanData, s1, s2);
   e1 = 0;
   e2 = 0;
   Cs2DecS88Event4(CanData, &e1, &e2);
   TestVars("S88Event4", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncS88Event5(CanData, s1, s2, s3);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   Cs2DecS88Event5(CanData, &e1, &e2, &e3);
   TestVars("S88Event5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncS88Event8(CanData, s1, s2, s3, s4, s5);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   e4 = 0;
   e5 = 0;
   Cs2DecS88Event8(CanData, &e1, &e2, &e3, &e4, &e5);
   TestVars("S88Event8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncPing0(CanData);
   Cs2DecPing0(CanData);
   TestVars("Ping0", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncPing8(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecPing8(CanData, &eUid, &e1, &e2);
   TestVars("Ping8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncStatus5(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecStatus5(CanData, &eUid, &e1);
   TestVars("Status5", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncStatus6(CanData, sUid, s1, s2);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecStatus6(CanData, &eUid, &e1, &e2);
   TestVars("Status6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#if 0
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncConfigQuery(CanData, char *FileName);
   Cs2DecConfigQuery(CanData, char *FileName);
   TestVars("ConfigQuery", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#endif
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncCfgdatStream6(CanData, sUid, s1);
   eUid = 0;
   e1 = 0;
   Cs2DecCfgdatStream6(CanData, &eUid, &e1);
   TestVars("CfgdatStream6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#if 0
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncCfgdatStream7(CanData, sUid, s1, char *Byte6);
   eUid = 0;
   e1 = 0;
   Cs2DecCfgdatStream7(CanData, &eUid, &e1, char *Byte6);
   TestVars("CfgdatStream7", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncCfgdatStream8(CanData, char *Bytes);
   Cs2DecCfgdatStream8(CanData, char *Bytes);
   TestVars("CfgdatStream8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
#endif
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncAutomatic6(CanData, s1, s2, s3, s4);
   e1 = 0;
   e2 = 0;
   e3 = 0;
   e4 = 0;
   Cs2DecAutomatic6(CanData, &e1, &e2, &e3, &e4);
   TestVars("Automatic6", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   InitVars(&sUid, &sSid, &s1, &s2, &s3, &s4, &s5, &eUid, &eSid, &e1, &e2, &e3, &e4, &e5);
   Cs2EncAutomatic8(CanData, s1, s2, sUid);
   eUid = 0;
   e1 = 0;
   e2 = 0;
   Cs2DecAutomatic8(CanData, &e1, &e2, &eUid);
   TestVars("Automatic8", sUid, sSid, s1, s2, s3, s4, s5, eUid, eSid, e1, e2, e3, e4, e5);
   /* ende */
   puts("end test");
   return(0);
}
