#!/bin/sh

BOARD='mfx-link'

cp ${BOARD}-F.Cu.gtl      ${BOARD}.GTL
cp ${BOARD}-B.Cu.gbl      ${BOARD}.GBL
cp ${BOARD}-F.Mask.gts    ${BOARD}.GTS
cp ${BOARD}-B.Mask.gbs    ${BOARD}.GBS
cp ${BOARD}-F.SilkS.gto   ${BOARD}.GTO
cp ${BOARD}-B.SilkS.gbo   ${BOARD}.GBO
cp ${BOARD}.drl           ${BOARD}-PTH.TXT
# cp ${BOARD}-NPTH.drl      ${BOARD}-NPTH.TXT
cp ${BOARD}-Edge.Cuts.gm1 ${BOARD}.GML

rm -f ${BOARD}.ZIP
# zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}-NPTH.TXT ${BOARD}.GML 
zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}.GML 
