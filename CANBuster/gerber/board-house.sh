#!/bin/sh

BOARD='CANBuster'

cp ${BOARD}-F.Cu.gbr      ${BOARD}.GTL
cp ${BOARD}-B.Cu.gbr      ${BOARD}.GBL
cp ${BOARD}-F.Mask.gbr    ${BOARD}.GTS
cp ${BOARD}-B.Mask.gbr    ${BOARD}.GBS
cp ${BOARD}-F.SilkS.gbr   ${BOARD}.GTO
cp ${BOARD}-B.SilkS.gbr   ${BOARD}.GBO
cp ${BOARD}.drl           ${BOARD}-PTH.TXT
cp ${BOARD}-NPTH.drl      ${BOARD}-NPTH.TXT
cp ${BOARD}-Edge.Cuts.gbr ${BOARD}.GML

rm -f ${BOARD}.ZIP
zip ${BOARD}.ZIP ${BOARD}.GTL ${BOARD}.GBL ${BOARD}.GTS ${BOARD}.GBS ${BOARD}.GTO ${BOARD}.GBO ${BOARD}-PTH.TXT ${BOARD}-NPTH.TXT ${BOARD}.GML 
