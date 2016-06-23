#!/bin/sh

cp BPI-AddOn-F.Cu.gtl      BPI-AddOn.GTL
cp BPI-AddOn-B.Cu.gbl      BPI-AddOn.GBL
cp BPI-AddOn-F.Mask.gts    BPI-AddOn.GTS
cp BPI-AddOn-B.Mask.gbs    BPI-AddOn.GBS
cp BPI-AddOn-F.SilkS.gto   BPI-AddOn.GTO
cp BPI-AddOn-B.SilkS.gbo   BPI-AddOn.GBO
cp BPI-AddOn.drl           BPI-AddOn-PTH.TXT
cp BPI-AddOn-NPTH.drl      BPI-AddOn-NPTH.TXT
cp BPI-AddOn-Edge.Cuts.gm1 BPI-AddOn.GML

rm -f BPI-AddOn.ZIP
zip BPI-AddOn.ZIP BPI-AddOn.GTL BPI-AddOn.GBL BPI-AddOn.GTS BPI-AddOn.GBS BPI-AddOn.GTO BPI-AddOn.GBO BPI-AddOn-PTH.TXT BPI-AddOn-NPTH.TXT BPI-AddOn.GML 
