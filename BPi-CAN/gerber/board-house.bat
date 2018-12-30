
copy distributor_v2_only-F.Cu.gtl      distributor_v2_only.GTL
copy distributor_v2_only-B.Cu.gbl      distributor_v2_only.GBL
copy distributor_v2_only-F.Mask.gts    distributor_v2_only.GTS
copy distributor_v2_only-B.Mask.gbs    distributor_v2_only.GBS
copy distributor_v2_only-F.SilkS.gto   distributor_v2_only.GTO
copy distributor_v2_only-B.SilkS.gbo   distributor_v2_only.GBO
copy distributor_v2_only-PTH.drl       distributor_v2_only-PTH.TXT
copy distributor_v2_only-NPTH.drl      distributor_v2_only-NPTH.TXT
copy distributor_v2_only-Edge.Cuts.gm1 distributor_v2_only.GML
copy distributor_v2_only-Margin.gbr    distributor_v2_only.GBR

del distributor_v2-original.zip
del distributor_v2-renamed.zip

"c:\Program Files\7-Zip\7z.exe" a -- distributor_v2-renamed.zip distributor_v2_only.GTL distributor_v2_only.GBL distributor_v2_only.GTS distributor_v2_only.GBS distributor_v2_only.GTO distributor_v2_only.GBO distributor_v2_only-PTH.TXT distributor_v2_only-NPTH.TXT distributor_v2_only.GML distributor_v2_only.GBR

"c:\Program Files\7-Zip\7z.exe" a -- distributor_v2-original.zip distributor_v2_only-F.Cu.gtl distributor_v2_only-B.Cu.gbl distributor_v2_only-F.Mask.gts distributor_v2_only-B.Mask.gbs distributor_v2_only-F.SilkS.gto distributor_v2_only-B.SilkS.gbo distributor_v2_only-PTH.txt distributor_v2_only-NPTH.txt distributor_v2_only-Edge.Cuts.gm1 distributor_v2_only-Margin.gbr