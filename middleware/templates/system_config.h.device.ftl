<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.device.ftl

  Summary:
    USB Device Freemarker Template File

  Description:
    This file contains configurations necessary to run the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
*******************************************************************************/

/*******************************************************************************
Copyright (c) 2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS  WITHOUT  WARRANTY  OF  ANY  KIND,
EITHER EXPRESS  OR  IMPLIED,  INCLUDING  WITHOUT  LIMITATION,  ANY  WARRANTY  OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A  PARTICULAR  PURPOSE.
IN NO EVENT SHALL MICROCHIP OR  ITS  LICENSORS  BE  LIABLE  OR  OBLIGATED  UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,  BREACH  OF  WARRANTY,  OR
OTHER LEGAL  EQUITABLE  THEORY  ANY  DIRECT  OR  INDIRECT  DAMAGES  OR  EXPENSES
INCLUDING BUT NOT LIMITED TO ANY  INCIDENTAL,  SPECIAL,  INDIRECT,  PUNITIVE  OR
CONSEQUENTIAL DAMAGES, LOST  PROFITS  OR  LOST  DATA,  COST  OF  PROCUREMENT  OF
SUBSTITUTE  GOODS,  TECHNOLOGY,  SERVICES,  OR  ANY  CLAIMS  BY  THIRD   PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE  THEREOF),  OR  OTHER  SIMILAR  COSTS.
*******************************************************************************/
-->
/*** USB Device Stack Configuration ***/

<#if CONFIG_DRV_USB_DEVICE_SUPPORT == true>
<#assign usbDeviceMSDNumSectors = 0>

<#if CONFIG_USB_DEVICE_FUNCTION_1_MSD_MAX_SECTORS_IDX0?has_content>
    <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_1_MSD_MAX_SECTORS_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_2_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_2_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_2_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_3_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_3_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_3_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_4_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_4_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_4_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_5_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_5_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_6_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_6_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_7_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_7_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_8_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_8_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_9_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_9_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_MSD_MAX_SECTORS_IDX0?has_content>
    <#if CONFIG_USB_DEVICE_FUNCTION_10_MSD_MAX_SECTORS_IDX0?number gt usbDeviceMSDNumSectors> 
        <#assign usbDeviceMSDNumSectors = CONFIG_USB_DEVICE_FUNCTION_10_MSD_MAX_SECTORS_IDX0?number>
    </#if>
</#if>

<#if CONFIG_USB_DEVICE_USE_MSD == true>
#define USB_DEVICE_MSD_NUM_SECTOR_BUFFERS ${usbDeviceMSDNumSectors}
</#if>

</#if>


<#-- Instance 0 -->

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER     ${CONFIG_USB_DEVICE_INSTANCES_NUMBER}

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE      ${CONFIG_USB_DEVICE_EP0_BUFFER_SIZE}

<#if CONFIG_USB_DEVICE_SOF_EVENT_ENABLE == true>
/* Enable SOF Events */ 
#define USB_DEVICE_SOF_EVENT_ENABLE     
</#if>

<#if CONFIG_USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE == true>
/* Enable Set descriptor events */
#define USB_DEVICE_SET_DESCRIPTOR_EVENT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE == true>
/* Enable Synch Frame Event */ 
#define USB_DEVICE_SYNCH_FRAME_EVENT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_BOS_DESCRIPTOR_SUPPORT == true>
/* Enable BOS Descriptor */
#define USB_DEVICE_BOS_DESCRIPTOR_SUPPORT_ENABLE
</#if>

<#if CONFIG_USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED == true>
/* Enable Advanced String Descriptor table. This feature lets the user specify 
   String Index along with the String descriptor Structure  */
#define USB_DEVICE_STRING_DESCRIPTOR_TABLE_ADVANCED_ENABLE
<#if CONFIG_USB_DEVICE_MICROSOFT_OS_DESCRIPTOR_SUPPORT_ENABLE == true>

/* Enable Microsoft OS Descriptor support.  */
#define USB_DEVICE_MICROSOFT_OS_DESCRIPTOR_SUPPORT_ENABLE
</#if>
</#if>
<#if CONFIG_USB_DEVICE_USE_AUDIO == true>
<#assign usbDeviceAudioInstancesNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#assign usbDeviceAudioInstancesNumber = usbDeviceAudioInstancesNumber + 1>
</#if>
/* Maximum instances of Audio function driver */
#define USB_DEVICE_AUDIO_INSTANCES_NUMBER  ${usbDeviceAudioInstancesNumber}

<#assign usbDeviceAudioQueueSizeCombined = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true>
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudioQueueSizeCombined = usbDeviceAudioQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

/* Audio Queue Depth Combined */
#define USB_DEVICE_AUDIO_QUEUE_DEPTH_COMBINED     ${usbDeviceAudioQueueSizeCombined}


<#assign usbDeviceAudioMaxStreamingInterfaces = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudioMaxStreamingInterfaces)>
    	<#assign usbDeviceAudioMaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
/* No of Audio streaming interfaces */
#define USB_DEVICE_AUDIO_MAX_STREAMING_INTERFACES   ${usbDeviceAudioMaxStreamingInterfaces}

<#assign usbDeviceAudioMaxAlternateSettings = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudioMaxAlternateSettings)>
    	<#assign usbDeviceAudioMaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
/* No of alternate settings */
#define USB_DEVICE_AUDIO_MAX_ALTERNATE_SETTING      ${usbDeviceAudioMaxAlternateSettings}

</#if>

<#if CONFIG_USB_DEVICE_USE_AUDIO_2_0 == true>
<#assign usbDeviceAudio2InstancesNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#assign usbDeviceAudio2InstancesNumber = usbDeviceAudio2InstancesNumber + 1>	
</#if>
/* Maximum instances of Audio function driver */
#define USB_DEVICE_AUDIO_V2_INSTANCES_NUMBER  ${usbDeviceAudio2InstancesNumber}

<#assign usbDeviceAudio2QueueSizeCombined = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_READ_QUEUE_SIZE_IDX0?number >
<#assign usbDeviceAudio2QueueSizeCombined = usbDeviceAudio2QueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_WRITE_QUEUE_SIZE_IDX0?number>
</#if>

/* Audio Queue Depth Combined */
#define USB_DEVICE_AUDIO_V2_QUEUE_DEPTH_COMBINED     ${usbDeviceAudio2QueueSizeCombined}


<#assign usbDeviceAudio2MaxStreamingInterfaces = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number > usbDeviceAudio2MaxStreamingInterfaces)>
    	<#assign usbDeviceAudio2MaxStreamingInterfaces = CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_STREAMING_INTERFACES_NUMBER_IDX0?number>
    </#if>
</#if>
/* No of Audio streaming interfaces */
#define USB_DEVICE_AUDIO_V2_MAX_STREAMING_INTERFACES   ${usbDeviceAudio2MaxStreamingInterfaces}

<#assign usbDeviceAudio2MaxAlternateSettings = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_1_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_2_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_3_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_4_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_5_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_6_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_7_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_8_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_9_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_AUDIO_2_0_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number > usbDeviceAudio2MaxAlternateSettings)>
    	<#assign usbDeviceAudio2MaxAlternateSettings = CONFIG_USB_DEVICE_FUNCTION_10_AUDIO_2_0_MAX_ALTERNATE_SETTING_IDX0?number>
    </#if>	
</#if>
/* No of alternate settings */
#define USB_DEVICE_AUDIO_V2_MAX_ALTERNATE_SETTING      ${usbDeviceAudio2MaxAlternateSettings}

</#if>


<#if CONFIG_USB_DEVICE_USE_CDC == true>

<#assign usbDeviceCdcInstancesNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX0 == true>
    <#assign usbDeviceCdcInstancesNumber = usbDeviceCdcInstancesNumber + 1>
</#if>

/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER     ${usbDeviceCdcInstancesNumber}

<#assign usbDeviceCdcQueueSizeCombined = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_CDC_IDX0 == true>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_CDC_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_CDC_WRITE_Q_SIZE_IDX0?number>
<#assign usbDeviceCdcQueueSizeCombined = usbDeviceCdcQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_CDC_SERIAL_NOTIFIACATION_Q_SIZE_IDX0?number>
</#if>
/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED ${usbDeviceCdcQueueSizeCombined}

</#if>
<#if CONFIG_USB_DEVICE_USE_HID == true>

<#assign usbDeviceHidInstancesNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true>
    <#assign usbDeviceHidInstancesNumber = usbDeviceHidInstancesNumber + 1>
</#if>
/* Maximum instances of HID function driver */
#define USB_DEVICE_HID_INSTANCES_NUMBER     ${usbDeviceHidInstancesNumber}

<#assign usbDeviceHidQueueSizeCombined = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_1_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_2_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_3_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_4_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_5_HID_WRITE_Q_SIZE_IDX0?number>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_6_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_HID_READ_Q_SIZE_IDX0?number>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_7_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_8_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_9_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_HID_IDX0 == true>
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_HID_READ_Q_SIZE_IDX0?number >
<#assign usbDeviceHidQueueSizeCombined = usbDeviceHidQueueSizeCombined + CONFIG_USB_DEVICE_FUNCTION_10_HID_WRITE_Q_SIZE_IDX0?number>
</#if>

/* HID Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_HID_QUEUE_DEPTH_COMBINED ${usbDeviceHidQueueSizeCombined}

</#if>
<#if CONFIG_USB_DEVICE_USE_MSD == true>
<#assign usbDeviceMsdInstancesNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true>
    <#assign usbDeviceMsdInstancesNumber = usbDeviceMsdInstancesNumber + 1>
</#if>

/* Number of MSD Function driver instances in the application */
#define USB_DEVICE_MSD_INSTANCES_NUMBER ${usbDeviceMsdInstancesNumber}

<#assign usbDeviceMsdMaxLunNumber = 0>
<#if CONFIG_USB_DEVICE_FUNCTION_1_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_1_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_1_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_2_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_2_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_2_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_3_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_3_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_3_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_4_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_4_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_4_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_5_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_5_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_5_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_6_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_6_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_6_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_7_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_7_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_7_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_8_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_8_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_8_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_9_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_9_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_9_MSD_LUN_IDX0?number>
    </#if>
</#if>
<#if CONFIG_USB_DEVICE_FUNCTION_10_DEVICE_CLASS_MSD_IDX0 == true>
    <#if (CONFIG_USB_DEVICE_FUNCTION_10_MSD_LUN_IDX0?number > usbDeviceMsdMaxLunNumber)>
    	<#assign usbDeviceMsdMaxLunNumber = CONFIG_USB_DEVICE_FUNCTION_10_MSD_LUN_IDX0?number>
    </#if>
</#if>
/* Number of Logical Units */
#define USB_DEVICE_MSD_LUNS_NUMBER      ${usbDeviceMsdMaxLunNumber}

/* Size of disk image (in KB) in Program Flash Memory */
#define DRV_NVM_BLOCK_MEMORY_SIZE       36

</#if>
<#if CONFIG_USB_DEVICE_USE_ENDPOINT_FUNCTIONS == true>
/* Endpoint Transfer Queue Size combined for Read and write */
#define USB_DEVICE_ENDPOINT_QUEUE_DEPTH_COMBINED    ${CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_READ_IDX0?number + CONFIG_USB_DEVICE_ENDPOINT_QUEUE_SIZE_WRITE_IDX0?number}

</#if>
<#--
/*******************************************************************************
 End of File
*/
-->

