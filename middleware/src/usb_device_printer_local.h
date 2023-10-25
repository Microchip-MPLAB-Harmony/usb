/*******************************************************************************
  USB Printer class driver interface header

  Company:
    Microchip Technology Inc.

  File Name:
    usb_device_printer_local.h

  Summary:
    USB Printer class driver interface header

  Description:
    USB Printer class driver interface header
*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

#ifndef USB_DEVICE_PRINTER_LOCAL_H
#define USB_DEVICE_PRINTER_LOCAL_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "configuration.h"
#include "system/system_common.h"
#include "system/system_module.h"
#include "usb/usb_common.h"
#include "usb/usb_chapter_9.h"
#include "usb/usb_device.h"
#include "osal/osal.h"


// *****************************************************************************
// *****************************************************************************
// Section: Constants
// *****************************************************************************
// *****************************************************************************

#define USB_DEVICE_PRINTER_ENDPOINT_RX          USB_DATA_DIRECTION_HOST_TO_DEVICE 
#define USB_DEVICE_PRINTER_ENDPOINT_TX          USB_DATA_DIRECTION_DEVICE_TO_HOST

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
/* Printer endpoint instance.

  Summary:
    Identifies the Printer endpoint instance.

  Description:
    This type identifies the Printer endpoint instance.

  Remarks:
    This structure is internal to the Printer function driver.
*/
typedef struct
{
    /* End point address */
    uint8_t address;

    /* End point maximum payload */
    uint16_t maxPacketSize;

    bool    isConfigured;

}USB_DEVICE_PRINTER_ENDPOINT;


// *****************************************************************************
/* Printer interface types

  Summary:

  Description:

  Remarks:
*/
typedef enum
{
    /* The unidirectional interface supports only the sending of data to 
    the printer via a Bulk OUT endpoint */
    USB_DEVICE_PRINTER_UNIDIRECTIONAL = 0x1,

    /* The bi-directional interface supports sending data to the printer
    via a Bulk OUT endpoint, and receiving status and other information 
    from the printer via a Bulk IN endpoint */
    USB_DEVICE_PRINTER_BIDIRECTIONAL = 0x2,    

    /* Just as the bi-directional interface and using the 1284.4 protocol */
    USB_DEVICE_PRINTER_IEEE1284_4 = 0x03    

} USB_DEVICE_PRINTER_IFACE_TYPE;

// *****************************************************************************
/* USB device Printer state enumeration.

  Summary:
    Identifies the different states Printer data transfer state machine.

  Description:
    This enumeration defines values for different Printer data transfer states.

  Remarks:
    None.
 */

typedef enum
{
    USB_DEVICE_PRINTER_STATE_DATA_IN,
    USB_DEVICE_PRINTER_STATE_DATA_OUT,
    USB_DEVICE_PRINTER_STATE_IDLE
	
} USB_DEVICE_PRINTER_STATE;

// *****************************************************************************
/* Printer instance structure.

  Summary:
    Identifies the Printer instance.

  Description:
    This type identifies the Printer instance.

  Remarks:
    This structure is internal to the Printer function driver.
*/
typedef struct
{
    /*  */
    USB_DEVICE_HANDLE deviceHandle;

    /* interface type */
    USB_DEVICE_PRINTER_IFACE_TYPE interfaceType;

    /* The IN bulk endpoint */
    USB_DEVICE_PRINTER_ENDPOINT bulkEndpointTx;

    /* The OUT bulk endpoint */
    USB_DEVICE_PRINTER_ENDPOINT bulkEndpointRx;

    /* The transmit IRP */
    USB_DEVICE_IRP irpTx;

    /* The receive IRP */
    USB_DEVICE_IRP irpRx;

    /* Application callback */
    USB_DEVICE_PRINTER_EVENT_HANDLER appEventCallBack;

    /* Application user data */
    uintptr_t userData;

    /* The current alternate setting for this
     * instance */
    uint8_t alternateSetting;

    /* Length of device ID string*/
    uint16_t deviceIDLength;

    /* device ID string */
    uint8_t deviceIDString[USB_DEVICE_PRINTER_DEVICE_ID_STRING_LENGTH];

    /* Transmit Queue Size */
    unsigned int queueSizeWrite;

    /* Receive Queue Size */
    unsigned int queueSizeRead;    

    /* Current Queue Size*/
    volatile unsigned int currentQSizeWrite;
    volatile unsigned int currentQSizeRead;

} USB_DEVICE_PRINTER_INSTANCE;

// *****************************************************************************
/* Printer Common data object

  Summary:
    Object used to keep track of data that is common to all instances of the
    Printer function driver.

  Description:
    This object is used to keep track of any data that is common to all
    instances of the Printer function driver.

  Remarks:
    None.
*/
typedef struct
{
    /* Set to true if all members of this structure
       have been initialized once */
    bool isMutexPrinterIrpInitialized;

    /* Mutex to protect client object pool */
    OSAL_MUTEX_DECLARE(mutexPrinterIRP);

} USB_DEVICE_PRINTER_COMMON_DATA_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: Printer specific functions
// *****************************************************************************
// *****************************************************************************

//******************************************************************************
/* Function:
    static void F_USB_DEVICE_PRINTER_ControlTransferHandler
    (
        SYS_MODULE_INDEX iPRN ,
        USB_DEVICE_CONTROL_TRANSFER_EVENT controlTransferEvent,
        void * controlTransferEventData
    );
 
  Summary:
    Handles end-point 0 requests.

  Description:
    This function handles ep0 requests.

  Remarks:
    Called by the device layer.
 */

void F_USB_DEVICE_PRINTER_ControlTransferHandler
(
    SYS_MODULE_INDEX iPRN ,
    USB_DEVICE_EVENT controlTransferEvent,
    USB_SETUP_PACKET * setupPacket
);


//******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_Initialization ( SYS_MODULE_INDEX iPRN ,
                                     DRV_HANDLE deviceHandle ,
                                     void* funcDriverInitData ,
                                     uint8_t infNum ,
                                     uint8_t altSetting ,
                                     uint8_t descType ,
                                     uint8_t * pDesc )
  Summary:
    Printer device class init function.

  Description:
    This function handles Printer initialization.

  Remarks:
    Called by the device layer per instance.
 */

void F_USB_DEVICE_PRINTER_Initialization
(
    SYS_MODULE_INDEX iPRN ,
    DRV_HANDLE deviceHandle ,
    void* funcDriverInitData ,
    uint8_t infNum ,
    uint8_t altSetting ,
    uint8_t descType ,
    uint8_t * pDesc 
);


// *****************************************************************************

/* Function:
    void	F_USB_DEVICE_PRINTER_Deinitialization (SYS_MODULE_INDEX iPRN)

  Summary:
    Printer function driver deinitialization.

  Description:
    This function deinitializes the specified instance of the Printer function driver.
    This function is called by the USB device layer.

  Precondition:
    None.

  Parameters:
    iPRN	- USB function driver index

  Returns:
    None.

  Example:
    <code>
    Called by the device layer.
    </code>

  Remarks:
    This function is internal to the USB stack. This API should not be
    called explicitly.
 */

void F_USB_DEVICE_PRINTER_Deinitialization ( SYS_MODULE_INDEX iPRN );

// ******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_GlobalInitialize ( void )

  Summary:
    This function initializes resourses required common to all instances of 
    Printer function driver.

 Description:
    This function initializes resourses common to all instances of Printer
    function driver. This function is called by the USB Device layer 
    during Initalization.

 Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Example:
    <code>
    Called by the device layer.
    </code>

  Remarks:
    This is local function and should not be called directly by the application.
*/
void F_USB_DEVICE_PRINTER_GlobalInitialize (void);

//******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_WriteIRPCallback (USB_DEVICE_IRP * irp )

  Summary:
    TX data callback.

  Description:
    This function handles TX data events 

  Remarks:
    Called by the controller driver 
 */

void F_USB_DEVICE_PRINTER_WriteIRPCallback (USB_DEVICE_IRP * irp );


//******************************************************************************
/* Function:
    void F_USB_DEVICE_PRINTER_ReadIRPCallback (USB_DEVICE_IRP * irp )

  Summary:
    RX data callback.

  Description:
    This function handles RX data events

  Remarks:
    Called by the controller driver
 */

void F_USB_DEVICE_PRINTER_ReadIRPCallback (USB_DEVICE_IRP * irp );

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _USB_DEVICE_PRINTER_LOCAL_H

/*******************************************************************************
 End of File
*/


