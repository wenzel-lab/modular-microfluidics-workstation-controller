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
ifeq "$(wildcard nbproject/Makefile-local-PICkit4.mk)" "nbproject/Makefile-local-PICkit4.mk"
include nbproject/Makefile-local-PICkit4.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PICkit4
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
	${MAKE}  -f nbproject/Makefile-PICkit4.mk ${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33CK256MP502
MP_LINKER_FILE_OPTION=,--script=p33CK256MP502.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/PICkit4/1a8f8926720bb7269b2f5c7d4cc48eb06ed09d10 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/PICkit4/840c388101c50b415e759949b87da56b73154005 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/PICkit4/2b30757549ef270ceaee1cb275ea7f62814583ce .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/PICkit4/e1df86548d24ba022a09fac81626d3d377071167 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/PICkit4/ad0bebcdc0d38afa93d46771780d4044ee109160 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/PICkit4/b1c2ade3712dd9e68d76084c921fb449eeabf1ce .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/PICkit4/e4dddf17d9b5ccee456780309dc1c0ebdb4d25b6 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/PICkit4/f531029b4bbf1100a662cdd59bc407838e534619 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/PICkit4/20d7147ebdcd452664163dd255b6949b95951ccb .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/PICkit4/7090ba510a08f6ecb34928867b814969d1d69908 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/PICkit4/fd83279d0a33612e620d08227bf178813fc24ca4 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c2.o: mcc_generated_files/i2c2.c  .generated_files/flags/PICkit4/6fe5b6505acdaf026d3409aa1e87240acae0bd90 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c2.c  -o ${OBJECTDIR}/mcc_generated_files/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c2.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/PICkit4/8e85796a074efaa4acdd3bd7386ab97d1506f947 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/PICkit4/824b2e7f76e8a1a6e88edf0dcb2722f1a119c08f .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi.o: spi.c  .generated_files/flags/PICkit4/285e2c16f18cb97dfcefe737cdbc46139f03b875 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ads1115.o: ads1115.c  .generated_files/flags/PICkit4/578eebe03cdfa484e100037cfd5354a99b214001 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ads1115.o.d 
	@${RM} ${OBJECTDIR}/ads1115.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ads1115.c  -o ${OBJECTDIR}/ads1115.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ads1115.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/PICkit4/b3e8dd8ea621b33aaf710d6e1fa488ddf7319e08 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/storage.o: storage.c  .generated_files/flags/PICkit4/27e5efe62355bdf68fa20b4099ff34cff67fa09b .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/storage.o.d 
	@${RM} ${OBJECTDIR}/storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  storage.c  -o ${OBJECTDIR}/storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/storage.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pca9544a.o: pca9544a.c  .generated_files/flags/PICkit4/bfff26a71cd39af5a15902507a982fc594f2db3 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pca9544a.o.d 
	@${RM} ${OBJECTDIR}/pca9544a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pca9544a.c  -o ${OBJECTDIR}/pca9544a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pca9544a.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sensirion_lg16.o: sensirion_lg16.c  .generated_files/flags/PICkit4/49f432525f2daa6aaed6db4aee8225d4212122a .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o.d 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensirion_lg16.c  -o ${OBJECTDIR}/sensirion_lg16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sensirion_lg16.o.d"      -g -D__DEBUG   -mno-eds-warn  -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
else
${OBJECTDIR}/mcc_generated_files/reset.o: mcc_generated_files/reset.c  .generated_files/flags/PICkit4/989b3fee3e289acb06415b1143c2eb26f48a1136 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/reset.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/reset.c  -o ${OBJECTDIR}/mcc_generated_files/reset.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/reset.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/system.o: mcc_generated_files/system.c  .generated_files/flags/PICkit4/6eb8c85b3e174c758700b5d70a9f089e1d5872d3 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/system.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/system.c  -o ${OBJECTDIR}/mcc_generated_files/system.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/system.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/traps.o: mcc_generated_files/traps.c  .generated_files/flags/PICkit4/f95d5b99ff642c3825517489b71276a95ad1e5a8 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/traps.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/traps.c  -o ${OBJECTDIR}/mcc_generated_files/traps.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/traps.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/clock.o: mcc_generated_files/clock.c  .generated_files/flags/PICkit4/c4c2277de433b743fbe20628e77c740d905e8da8 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/clock.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/clock.c  -o ${OBJECTDIR}/mcc_generated_files/clock.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/clock.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.o: mcc_generated_files/interrupt_manager.c  .generated_files/flags/PICkit4/17b0e7e2a3fe2fbf192f9cdd9ee3347f0189b136 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/interrupt_manager.c  -o ${OBJECTDIR}/mcc_generated_files/interrupt_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/interrupt_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/mcc.o: mcc_generated_files/mcc.c  .generated_files/flags/PICkit4/53dbe918c6ab99df304edad02fdfd168d6044f5b .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/mcc.c  -o ${OBJECTDIR}/mcc_generated_files/mcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/mcc.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/pin_manager.o: mcc_generated_files/pin_manager.c  .generated_files/flags/PICkit4/48743aee81542cc09baece50a15e32b4ab23dfcd .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/pin_manager.c  -o ${OBJECTDIR}/mcc_generated_files/pin_manager.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/pin_manager.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi2.o: mcc_generated_files/spi2.c  .generated_files/flags/PICkit4/4710ba3830a1e6102163feeb4c9d9bd782f4bb75 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi2.c  -o ${OBJECTDIR}/mcc_generated_files/spi2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi1.o: mcc_generated_files/spi1.c  .generated_files/flags/PICkit4/e88fedb48378789215c245e3a2ad3b322982753f .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi1.c  -o ${OBJECTDIR}/mcc_generated_files/spi1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/tmr1.o: mcc_generated_files/tmr1.c  .generated_files/flags/PICkit4/e46d0e822913126543814b55ebe19f4962e1126a .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/tmr1.c  -o ${OBJECTDIR}/mcc_generated_files/tmr1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/tmr1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/spi3.o: mcc_generated_files/spi3.c  .generated_files/flags/PICkit4/81cf545b8da1a300705096460d731bd0b5dc18d6 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi3.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/spi3.c  -o ${OBJECTDIR}/mcc_generated_files/spi3.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/spi3.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/i2c2.o: mcc_generated_files/i2c2.c  .generated_files/flags/PICkit4/a2a3af7b3c239eff7934f756c894ff8f23888b96 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c2.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/i2c2.c  -o ${OBJECTDIR}/mcc_generated_files/i2c2.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/i2c2.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/mcc_generated_files/uart1.o: mcc_generated_files/uart1.c  .generated_files/flags/PICkit4/c593563b2030fab5671932c0d86f8aad1063dc7a .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/uart1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  mcc_generated_files/uart1.c  -o ${OBJECTDIR}/mcc_generated_files/uart1.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/mcc_generated_files/uart1.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/PICkit4/21d62f50c9f3e1e3269cab0a88366d9026c86733 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/main.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/spi.o: spi.c  .generated_files/flags/PICkit4/9288f7a79b195d9d76aa7f23e7a9ddc5443f2c .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/spi.o.d 
	@${RM} ${OBJECTDIR}/spi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  spi.c  -o ${OBJECTDIR}/spi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/spi.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/ads1115.o: ads1115.c  .generated_files/flags/PICkit4/55ca9bfafa16478477a6020be7c937b49ddd53ff .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ads1115.o.d 
	@${RM} ${OBJECTDIR}/ads1115.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ads1115.c  -o ${OBJECTDIR}/ads1115.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/ads1115.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/eeprom.o: eeprom.c  .generated_files/flags/PICkit4/7081cfb57af88b236e46df3213cbc6c187ea4582 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/eeprom.o.d 
	@${RM} ${OBJECTDIR}/eeprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  eeprom.c  -o ${OBJECTDIR}/eeprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/eeprom.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/storage.o: storage.c  .generated_files/flags/PICkit4/124fea0943a0cea92ceb1093670f95d6d19182c5 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/storage.o.d 
	@${RM} ${OBJECTDIR}/storage.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  storage.c  -o ${OBJECTDIR}/storage.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/storage.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/pca9544a.o: pca9544a.c  .generated_files/flags/PICkit4/b7622b1e7b6d97c38021e0074165b1fc1b86c083 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/pca9544a.o.d 
	@${RM} ${OBJECTDIR}/pca9544a.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  pca9544a.c  -o ${OBJECTDIR}/pca9544a.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/pca9544a.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
${OBJECTDIR}/sensirion_lg16.o: sensirion_lg16.c  .generated_files/flags/PICkit4/5a190eb9a961efd1afa15848ae44365d11689892 .generated_files/flags/PICkit4/da39a3ee5e6b4b0d3255bfef95601890afd80709
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o.d 
	@${RM} ${OBJECTDIR}/sensirion_lg16.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  sensirion_lg16.c  -o ${OBJECTDIR}/sensirion_lg16.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MP -MMD -MF "${OBJECTDIR}/sensirion_lg16.o.d"      -mno-eds-warn  -g -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -O0 -msmart-io=1 -Wall -mno-isr-warn -msfr-warn=off    -mdfp="${DFP_DIR}/xc16"
	
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
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG=__DEBUG   -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)      -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D__DEBUG=__DEBUG,,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
	
else
${DISTDIR}/firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o ${DISTDIR}/firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -DXPRJ_PICkit4=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--local-stack,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem,--memorysummary,${DISTDIR}/memoryfile.xml$(MP_EXTRA_LD_POST)  -mdfp="${DFP_DIR}/xc16" 
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
