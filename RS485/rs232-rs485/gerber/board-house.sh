#!/bin/sh

set -x
BOARD='rs232-rs485'

cp ${BOARD}-F_Cu.gtl      ${BOARD}.GTL
cp ${BOARD}-B_Cu.gbl      ${BOARD}.GBL
cp ${BOARD}-F_Mask.gts    ${BOARD}.GTS
cp ${BOARD}-B_Mask.gbs    ${BOARD}.GBS
cp ${BOARD}-F_SilkS.gto   ${BOARD}.GTO
cp ${BOARD}-B_SilkS.gbo   ${BOARD}.GBO
cp ${BOARD}-PTH.drl           ${BOARD}-PTH.TXT
cp ${BOARD}-Edge_Cuts.gm1 ${BOARD}.GM1

if [ -f ${BOARD}-NPTH.drl ]; then
    cp ${BOARD}-NPTH.drl      ${BOARD}-NPTH.TXT
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}-NPTH.TXT ${BOARD}.GM1 
else
    rm -f ${BOARD}.ZIP
    zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}.GM1 
fi
