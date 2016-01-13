#!/bin/sh

cp BPi-CAN-F.Cu.gbr      BPi_CAN.GTL
cp BPi-CAN-B.Cu.gbr      BPi_CAN.GBL
cp BPi-CAN-F.Mask.gbr    BPi_CAN.GTS
cp BPi-CAN-B.Mask.gbr    BPi_CAN.GBS
cp BPi-CAN-F.SilkS.gbr   BPi_CAN.GTO
cp BPi-CAN-B.SilkS.gbr   BPi_CAN.GBO
cp BPi-CAN.drl           BPi_CAN-PTH.TXT
cp BPi-CAN-NPTH.drl      BPi_CAN-NPTH.TXT
cp BPi-CAN-Edge.Cuts.gbr BPi_CAN.GML

rm -f BPi_CAN.ZIP
zip BPi_CAN.ZIP BPi_CAN.GTL BPi_CAN.GBL BPi_CAN.GTS BPi_CAN.GBS BPi_CAN.GTO BPi_CAN.GBO BPi_CAN-PTH.TXT BPi_CAN-NPTH.TXT BPi_CAN.GML 
