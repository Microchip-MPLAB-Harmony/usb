<#--
/*******************************************************************************
  USB Device Freemarker Template File

  Company:
    Microchip Technology Inc.

  File Name:
    system_config.h.host_hid.ftl

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
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
-->
/* Number of HID Client driver instances in the application */
#define USB_HOST_HID_INSTANCES_NUMBER        ${CONFIG_USB_HOST_HID_NUMBER_OF_INSTANCES}

/* Maximum number of INTERRUPT IN endpoints supported per HID interface */
#define USB_HOST_HID_INTERRUPT_IN_ENDPOINTS_NUMBER 1

/* Number of total usage driver instances registered with HID client driver */
#define USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER  1

/* Maximum number PUSH items that can be saved in the Global item queue per field
 * per HID interface */
#define USB_HID_GLOBAL_PUSH_POP_STACK_SIZE 1
<#--
/*******************************************************************************
 End of File
*/
-->

