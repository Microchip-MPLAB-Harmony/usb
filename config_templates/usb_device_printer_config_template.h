/*******************************************************************************
  USB Device Printer Class Configuration Definitions 

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_printer_config_template.h

  Summary:
    USB device Printer Class configuration definitions template

  Description:
    This file contains configurations macros needed to configure the Printer
    Function Driver. This file is a template file only. It should not be
    included by the application. The configuration macros defined in the file
    should be defined in the configuration specific system_config.h.
*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _USB_DEVICE_PRINTER_CONFIG_TEMPLATE_H_
#define _USB_DEVICE_PRINTER_CONFIG_TEMPLATE_H_

#error "This is configuration template file. Do not include it directly."
//DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Section: USB Device Printer Class Configuration
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* USB device Printer Maximum Number of instances

  Summary:
    Specifies the number of Printer instances.

  Description:
    This macro defines the number of instances of the Printer Function Driver.
    Do not modify this value. ALways set this value to 1, only one instance. 

  Remarks:
    None.
*/

#define USB_DEVICE_PRINTER_INSTANCES_NUMBER       /*DOM-IGNORE-BEGIN*/ 1 /*DOM-IGNORE-END*/

// *****************************************************************************
/* USB device Printer Combined Queue Size

  Summary:
    Specifies the combined queue size of all Printer instances.

  Description:
    This macro defines the number of entries in all queues in all instances of
    the Printer function driver. This value can be obtained by adding up the read
    and write queue sizes of each Printer Function driver instance. In a simple
    single instance USB Printer device application, that does not require buffer
    queuing and serial state notification, the
    USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED macro can be set to 2. Consider a case
    with two Printer function driver instances, Printer 1 has a read queue size of 2 and
    write queue size of 3, Printer 2 has a read queue size of 4 and write queue size
    of 1, this macro should be set to 10 (2 +3 + 4 + 1). 

  Remarks:
    None.
*/

#define USB_DEVICE_PRINTER_QUEUE_DEPTH_COMBINED /*DOM-IGNORE-BEGIN*/ 2 /*DOM-IGNORE-END*/

#endif // #ifndef _USB_DEVICE_PRINTER_CONFIG_TEMPLATE_H_

/*******************************************************************************
 End of File
*/

