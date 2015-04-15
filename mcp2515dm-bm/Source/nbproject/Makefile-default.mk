#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=user/BusMon.c user/SPI2515.c main.c system/usb/usbmmap.c system/interrupt/interrupt.c user/REGS2515.asm system/usb/usbdrv/usbdrv.c autofiles/usbdsc.c system/usb/usb9/usb9.c system/usb/usbctrltrf/usbctrltrf.c system/usb/class/hid/hid.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/user/BusMon.o ${OBJECTDIR}/user/SPI2515.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system/usb/usbmmap.o ${OBJECTDIR}/system/interrupt/interrupt.o ${OBJECTDIR}/user/REGS2515.o ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o ${OBJECTDIR}/autofiles/usbdsc.o ${OBJECTDIR}/system/usb/usb9/usb9.o ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o ${OBJECTDIR}/system/usb/class/hid/hid.o
POSSIBLE_DEPFILES=${OBJECTDIR}/user/BusMon.o.d ${OBJECTDIR}/user/SPI2515.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/system/usb/usbmmap.o.d ${OBJECTDIR}/system/interrupt/interrupt.o.d ${OBJECTDIR}/user/REGS2515.o.d ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o.d ${OBJECTDIR}/autofiles/usbdsc.o.d ${OBJECTDIR}/system/usb/usb9/usb9.o.d ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o.d ${OBJECTDIR}/system/usb/class/hid/hid.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/user/BusMon.o ${OBJECTDIR}/user/SPI2515.o ${OBJECTDIR}/main.o ${OBJECTDIR}/system/usb/usbmmap.o ${OBJECTDIR}/system/interrupt/interrupt.o ${OBJECTDIR}/user/REGS2515.o ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o ${OBJECTDIR}/autofiles/usbdsc.o ${OBJECTDIR}/system/usb/usb9/usb9.o ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o ${OBJECTDIR}/system/usb/class/hid/hid.o

# Source Files
SOURCEFILES=user/BusMon.c user/SPI2515.c main.c system/usb/usbmmap.c system/interrupt/interrupt.c user/REGS2515.asm system/usb/usbdrv/usbdrv.c autofiles/usbdsc.c system/usb/usb9/usb9.c system/usb/usbctrltrf/usbctrltrf.c system/usb/class/hid/hid.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4550
MP_PROCESSOR_OPTION_LD=18f4550
MP_LINKER_DEBUG_OPTION=-r=ROM@0x7DC0:0x7FFF -r=RAM@GPR:0x3F4:0x3FF -u_DEBUGSTACK
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/user/REGS2515.o: user/REGS2515.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/REGS2515.o.d 
	@${RM} ${OBJECTDIR}/user/REGS2515.o 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/user/REGS2515.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -d__DEBUG -d__MPLAB_DEBUGGER_PK3=1 -q -p$(MP_PROCESSOR_OPTION)  -l\"${OBJECTDIR}/user/REGS2515.lst\" -e\"${OBJECTDIR}/user/REGS2515.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/user/REGS2515.o\" user/REGS2515.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/user/REGS2515.o 
	@${FIXDEPS} "${OBJECTDIR}/user/REGS2515.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/user/REGS2515.o: user/REGS2515.asm  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/REGS2515.o.d 
	@${RM} ${OBJECTDIR}/user/REGS2515.o 
	@${FIXDEPS} dummy.d -e "${OBJECTDIR}/user/REGS2515.err" $(SILENT) -c ${MP_AS} $(MP_EXTRA_AS_PRE) -q -p$(MP_PROCESSOR_OPTION)  -l\"${OBJECTDIR}/user/REGS2515.lst\" -e\"${OBJECTDIR}/user/REGS2515.err\" $(ASM_OPTIONS)  -o\"${OBJECTDIR}/user/REGS2515.o\" user/REGS2515.asm 
	@${DEP_GEN} -d ${OBJECTDIR}/user/REGS2515.o 
	@${FIXDEPS} "${OBJECTDIR}/user/REGS2515.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/user/BusMon.o: user/BusMon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/BusMon.o.d 
	@${RM} ${OBJECTDIR}/user/BusMon.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/user/BusMon.o   user/BusMon.c 
	@${DEP_GEN} -d ${OBJECTDIR}/user/BusMon.o 
	@${FIXDEPS} "${OBJECTDIR}/user/BusMon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/user/SPI2515.o: user/SPI2515.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/SPI2515.o.d 
	@${RM} ${OBJECTDIR}/user/SPI2515.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/user/SPI2515.o   user/SPI2515.c 
	@${DEP_GEN} -d ${OBJECTDIR}/user/SPI2515.o 
	@${FIXDEPS} "${OBJECTDIR}/user/SPI2515.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbmmap.o: system/usb/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb 
	@${RM} ${OBJECTDIR}/system/usb/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbmmap.o   system/usb/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/interrupt/interrupt.o: system/interrupt/interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/interrupt 
	@${RM} ${OBJECTDIR}/system/interrupt/interrupt.o.d 
	@${RM} ${OBJECTDIR}/system/interrupt/interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/interrupt/interrupt.o   system/interrupt/interrupt.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/interrupt/interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/system/interrupt/interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbdrv/usbdrv.o: system/usb/usbdrv/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usbdrv 
	@${RM} ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o   system/usb/usbdrv/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbdrv/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/autofiles/usbdsc.o: autofiles/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/autofiles 
	@${RM} ${OBJECTDIR}/autofiles/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/autofiles/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/autofiles/usbdsc.o   autofiles/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/autofiles/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/autofiles/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usb9/usb9.o: system/usb/usb9/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usb9 
	@${RM} ${OBJECTDIR}/system/usb/usb9/usb9.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usb9/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usb9/usb9.o   system/usb/usb9/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usb9/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usb9/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o: system/usb/usbctrltrf/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usbctrltrf 
	@${RM} ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o   system/usb/usbctrltrf/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/class/hid/hid.o: system/usb/class/hid/hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/class/hid 
	@${RM} ${OBJECTDIR}/system/usb/class/hid/hid.o.d 
	@${RM} ${OBJECTDIR}/system/usb/class/hid/hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/class/hid/hid.o   system/usb/class/hid/hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/class/hid/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/class/hid/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/user/BusMon.o: user/BusMon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/BusMon.o.d 
	@${RM} ${OBJECTDIR}/user/BusMon.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/user/BusMon.o   user/BusMon.c 
	@${DEP_GEN} -d ${OBJECTDIR}/user/BusMon.o 
	@${FIXDEPS} "${OBJECTDIR}/user/BusMon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/user/SPI2515.o: user/SPI2515.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/user 
	@${RM} ${OBJECTDIR}/user/SPI2515.o.d 
	@${RM} ${OBJECTDIR}/user/SPI2515.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/user/SPI2515.o   user/SPI2515.c 
	@${DEP_GEN} -d ${OBJECTDIR}/user/SPI2515.o 
	@${FIXDEPS} "${OBJECTDIR}/user/SPI2515.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/main.o   main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbmmap.o: system/usb/usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb 
	@${RM} ${OBJECTDIR}/system/usb/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbmmap.o   system/usb/usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/interrupt/interrupt.o: system/interrupt/interrupt.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/interrupt 
	@${RM} ${OBJECTDIR}/system/interrupt/interrupt.o.d 
	@${RM} ${OBJECTDIR}/system/interrupt/interrupt.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/interrupt/interrupt.o   system/interrupt/interrupt.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/interrupt/interrupt.o 
	@${FIXDEPS} "${OBJECTDIR}/system/interrupt/interrupt.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbdrv/usbdrv.o: system/usb/usbdrv/usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usbdrv 
	@${RM} ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o   system/usb/usbdrv/usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbdrv/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbdrv/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/autofiles/usbdsc.o: autofiles/usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/autofiles 
	@${RM} ${OBJECTDIR}/autofiles/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/autofiles/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/autofiles/usbdsc.o   autofiles/usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/autofiles/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/autofiles/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usb9/usb9.o: system/usb/usb9/usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usb9 
	@${RM} ${OBJECTDIR}/system/usb/usb9/usb9.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usb9/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usb9/usb9.o   system/usb/usb9/usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usb9/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usb9/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o: system/usb/usbctrltrf/usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/usbctrltrf 
	@${RM} ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o   system/usb/usbctrltrf/usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/usbctrltrf/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/system/usb/class/hid/hid.o: system/usb/class/hid/hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/system/usb/class/hid 
	@${RM} ${OBJECTDIR}/system/usb/class/hid/hid.o.d 
	@${RM} ${OBJECTDIR}/system/usb/class/hid/hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/system/usb/class/hid/hid.o   system/usb/class/hid/hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/system/usb/class/hid/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/system/usb/class/hid/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    18f4550.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f4550.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   18f4550.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "18f4550.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MCP2515_BusMonitor.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
