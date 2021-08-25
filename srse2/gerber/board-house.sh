#!/bin/sh

BOARD='SRSE2'

cp ${BOARD}-F.Cu.gtl      ${BOARD}.GTL
cp ${BOARD}-B.Cu.gbl      ${BOARD}.GBL
cp ${BOARD}-F.Mask.gts    ${BOARD}.GTS
cp ${BOARD}-B.Mask.gbs    ${BOARD}.GBS
cp ${BOARD}-F.SilkS.gto   ${BOARD}.GTO
cp ${BOARD}-B.SilkS.gbo   ${BOARD}.GBO
cp ${BOARD}.drl           ${BOARD}-PTH.TXT
cp ${BOARD}-Edge.Cuts.gm1 ${BOARD}.GML

if [ -f ${BOARD}-NPTH.drl ]; then
    cp ${BOARD}-NPTH.drl      ${BOARD}-NPTH.TXT
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}-NPTH.TXT ${BOARD}.GML 
else
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}.GML 
fi
