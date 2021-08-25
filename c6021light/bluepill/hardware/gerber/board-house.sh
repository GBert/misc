#!/bin/sh

BOARD='c6021light'

cp ${BOARD}-F_Cu.gtl      ${BOARD}.GTL
cp ${BOARD}-B_Cu.gbl      ${BOARD}.GBL
cp ${BOARD}-F_Mask.gts    ${BOARD}.GTS
cp ${BOARD}-B_Mask.gbs    ${BOARD}.GBS
cp ${BOARD}-F_SilkS.gto   ${BOARD}.GTO
cp ${BOARD}-B_SilkS.gbo   ${BOARD}.GBO
cp ${BOARD}-PTH.drl           ${BOARD}-PTH.TXT
cp ${BOARD}-Edge_Cuts.gm1 ${BOARD}.GML

if [ -f ${BOARD}-NPTH.drl ]; then
    cp ${BOARD}-NPTH.drl      ${BOARD}-NPTH.TXT
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}-NPTH.TXT ${BOARD}.GML 
else
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}.GML 
fi
