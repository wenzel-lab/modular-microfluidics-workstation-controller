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
ifeq "$(wildcard nbproject/Makefile-local-PICkit3.mk)" "nbproject/Makefile-local-PICkit3.mk"
include nbproject/Makefile-local-PICkit3.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PICkit3
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/reset.c mcc_generated_files/system.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/spi2.c mcc_generated_files/spi1.c mcc_generated_files/tmr1.c mcc_generated_files/spi3.c mcc_generated_files/i2c2.c mcc_generated_files/uart1.c main.c spi.c ads1115.c eeprom.c storage.c pca9544a.c sensirion_lg16.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/spi3.o ${OBJECTDIR}/mcc_generated_files/i2c2.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/ads1115.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/storage.o ${OBJECTDIR}/pca9544a.o ${OBJECTDIR}/sensirion_lg16.o
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/reset.o.d ${OBJECTDIR}/mcc_generated_files/system.o.d ${OBJECTDIR}/mcc_generated_files/traps.o.d ${OBJECTDIR}/mcc_generated_files/clock.o.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d ${OBJECTDIR}/mcc_generated_files/mcc.o.d ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d ${OBJECTDIR}/mcc_generated_files/spi2.o.d ${OBJECTDIR}/mcc_generated_files/spi1.o.d ${OBJECTDIR}/mcc_generated_files/tmr1.o.d ${OBJECTDIR}/mcc_generated_files/spi3.o.d ${OBJECTDIR}/mcc_generated_files/i2c2.o.d ${OBJECTDIR}/mcc_generated_files/uart1.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/spi.o.d ${OBJECTDIR}/ads1115.o.d ${OBJECTDIR}/eeprom.o.d ${OBJECTDIR}/storage.o.d ${OBJECTDIR}/pca9544a.o.d ${OBJECTDIR}/sensirion_lg16.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/reset.o ${OBJECTDIR}/mcc_generated_files/system.o ${OBJECTDIR}/mcc_generated_files/traps.o ${OBJECTDIR}/mcc_generated_files/clock.o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o ${OBJECTDIR}/mcc_generated_files/mcc.o ${OBJECTDIR}/mcc_generated_files/pin_manager.o ${OBJECTDIR}/mcc_generated_files/spi2.o ${OBJECTDIR}/mcc_generated_files/spi1.o ${OBJECTDIR}/mcc_generated_files/tmr1.o ${OBJECTDIR}/mcc_generated_files/spi3.o ${OBJECTDIR}/mcc_generated_files/i2c2.o ${OBJECTDIR}/mcc_generated_files/uart1.o ${OBJECTDIR}/main.o ${OBJECTDIR}/spi.o ${OBJECTDIR}/ads1115.o ${OBJECTDIR}/eeprom.o ${OBJECTDIR}/storage.o ${OBJECTDIR}/pca9544a.o ${OBJECTDIR}/sensirion_lg16.o

# Source Files
SOURCEFILES=mcc_generated_files/reset.c mcc_generated_files/system.c mcc_generated_files/traps.c mcc_generated_files/clock.c mcc_generated_files/interrupt_manager.c mcc_generated_files/mcc.c mcc_generated_files/pin_manager.c mcc_generated_files/spi2.c mcc_generated_files/spi1.c mcc_generated_files/tmr1.c mcc_generated_files/spi3.c mcc_generated_files/i2c2.c mcc_generated_files/uart1.c main.c spi.c ads1115.c eeprom.c storage.c pca9544a.c sensirion_lg16.c



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
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-PICkit3.mk ${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CK256MP502
MP_LINKER_FILE_OPTION=,--script=p33CK256MP502.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/PICkit3/b608dde71b9535b0317ae75a4e687a71361a9fb1 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/PICkit3/f87c70eea09ca2ad201314fbc0aa1b62fcb3f21b .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/PICkit3/55303e4df83e011bee383f78c02671af3ac3884 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/PICkit3/9a7d2af44a021991714e386630a8cc8e727ba552 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/PICkit3/52718732b0b65e49d227abecd7649db83f365b05 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/PICkit3/19c1eeace595250beb2655975e453f07a2c5e69d .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/PICkit3/e09c6b7f786a3ec6ceb59cf37ea073a6704a4d7b .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/PICkit3/afec9424b6a97fa4a71adf7c95bed9ee57d8b510 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/PICkit3/31dbfe0770ab243a7f5f8d5974ee214a38f5e727 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/PICkit3/6a33fcb93fe879a7e965c3a91dc3ff23fdfe56e .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/PICkit3/6b4214d35dd1410a0f332ec72fd92d17ac0e6c7 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c2.o: mcc_generated_files/i2c2.c  .generated_files/flags/PICkit3/4252775b3139e6c97f291efb412d8e06c3177577 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c2.c  -o ${OBJECTDIR}/mcc_generated_files/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c2.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/PICkit3/74b3a2c5caf03665b72e61e44e3a57f1c84401f3 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/PICkit3/22d095d8731aa8931bf6d8bdce5a83d0535e1dbf .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi.o: spi.c  .generated_files/flags/PICkit3/4ffe6cfd0b6695d8c7e562cd832eb445d215449e .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ads1115.o: ads1115.c  .generated_files/flags/PICkit3/f29428a6709966440a28761c2ed054e566de558f .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ads1115.o.d 
	@${RM} ${OBJECTDIR}/ads1115.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ads1115.c  -o ${OBJECTDIR}/ads1115.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ads1115.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/PICkit3/39a006ad2cbca15cbb035e2cd253c923689aa789 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/storage.o: storage.c  .generated_files/flags/PICkit3/13b05d530f581b7c2659876ebba80262f9cbf4da .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/storage.o.d 
	@${RM} ${OBJECTDIR}/storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  storage.c  -o ${OBJECTDIR}/storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/storage.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pca9544a.o: pca9544a.c  .generated_files/flags/PICkit3/1f9d17aa4b50bfa170a280adeebdb8e3deffc3f8 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pca9544a.o.d 
	@${RM} ${OBJECTDIR}/pca9544a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pca9544a.c  -o ${OBJECTDIR}/pca9544a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pca9544a.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sensirion_lg16.o: sensirion_lg16.c  .generated_files/flags/PICkit3/de2957ec9752e4760d96cac5e1d1859a0bbe2c20 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o.d 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensirion_lg16.c  -o ${OBJECTDIR}/sensirion_lg16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sensirion_lg16.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -mno-eds-warn  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/PICkit3/34449c389f9b4679f18f28d99f767f564f41cd65 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/PICkit3/ed9c8412a0ff4e7dac843b27f64959f16d8aed64 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/PICkit3/998750813c88d64526a271a7f009091e310cb6cb .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/PICkit3/5a88af534c284f46f8e6d0f0d1df8b4055df7dba .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/PICkit3/b855f2a6ec13cce28d72c50856abf5cdee2bda61 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/PICkit3/c280c34988f104c68772f38799d851781df94351 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/PICkit3/7759268d186c2db47cbb77e1315ed403cfa46ad9 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/PICkit3/599df539be9faac975ffcd220fbcf69e65234307 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/PICkit3/2192058a172988db31bbfee9e2713881a8769226 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/PICkit3/39b22298862b546963dce863d7980de1cea53a47 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/PICkit3/c5fbc45897efc59ec2e10c586b26f11d32aef375 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c2.o: mcc_generated_files/i2c2.c  .generated_files/flags/PICkit3/8be8adeeec3c74ce34ccdc037cb4364099a29a18 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c2.c  -o ${OBJECTDIR}/mcc_generated_files/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/PICkit3/7578fb1318b438f17e5774de044e18be77a34ba8 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/PICkit3/b4b8a962ff8dcaafdef7276db91310954fd4953a .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi.o: spi.c  .generated_files/flags/PICkit3/474e71a8c1ec6aa149f95f682ce755708d4ddc19 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ads1115.o: ads1115.c  .generated_files/flags/PICkit3/b928ce8bfff2a481336435ba6f987029ced865fa .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ads1115.o.d 
	@${RM} ${OBJECTDIR}/ads1115.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ads1115.c  -o ${OBJECTDIR}/ads1115.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ads1115.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/PICkit3/5dfbc32de577221dd463a6d775eedd1472316607 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/storage.o: storage.c  .generated_files/flags/PICkit3/4fdf84f1e1d0c62c1a30fa0474cfa072450244c4 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/storage.o.d 
	@${RM} ${OBJECTDIR}/storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  storage.c  -o ${OBJECTDIR}/storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/storage.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pca9544a.o: pca9544a.c  .generated_files/flags/PICkit3/1d8325f4f26e163b093e10a8af7fef0a04a106f6 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pca9544a.o.d 
	@${RM} ${OBJECTDIR}/pca9544a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pca9544a.c  -o ${OBJECTDIR}/pca9544a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pca9544a.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sensirion_lg16.o: sensirion_lg16.c  .generated_files/flags/PICkit3/df35bdb0baa92b1ed5364d14d3a80925688cb8e4 .generated_files/flags/PICkit3/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o.d 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensirion_lg16.c  -o ${OBJECTDIR}/sensirion_lg16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sensirion_lg16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)   -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_PICkit3=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	${MP_CC_DIR}\\xc16-bin2hex ${DISTDIR}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf   -mdfp="${DFP_DIR}/xc16" 
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(wildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
