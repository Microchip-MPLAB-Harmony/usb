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
ifeq "$(wildcard nbproject/Makefile-local-sam_e70_xult_freertos.mk)" "nbproject/Makefile-local-sam_e70_xult_freertos.mk"
include nbproject/Makefile-local-sam_e70_xult_freertos.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=sam_e70_xult_freertos
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/main.c ../src/config/sam_e70_xult_freertos/initialization.c ../src/config/sam_e70_xult_freertos/interrupts.c ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c ../src/config/sam_e70_xult_freertos/exceptions.c ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c ../src/config/sam_e70_xult_freertos/startup.c ../src/config/sam_e70_xult_freertos/libc_syscalls.c ../src/config/sam_e70_xult_freertos/bsp/bsp.c ../src/config/sam_e70_xult_freertos/freertos_hooks.c ../src/third_party/rtos/FreeRTOS/Source/croutine.c ../src/third_party/rtos/FreeRTOS/Source/list.c ../src/third_party/rtos/FreeRTOS/Source/queue.c ../src/third_party/rtos/FreeRTOS/Source/tasks.c ../src/third_party/rtos/FreeRTOS/Source/timers.c ../src/third_party/rtos/FreeRTOS/Source/event_groups.c ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c ../src/app.c ../src/config/sam_e70_xult_freertos/tasks.c ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c ../src/config/sam_e70_xult_freertos/usb_device_init_data.c ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c ../src/keyboard.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1552905064/initialization.o ${OBJECTDIR}/_ext/1552905064/interrupts.o ${OBJECTDIR}/_ext/146190766/xc32_monitor.o ${OBJECTDIR}/_ext/1552905064/exceptions.o ${OBJECTDIR}/_ext/45647770/plib_clk.o ${OBJECTDIR}/_ext/45635366/plib_pio.o ${OBJECTDIR}/_ext/1414743522/plib_nvic.o ${OBJECTDIR}/_ext/1552905064/startup.o ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o ${OBJECTDIR}/_ext/683629640/bsp.o ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o ${OBJECTDIR}/_ext/404212886/croutine.o ${OBJECTDIR}/_ext/404212886/list.o ${OBJECTDIR}/_ext/404212886/queue.o ${OBJECTDIR}/_ext/404212886/tasks.o ${OBJECTDIR}/_ext/404212886/timers.o ${OBJECTDIR}/_ext/404212886/event_groups.o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ${OBJECTDIR}/_ext/1665200909/heap_1.o ${OBJECTDIR}/_ext/977623654/port.o ${OBJECTDIR}/_ext/1775855163/sys_int.o ${OBJECTDIR}/_ext/281930714/osal_freertos.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1552905064/tasks.o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o ${OBJECTDIR}/_ext/1307561886/usb_device.o ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o ${OBJECTDIR}/_ext/1360937237/keyboard.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1552905064/initialization.o.d ${OBJECTDIR}/_ext/1552905064/interrupts.o.d ${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d ${OBJECTDIR}/_ext/1552905064/exceptions.o.d ${OBJECTDIR}/_ext/45647770/plib_clk.o.d ${OBJECTDIR}/_ext/45635366/plib_pio.o.d ${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d ${OBJECTDIR}/_ext/1552905064/startup.o.d ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d ${OBJECTDIR}/_ext/683629640/bsp.o.d ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d ${OBJECTDIR}/_ext/404212886/croutine.o.d ${OBJECTDIR}/_ext/404212886/list.o.d ${OBJECTDIR}/_ext/404212886/queue.o.d ${OBJECTDIR}/_ext/404212886/tasks.o.d ${OBJECTDIR}/_ext/404212886/timers.o.d ${OBJECTDIR}/_ext/404212886/event_groups.o.d ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d ${OBJECTDIR}/_ext/1665200909/heap_1.o.d ${OBJECTDIR}/_ext/977623654/port.o.d ${OBJECTDIR}/_ext/1775855163/sys_int.o.d ${OBJECTDIR}/_ext/281930714/osal_freertos.o.d ${OBJECTDIR}/_ext/1360937237/app.o.d ${OBJECTDIR}/_ext/1552905064/tasks.o.d ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d ${OBJECTDIR}/_ext/1307561886/usb_device.o.d ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d ${OBJECTDIR}/_ext/1360937237/keyboard.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1552905064/initialization.o ${OBJECTDIR}/_ext/1552905064/interrupts.o ${OBJECTDIR}/_ext/146190766/xc32_monitor.o ${OBJECTDIR}/_ext/1552905064/exceptions.o ${OBJECTDIR}/_ext/45647770/plib_clk.o ${OBJECTDIR}/_ext/45635366/plib_pio.o ${OBJECTDIR}/_ext/1414743522/plib_nvic.o ${OBJECTDIR}/_ext/1552905064/startup.o ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o ${OBJECTDIR}/_ext/683629640/bsp.o ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o ${OBJECTDIR}/_ext/404212886/croutine.o ${OBJECTDIR}/_ext/404212886/list.o ${OBJECTDIR}/_ext/404212886/queue.o ${OBJECTDIR}/_ext/404212886/tasks.o ${OBJECTDIR}/_ext/404212886/timers.o ${OBJECTDIR}/_ext/404212886/event_groups.o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ${OBJECTDIR}/_ext/1665200909/heap_1.o ${OBJECTDIR}/_ext/977623654/port.o ${OBJECTDIR}/_ext/1775855163/sys_int.o ${OBJECTDIR}/_ext/281930714/osal_freertos.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1552905064/tasks.o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o ${OBJECTDIR}/_ext/1307561886/usb_device.o ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o ${OBJECTDIR}/_ext/1360937237/keyboard.o

# Source Files
SOURCEFILES=../src/main.c ../src/config/sam_e70_xult_freertos/initialization.c ../src/config/sam_e70_xult_freertos/interrupts.c ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c ../src/config/sam_e70_xult_freertos/exceptions.c ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c ../src/config/sam_e70_xult_freertos/startup.c ../src/config/sam_e70_xult_freertos/libc_syscalls.c ../src/config/sam_e70_xult_freertos/bsp/bsp.c ../src/config/sam_e70_xult_freertos/freertos_hooks.c ../src/third_party/rtos/FreeRTOS/Source/croutine.c ../src/third_party/rtos/FreeRTOS/Source/list.c ../src/third_party/rtos/FreeRTOS/Source/queue.c ../src/third_party/rtos/FreeRTOS/Source/tasks.c ../src/third_party/rtos/FreeRTOS/Source/timers.c ../src/third_party/rtos/FreeRTOS/Source/event_groups.c ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c ../src/app.c ../src/config/sam_e70_xult_freertos/tasks.c ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c ../src/config/sam_e70_xult_freertos/usb_device_init_data.c ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c ../src/keyboard.c


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
	${MAKE}  -f nbproject/Makefile-sam_e70_xult_freertos.mk dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAME70Q21B
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/initialization.o: ../src/config/sam_e70_xult_freertos/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/initialization.o.d" -o ${OBJECTDIR}/_ext/1552905064/initialization.o ../src/config/sam_e70_xult_freertos/initialization.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/interrupts.o: ../src/config/sam_e70_xult_freertos/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/interrupts.o.d" -o ${OBJECTDIR}/_ext/1552905064/interrupts.o ../src/config/sam_e70_xult_freertos/interrupts.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/146190766/xc32_monitor.o: ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/146190766" 
	@${RM} ${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/146190766/xc32_monitor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/146190766/xc32_monitor.o ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/exceptions.o: ../src/config/sam_e70_xult_freertos/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/exceptions.o.d" -o ${OBJECTDIR}/_ext/1552905064/exceptions.o ../src/config/sam_e70_xult_freertos/exceptions.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/45647770/plib_clk.o: ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/45647770" 
	@${RM} ${OBJECTDIR}/_ext/45647770/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/45647770/plib_clk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/45647770/plib_clk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/45647770/plib_clk.o.d" -o ${OBJECTDIR}/_ext/45647770/plib_clk.o ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/45635366/plib_pio.o: ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/45635366" 
	@${RM} ${OBJECTDIR}/_ext/45635366/plib_pio.o.d 
	@${RM} ${OBJECTDIR}/_ext/45635366/plib_pio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/45635366/plib_pio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/45635366/plib_pio.o.d" -o ${OBJECTDIR}/_ext/45635366/plib_pio.o ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1414743522/plib_nvic.o: ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1414743522" 
	@${RM} ${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1414743522/plib_nvic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1414743522/plib_nvic.o ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/startup.o: ../src/config/sam_e70_xult_freertos/startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/startup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/startup.o.d" -o ${OBJECTDIR}/_ext/1552905064/startup.o ../src/config/sam_e70_xult_freertos/startup.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/libc_syscalls.o: ../src/config/sam_e70_xult_freertos/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o ../src/config/sam_e70_xult_freertos/libc_syscalls.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/683629640/bsp.o: ../src/config/sam_e70_xult_freertos/bsp/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/683629640" 
	@${RM} ${OBJECTDIR}/_ext/683629640/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/683629640/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/683629640/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/683629640/bsp.o.d" -o ${OBJECTDIR}/_ext/683629640/bsp.o ../src/config/sam_e70_xult_freertos/bsp/bsp.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/freertos_hooks.o: ../src/config/sam_e70_xult_freertos/freertos_hooks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d" -o ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o ../src/config/sam_e70_xult_freertos/freertos_hooks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/croutine.o: ../src/third_party/rtos/FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/croutine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/croutine.o.d" -o ${OBJECTDIR}/_ext/404212886/croutine.o ../src/third_party/rtos/FreeRTOS/Source/croutine.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/list.o: ../src/third_party/rtos/FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/list.o.d" -o ${OBJECTDIR}/_ext/404212886/list.o ../src/third_party/rtos/FreeRTOS/Source/list.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/queue.o: ../src/third_party/rtos/FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/queue.o.d" -o ${OBJECTDIR}/_ext/404212886/queue.o ../src/third_party/rtos/FreeRTOS/Source/queue.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/tasks.o: ../src/third_party/rtos/FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/tasks.o.d" -o ${OBJECTDIR}/_ext/404212886/tasks.o ../src/third_party/rtos/FreeRTOS/Source/tasks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/timers.o: ../src/third_party/rtos/FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/timers.o.d" -o ${OBJECTDIR}/_ext/404212886/timers.o ../src/third_party/rtos/FreeRTOS/Source/timers.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/event_groups.o: ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/event_groups.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/event_groups.o.d" -o ${OBJECTDIR}/_ext/404212886/event_groups.o ../src/third_party/rtos/FreeRTOS/Source/event_groups.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/stream_buffer.o: ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1665200909/heap_1.o: ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1665200909" 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" -o ${OBJECTDIR}/_ext/1665200909/heap_1.o ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/977623654/port.o: ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/977623654" 
	@${RM} ${OBJECTDIR}/_ext/977623654/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/977623654/port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/977623654/port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/977623654/port.o.d" -o ${OBJECTDIR}/_ext/977623654/port.o ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1775855163/sys_int.o: ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1775855163" 
	@${RM} ${OBJECTDIR}/_ext/1775855163/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1775855163/sys_int.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1775855163/sys_int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1775855163/sys_int.o.d" -o ${OBJECTDIR}/_ext/1775855163/sys_int.o ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/281930714/osal_freertos.o: ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/281930714" 
	@${RM} ${OBJECTDIR}/_ext/281930714/osal_freertos.o.d 
	@${RM} ${OBJECTDIR}/_ext/281930714/osal_freertos.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/281930714/osal_freertos.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/281930714/osal_freertos.o.d" -o ${OBJECTDIR}/_ext/281930714/osal_freertos.o ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/tasks.o: ../src/config/sam_e70_xult_freertos/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/tasks.o.d" -o ${OBJECTDIR}/_ext/1552905064/tasks.o ../src/config/sam_e70_xult_freertos/tasks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o: ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/6961668" 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d" -o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o: ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/6961668" 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d" -o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o: ../src/config/sam_e70_xult_freertos/usb_device_init_data.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d" -o ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o ../src/config/sam_e70_xult_freertos/usb_device_init_data.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1307561886/usb_device.o: ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1307561886" 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1307561886/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1307561886/usb_device.o.d" -o ${OBJECTDIR}/_ext/1307561886/usb_device.o ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1307561886/usb_device_hid.o: ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1307561886" 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d" -o ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/keyboard.o: ../src/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/keyboard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/keyboard.o.d" -o ${OBJECTDIR}/_ext/1360937237/keyboard.o ../src/keyboard.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/initialization.o: ../src/config/sam_e70_xult_freertos/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/initialization.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/initialization.o.d" -o ${OBJECTDIR}/_ext/1552905064/initialization.o ../src/config/sam_e70_xult_freertos/initialization.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/interrupts.o: ../src/config/sam_e70_xult_freertos/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/interrupts.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/interrupts.o.d" -o ${OBJECTDIR}/_ext/1552905064/interrupts.o ../src/config/sam_e70_xult_freertos/interrupts.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/146190766/xc32_monitor.o: ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/146190766" 
	@${RM} ${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/146190766/xc32_monitor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/146190766/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/146190766/xc32_monitor.o ../src/config/sam_e70_xult_freertos/stdio/xc32_monitor.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/exceptions.o: ../src/config/sam_e70_xult_freertos/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/exceptions.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/exceptions.o.d" -o ${OBJECTDIR}/_ext/1552905064/exceptions.o ../src/config/sam_e70_xult_freertos/exceptions.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/45647770/plib_clk.o: ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/45647770" 
	@${RM} ${OBJECTDIR}/_ext/45647770/plib_clk.o.d 
	@${RM} ${OBJECTDIR}/_ext/45647770/plib_clk.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/45647770/plib_clk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/45647770/plib_clk.o.d" -o ${OBJECTDIR}/_ext/45647770/plib_clk.o ../src/config/sam_e70_xult_freertos/peripheral/clk/plib_clk.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/45635366/plib_pio.o: ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/45635366" 
	@${RM} ${OBJECTDIR}/_ext/45635366/plib_pio.o.d 
	@${RM} ${OBJECTDIR}/_ext/45635366/plib_pio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/45635366/plib_pio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/45635366/plib_pio.o.d" -o ${OBJECTDIR}/_ext/45635366/plib_pio.o ../src/config/sam_e70_xult_freertos/peripheral/pio/plib_pio.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1414743522/plib_nvic.o: ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1414743522" 
	@${RM} ${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1414743522/plib_nvic.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1414743522/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1414743522/plib_nvic.o ../src/config/sam_e70_xult_freertos/peripheral/nvic/plib_nvic.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/startup.o: ../src/config/sam_e70_xult_freertos/startup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/startup.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/startup.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/startup.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/startup.o.d" -o ${OBJECTDIR}/_ext/1552905064/startup.o ../src/config/sam_e70_xult_freertos/startup.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/libc_syscalls.o: ../src/config/sam_e70_xult_freertos/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/1552905064/libc_syscalls.o ../src/config/sam_e70_xult_freertos/libc_syscalls.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/683629640/bsp.o: ../src/config/sam_e70_xult_freertos/bsp/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/683629640" 
	@${RM} ${OBJECTDIR}/_ext/683629640/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/683629640/bsp.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/683629640/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/683629640/bsp.o.d" -o ${OBJECTDIR}/_ext/683629640/bsp.o ../src/config/sam_e70_xult_freertos/bsp/bsp.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/freertos_hooks.o: ../src/config/sam_e70_xult_freertos/freertos_hooks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/freertos_hooks.o.d" -o ${OBJECTDIR}/_ext/1552905064/freertos_hooks.o ../src/config/sam_e70_xult_freertos/freertos_hooks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/croutine.o: ../src/third_party/rtos/FreeRTOS/Source/croutine.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/croutine.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/croutine.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/croutine.o.d" -o ${OBJECTDIR}/_ext/404212886/croutine.o ../src/third_party/rtos/FreeRTOS/Source/croutine.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/list.o: ../src/third_party/rtos/FreeRTOS/Source/list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/list.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/list.o.d" -o ${OBJECTDIR}/_ext/404212886/list.o ../src/third_party/rtos/FreeRTOS/Source/list.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/queue.o: ../src/third_party/rtos/FreeRTOS/Source/queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/queue.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/queue.o.d" -o ${OBJECTDIR}/_ext/404212886/queue.o ../src/third_party/rtos/FreeRTOS/Source/queue.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/tasks.o: ../src/third_party/rtos/FreeRTOS/Source/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/tasks.o.d" -o ${OBJECTDIR}/_ext/404212886/tasks.o ../src/third_party/rtos/FreeRTOS/Source/tasks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/timers.o: ../src/third_party/rtos/FreeRTOS/Source/timers.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/timers.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/timers.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/timers.o.d" -o ${OBJECTDIR}/_ext/404212886/timers.o ../src/third_party/rtos/FreeRTOS/Source/timers.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/event_groups.o: ../src/third_party/rtos/FreeRTOS/Source/event_groups.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/event_groups.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/event_groups.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/event_groups.o.d" -o ${OBJECTDIR}/_ext/404212886/event_groups.o ../src/third_party/rtos/FreeRTOS/Source/event_groups.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/404212886/stream_buffer.o: ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/404212886" 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o.d 
	@${RM} ${OBJECTDIR}/_ext/404212886/stream_buffer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/404212886/stream_buffer.o.d" -o ${OBJECTDIR}/_ext/404212886/stream_buffer.o ../src/third_party/rtos/FreeRTOS/Source/stream_buffer.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1665200909/heap_1.o: ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1665200909" 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o.d 
	@${RM} ${OBJECTDIR}/_ext/1665200909/heap_1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1665200909/heap_1.o.d" -o ${OBJECTDIR}/_ext/1665200909/heap_1.o ../src/third_party/rtos/FreeRTOS/Source/portable/MemMang/heap_1.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/977623654/port.o: ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/977623654" 
	@${RM} ${OBJECTDIR}/_ext/977623654/port.o.d 
	@${RM} ${OBJECTDIR}/_ext/977623654/port.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/977623654/port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/977623654/port.o.d" -o ${OBJECTDIR}/_ext/977623654/port.o ../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7/port.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1775855163/sys_int.o: ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1775855163" 
	@${RM} ${OBJECTDIR}/_ext/1775855163/sys_int.o.d 
	@${RM} ${OBJECTDIR}/_ext/1775855163/sys_int.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1775855163/sys_int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1775855163/sys_int.o.d" -o ${OBJECTDIR}/_ext/1775855163/sys_int.o ../src/config/sam_e70_xult_freertos/system/int/src/sys_int.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/281930714/osal_freertos.o: ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/281930714" 
	@${RM} ${OBJECTDIR}/_ext/281930714/osal_freertos.o.d 
	@${RM} ${OBJECTDIR}/_ext/281930714/osal_freertos.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/281930714/osal_freertos.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/281930714/osal_freertos.o.d" -o ${OBJECTDIR}/_ext/281930714/osal_freertos.o ../src/config/sam_e70_xult_freertos/osal/osal_freertos.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/tasks.o: ../src/config/sam_e70_xult_freertos/tasks.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/tasks.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/tasks.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/tasks.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/tasks.o.d" -o ${OBJECTDIR}/_ext/1552905064/tasks.o ../src/config/sam_e70_xult_freertos/tasks.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o: ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/6961668" 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o.d" -o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1.o ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o: ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/6961668" 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o.d" -o ${OBJECTDIR}/_ext/6961668/drv_usbhsv1_device.o ../src/config/sam_e70_xult_freertos/driver/usb/usbhsv1/src/drv_usbhsv1_device.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o: ../src/config/sam_e70_xult_freertos/usb_device_init_data.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1552905064" 
	@${RM} ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d 
	@${RM} ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o.d" -o ${OBJECTDIR}/_ext/1552905064/usb_device_init_data.o ../src/config/sam_e70_xult_freertos/usb_device_init_data.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1307561886/usb_device.o: ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1307561886" 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1307561886/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1307561886/usb_device.o.d" -o ${OBJECTDIR}/_ext/1307561886/usb_device.o ../src/config/sam_e70_xult_freertos/usb/src/usb_device.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1307561886/usb_device_hid.o: ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1307561886" 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1307561886/usb_device_hid.o.d" -o ${OBJECTDIR}/_ext/1307561886/usb_device_hid.o ../src/config/sam_e70_xult_freertos/usb/src/usb_device_hid.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
${OBJECTDIR}/_ext/1360937237/keyboard.o: ../src/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/keyboard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/keyboard.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/keyboard.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/sam_e70_xult_freertos" -I"../src/packs/ATSAME70Q21B_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -I"../src/third_party/rtos/FreeRTOS/Source/portable/GCC/SAM/CM7" -I"../src/third_party/rtos/FreeRTOS/Source/Include" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/keyboard.o.d" -o ${OBJECTDIR}/_ext/1360937237/keyboard.o ../src/keyboard.c    -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g  -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=0,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_sam_e70_xult_freertos=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=0,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/sam_e70_xult_freertos.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/sam_e70_xult_freertos
	${RM} -r dist/sam_e70_xult_freertos

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
