/*******************************************************************************
  USB Host HID Keyboard driver implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_keyboard.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host HID Keyboard driver.

  Description:
    This file contains the USB host HID Keyboard driver implementation. This
    file should be included in the project if USB HID Keyboard devices are to
    be supported.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>
#include "usb/usb_host_hid_keyboard.h"
#include "usb/src/usb_host_hid_keyboard_local.h"
#include "usb/src/usb_external_dependencies.h"

#include "usb/usb_host_hid.h"


/* Keyboard driver information on a per instance basis */
static USB_HOST_HID_KEYBOARD_DATA_OBJ keyboardData
        [USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER];
static USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardHandler;


// *****************************************************************************
/* Function:
    USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_EventHandlerSet
    (
        USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardEventHandler
    )
 
  Summary:
   Function registers application event handler with USB HID Keyboard driver
  
  Description:
   Function registers application event handler with USB HID Keyboard driver
  
  Remarks:
   Function registered should be of type USB_HOST_HID_KEYBOARD_EVENT_HANDLER.
*/
// *****************************************************************************
/* MISRA C-2012 Rule 5.1 deviate: 1
  Deviation record ID - H3_MISRAC_2012_R_5_1_DR_1*/
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block deviate:1 "MISRA C-2012 Rule 5.1" "H3_MISRAC_2012_R_5_1_DR_1"

USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_EventHandlerSet
(
    USB_HOST_HID_KEYBOARD_EVENT_HANDLER appKeyboardEventHandler
)
{
    /* Start of local variables */
    USB_HOST_HID_KEYBOARD_RESULT result = USB_HOST_HID_KEYBOARD_RESULT_INVALID_PARAMETER;
    /* End of local variables */
    
    if(NULL == appKeyboardEventHandler)
    {
        SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                "\r\nUSBHID Keyboard Driver: NULL Keyboard Application Handler");
    }
    else
    {
        appKeyboardHandler = appKeyboardEventHandler;
        result = USB_HOST_HID_KEYBOARD_RESULT_SUCCESS;
    }
    /*
     * USB_HOST_HID_KEYBOARD_RESULT_INVALID_PARAMETER: Invalid parameter
     * USB_HOST_HID_KEYBOARD_RESULT_SUCCESS: On success
     */
    return result;
} /* End of USB_HOST_HID_KEYBOARD_EventHandlerSet() */


// *****************************************************************************
/* Function:
    USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_ReportSend
    (
        USB_HOST_HID_KEYBOARD_HANDLE handle
        uint8_t outputReport
    )
 
  Summary:
   Function facilitates in sending OUTPUT report to Keyboard device
  
  Description:
   Function facilitates in sending OUTPUT report to Keyboard device
  
  Remarks:
   Function can be called only if LED Keys has been pressed
*/

USB_HOST_HID_KEYBOARD_RESULT USB_HOST_HID_KEYBOARD_ReportSend
(
    USB_HOST_HID_KEYBOARD_HANDLE handle,
    uint8_t outputReport
)
{
    /* Start of local variables */
    USB_HOST_HID_KEYBOARD_RESULT result = USB_HOST_HID_KEYBOARD_RESULT_INVALID_PARAMETER;
    USB_HOST_HID_RESULT status = USB_HOST_HID_RESULT_FAILURE;
    USB_HOST_HID_REQUEST_HANDLE requestHandle = USB_HOST_HID_REQUEST_HANDLE_INVALID;
    uint8_t loop = 0;
    /* End of local variables */
    for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER; loop++)
    {
        if(keyboardData[loop].inUse && 
                (keyboardData[loop].handle == handle))
        {
            /* Found the Keyboard data object */
            break;
        }
    }
    if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
    {
        /* Copy the Report Data */
        keyboardData[loop].outputReport = outputReport;
        /* Send Report to Keyboard device */
        status = USB_HOST_HID_ReportSend
                (
                    /* Keyboard driver handle */
                    handle,
                    /* OUTPUT report */
                    USB_HID_REPORT_TYPE_OUTPUT,
                    /* Report ID for OUTPUT report */
                    keyboardData[loop].outputReportID,
                    /* Number of bytes */
                    1,
                    /* Request Handle */
                    &requestHandle,
                    /* Report Data */
                    &(keyboardData[loop].outputReport)
                );
        if(status == USB_HOST_HID_RESULT_SUCCESS)
        {
            result = USB_HOST_HID_KEYBOARD_RESULT_SUCCESS;
        }
        else if(status == USB_HOST_HID_RESULT_REQUEST_BUSY)
        {
            result = USB_HOST_HID_KEYBOARD_RESULT_REQUEST_BUSY;
        }
        else
        {
            result = USB_HOST_HID_KEYBOARD_RESULT_FAILURE;
        }
    }
    else
    {
        /* Keyboard driver instance corresponding to handle not found */
        SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                "\r\nUSBHID Keyboard Driver: Invalid Keyboard Handle");
    }
    return result;
    
}/* End of USB_HOST_HID_KEYBOARD_ReportSend() */


// *****************************************************************************
/* Function:
    void USB_HOST_HID_KEYBOARD_EventHandler
    (
        USB_HOST_HID_OBJ_HANDLE handle,
        USB_HOST_HID_EVENT event,
        void * eventData
    )
 
  Summary:
    Keyboard driver event handler function registered with USB HID client driver
  
  Description:
    Keyboard driver event handler function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void USB_HOST_HID_KEYBOARD_EventHandler
(
    USB_HOST_HID_OBJ_HANDLE handle,
    USB_HOST_HID_EVENT event,
    void * eventData
)
{
    /* Start  of local variables */
    uint8_t loop = 0;
    uint8_t index = 0;
    /* End of local variables */
    
    if(handle != USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        switch(event)
        {
            case USB_HOST_HID_EVENT_ATTACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(!keyboardData[loop].inUse)
                    {
                        /* Grab the pool */
                        keyboardData[loop].inUse = true;
                        
                        /* Reset necessary data structures */
                        keyboardData[loop].handle = handle;
                        keyboardData[loop].index = 0;
                        keyboardData[loop].counter = 0;
                        keyboardData[loop].outputReportID = 0;
                        keyboardData[loop].state = USB_HOST_HID_KEYBOARD_ATTACHED;
                        
                        (void) memset(&keyboardData[loop].usageDriverData, 0,
                                sizeof(USB_HOST_HID_KEYBOARD_DATA));
                        (void) memset(&keyboardData[loop].lastKeyCode, 0,
                                sizeof(keyboardData[loop].lastKeyCode));
                       (void) memset((void *)keyboardData[loop].buffer, 0,
                                sizeof(keyboardData[loop].buffer));
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appKeyboardHandler != NULL)
                    {
                        appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_ATTACH,
                                NULL);
                    }
                }
                else
                {
                    SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                            "\r\nUSBHID Keyboard Driver: No free entry in Keyboard pool available");
                }
            
                break;
            
            case USB_HOST_HID_EVENT_DETACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(keyboardData[loop].inUse && 
                            (keyboardData[loop].handle == handle))
                    {
                        /* Release the pool object */
                        keyboardData[loop].inUse = false;
                        keyboardData[loop].state = USB_HOST_HID_KEYBOARD_DETACHED;
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appKeyboardHandler != NULL)
                    {
                        appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_DETACH,
                                NULL);
                    }
                    for(index = 0; index < USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE;
                            index++)
                    {
                        /* Reset the flag for all queue entries */
                        keyboardData[loop].buffer[index].tobeDone = false;
                    }
                }
                else
                {
                    SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                            "\r\nUSBHID Keyboard Driver: Invalid Keyboard Handle");
                }
            
                break;
            
            case USB_HOST_HID_EVENT_REPORT_RECEIVED:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(keyboardData[loop].inUse && 
                            (keyboardData[loop].handle == handle))
                    {
                        /* Found the Keyboard data object */
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    (void) memcpy((void *)keyboardData[loop].buffer[keyboardData[loop].index].data,
                                    (const void *)eventData, 64);
                    
                    keyboardData[loop].state = 
                                    USB_HOST_HID_KEYBOARD_REPORT_PROCESS;
                    /*
                     * The keyboard driver maintains a buffer where the IN
                     * Report data are stored. The Report data is processed
                     * in task context later. By this approach we eliminate
                     * any potential overwriting of IN data
                     */
                    keyboardData[loop].buffer[keyboardData[loop].index].tobeDone
                            = true;
                    /* Increase the queue index. This value will be used for
                     * next IN Report data storage.
                     */
                    keyboardData[loop].index++;

                    /* If reached the end of queue, reset the index to start
                     * from beginning of the queue in next iteration.*/
                    if(keyboardData[loop].index ==
                            USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE)
                    {
                        /* Reset it to 0 for next iteration */
                        keyboardData[loop].index = 0;

                        /* Reset the flag to false, so that in task routine
                         * we do not process this queue data now. The
                         * queue data will be processed only when there
                         * is valid data for index 0 */
                        keyboardData[loop].buffer[keyboardData[loop].index].tobeDone
                            = false;
                    }
                }
                else
                {
                    SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                            "\r\nUSBHID Keyboard Driver: Invalid Keyboard Handle");
                }

                break;
            
            default:
                /* Do Nothing */
                break;
        
        } /* end of switch() */
    } /* end of if(Valid Handle) */
    else
    {
        SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                "\r\nUSBHID Keyboard Driver: Invalid Keyboard Handle");
    }
} /* End of F_USB_HOST_HID_KEYBOARD_EventHandler() */

#pragma coverity compliance end_block "MISRA C-2012 Rule 5.1"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */

// *****************************************************************************
/* Function:
    void USB_HOST_HID_KEYBOARD_Task(USB_HOST_HID_OBJ_HANDLE handle)
 
  Summary:
    Keyboard driver task routine function registered with USB HID client driver
  
  Description:
    Keyboard driver task routine function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void USB_HOST_HID_KEYBOARD_Task(USB_HOST_HID_OBJ_HANDLE handle)
{
    /* Start of local variables */
    USB_HOST_HID_LOCAL_ITEM localItem = {.delimiterBranch = 0};
    USB_HOST_HID_GLOBAL_ITEM globalItem = {.reportSize = 0};
    USB_HOST_HID_MAIN_ITEM mainItem = {.localItem = NULL};
    
    uint64_t keyboardDataBufferTemp = 0;
    uint8_t * keyboardDataBuffer = NULL;
    uint8_t *ptr = NULL;
    uint8_t dataTemp[64] = {0};
    
    uint32_t reportOffset = 0;
    uint32_t currentReportOffsetTemp = 0;
    uint32_t usage = 0;
    uint32_t count = 0;
    
    uint8_t index = 1;
    uint8_t loop = 0;
    uint8_t i = 0;
    uint8_t keyboardIndex = 0;
    
    uint8_t counter = 0;
    bool lastKeyFound = false;
    bool tobeDone = false;
    uint32_t temp_32;
    USB_HOST_HID_RESULT result = USB_HOST_HID_RESULT_FAILURE;
    /* End of local variables */
    
    if(handle == USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        return;
    }
    for(keyboardIndex = 0; keyboardIndex < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
            keyboardIndex++)
    {
        if(keyboardData[keyboardIndex].inUse && (keyboardData[keyboardIndex].handle == handle))
        {
            /* Found the Keyboard data object */
            break;
        }
    }
    if(keyboardIndex == USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
    {
        /* Keyboard index corresponding to the handle not found */
        SYS_DEBUG_MESSAGE (SYS_ERROR_INFO,
                "\r\nUSBHID Keyboard Driver: Keyboard instance corresponding to handle not found");
        return;
    }
    /* Allocate global and local items buffer memory */
    mainItem.globalItem = &globalItem;
    mainItem.localItem = &localItem;
    
    switch(keyboardData[keyboardIndex].state)
    {
        case USB_HOST_HID_KEYBOARD_DETACHED:
            break;
        case USB_HOST_HID_KEYBOARD_ATTACHED:
            break;
        case USB_HOST_HID_KEYBOARD_REPORT_PROCESS:
            tobeDone = false;
            /*
             * Keyboard driver processes the IN Report on a sequential fashion
             * starting from counter = 0 to 
             * (USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE - 1). The processing
             * starts from index 0 once maximum queue size is reached.
             */
            counter = keyboardData[keyboardIndex].counter;
            if(counter == USB_HOST_HID_KEYBOARD_BUFFER_QUEUE_SIZE)
            {
                keyboardData[keyboardIndex].counter = 0;
                counter = 0;
            }
            /* Check if this buffer needs to be processed. tobeDone will
             * be set to true from ISR context.
             */
            if(keyboardData[keyboardIndex].buffer[counter].tobeDone)
            {
                /* Keep a temp backup of the data */
                (void) memcpy(&dataTemp,(const uint8_t *)keyboardData[keyboardIndex].buffer
                        [counter].data, 64);
                tobeDone = true;
            }
            
            if(tobeDone)
            {
                /* Increment the queue counter. Next task iteration the
                 * processing will start from here.
                 */
                keyboardData[keyboardIndex].counter++;
                /* Reset global items only once as they are applicable
                 * through out */
                (void) memset(&globalItem, 0,
                            (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                /* Reset the app Data otherwise key count will be an issue. Also
                 data from last report if exists will lead to false key press
                 or release event */
                (void) memset(&(keyboardData[keyboardIndex].usageDriverData), 0,
                        (size_t)sizeof(USB_HOST_HID_KEYBOARD_DATA));
                do
                {
                    /* Reset the field data except global items */
                    (void) memset(&localItem, 0, 
                            (size_t)sizeof(USB_HOST_HID_LOCAL_ITEM));
                    (void) memset(&(mainItem.data), 0,
                            (size_t)sizeof(USB_HID_MAIN_ITEM_OPTIONAL_DATA));
                    mainItem.tag = USB_HID_REPORT_TYPE_ERROR;

                    /* Start enquiring for the main items starting from
                     * index = 1. index = 0 is invalid to HID client driver. */
                    result = USB_HOST_HID_MainItemGet(handle,index,&mainItem);

                    if(result == USB_HOST_HID_RESULT_SUCCESS)
                    {
                        /* Copy the data as while processing the last field
                         we have changed the data for numbered report.*/
                        (void) memcpy((void *)keyboardData[keyboardIndex].buffer
                                [counter].data,
                                (const void *)&dataTemp, 64);
                        
                        if((uint32_t)mainItem.tag ==
                                (uint32_t)USB_HID_MAIN_ITEM_TAG_BEGIN_COLLECTION)
                        {                
                            /* Do not change report offset as they do not
                             * create data fields. Just reset the global item
                             * data.
                             *
                             * Also we will not reset the Global item as
                             * they are applicable across collections.
                             */
                        }/* Main item = BEGIN COLLECTION */
                        
                        else if((uint32_t)mainItem.tag == 
                                (uint32_t)USB_HID_MAIN_ITEM_TAG_END_COLLECTION)
                        {
                            /* Do not change report offset as they do not
                             * create data fields*/
                        }/* Main item = END COLLECTION */
                        
                        else if((uint32_t)mainItem.tag == (uint32_t)USB_HID_MAIN_ITEM_TAG_INPUT)
                        {
                            if(((mainItem.globalItem)->reportCount == 0U))
                            {
                                /* Try looking for the next main item as this
                                 * item will not create any data field.
                                 */
                                index++;
                                continue;
                            }

                            if(!((mainItem.globalItem)->reportID == 0U))
                            {
                                /* Numbered report */
                                if(keyboardData[keyboardIndex].buffer
                                        [counter].data[0] !=
                                        (mainItem.globalItem)->reportID)
                                {
                                        /* Report ID does not match. No point in
                                           parsing this data */
                                        index++;
                                        continue;
                                }
                                /* Numbered Report. Shift right by 1 byte */
                                for(loop = 0; loop < 64U; loop ++)
                                {
                                    if(loop == 63U)
                                    {
                                        keyboardData[keyboardIndex].buffer
                                                [counter].data[loop] = 0;
                                        break;
                                    }
                                    keyboardData[keyboardIndex].buffer
                                            [counter].data[loop] = 
                                            keyboardData[keyboardIndex].buffer
                                            [counter].data[loop + 1U];
                                    
                                }
                            } /* end of if numbered report */
                            
                            keyboardDataBuffer = keyboardData[keyboardIndex].buffer
                                                    [counter].data;
                            
                            currentReportOffsetTemp = reportOffset;
                            reportOffset = ((reportOffset) + 
                                    ((mainItem.globalItem)->reportCount) *
                                    ((mainItem.globalItem)->reportSize));
                            
                            /* Keyboard keys handling logic*/
                            if((((0xFF00U & (uint32_t)(mainItem.globalItem)->usagePage)>>8) 
                                    == (uint32_t)USB_HID_USAGE_PAGE_KEYBOARD_KEYPAD) || 
                                    ((0x00FFU & (uint32_t)(mainItem.globalItem)->usagePage)
                                        == (uint32_t)USB_HID_USAGE_PAGE_KEYBOARD_KEYPAD))
                            {
                                if((mainItem.data.inputOptionalData.isConstant == 0U) &&
                                        (mainItem.data.inputOptionalData.isVariable != 0U))
                                {
                                    /* Modifier byte */
                                    if(mainItem.localItem->usageMinMax.valid)
                                    {
                                        usage = mainItem.localItem->usageMinMax.min;
                                        while(usage <= 
                                                (mainItem.localItem->usageMinMax.max))
                                        {
                                            keyboardDataBuffer = keyboardData[keyboardIndex].buffer
                                                    [counter].data;

                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_CONTROL)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.leftControl = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_SHIFT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.leftShift = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_ALT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.leftAlt = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_LEFT_GUI)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.leftGui = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_CONTROL)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.rightControl = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_SHIFT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.rightShift = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_ALT)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.rightAlt = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            if((0x00FFU & usage) == (uint32_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_RIGHT_GUI)
                                            {
                                                keyboardDataBuffer = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                                keyboardData[keyboardIndex].usageDriverData.modifierKeysData.rightGui = 
                                                    (uint8_t)((((uint32_t)*keyboardDataBuffer) >> (currentReportOffsetTemp % 8U))
                                                            & 0x01U);
                                            }
                                            /* Move to the next usage */
                                            usage++;
                                            /* Update the report offset */
                                            currentReportOffsetTemp = 
                                                currentReportOffsetTemp + 
                                                (mainItem.globalItem)->reportSize;
                                        } /* end of while(all usages) */
                                    } /* Usage Min Max present */
                                } /* end of if Modifier bytes */

                                else if((mainItem.data.inputOptionalData.isConstant == 0U)
                                        && (mainItem.data.inputOptionalData.isVariable == 0U))
                                {
                                    /* Non Modifier keys */
                                    
                                    /* Starting from currentReportOffsetTemp compare
                                     report size of data for report count times.
                                     If present in current it is KEY PRESS event.
                                     If it is present in past but not present in
                                     current, it is KEY RELEASE */
                                    
                                    do
                                    {
                                        count++;
                                        ptr = keyboardDataBuffer + (currentReportOffsetTemp/8U);
                                        
                                        keyboardDataBufferTemp = 0;
                                        if((mainItem.globalItem)->reportSize >= 8U)
                                        {
                                            for (i = 0; i < ((mainItem.globalItem)->reportSize/8U); i++)
                                            {
                                                keyboardDataBufferTemp = (keyboardDataBufferTemp | ((uint64_t)ptr[i] << (i * 8U)));
                                            }
                                            /* The reason why we do this is we
                                             are copying some bits extra. Hence
                                             now we need to remove those bits*/
                                            keyboardDataBufferTemp = keyboardDataBufferTemp >> (currentReportOffsetTemp % 8U);
                                            keyboardDataBufferTemp = keyboardDataBufferTemp & (uint64_t)0xFFFFFFFFU;
                                        }
                                        else
                                        {
                                            keyboardDataBufferTemp = (uint64_t)keyboardDataBufferTemp | (uint64_t)ptr[0];
                                            
                                            keyboardDataBufferTemp = (uint64_t)keyboardDataBufferTemp << (8U - (mainItem.globalItem)->reportSize);
                                                    
                                            keyboardDataBufferTemp = (uint64_t)keyboardDataBufferTemp >> (8U - (mainItem.globalItem)->reportSize);
                                                    
                                        }
                                        if((keyboardDataBufferTemp != (uint64_t)USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED)
                                                &&(keyboardDataBufferTemp != (uint64_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_ROLL_OVER)
                                                &&(keyboardDataBufferTemp != (uint64_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_POST_FAIL)
                                                &&(keyboardDataBufferTemp != (uint64_t)USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_UNDEFINED))
                                        {
                                            /* Valid key press detected */
                                            keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].keyCode
                                                    = (USB_HID_KEYBOARD_KEYPAD) keyboardDataBufferTemp;
                                            keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].event
                                                    = USB_HID_KEY_PRESSED;
                                            
                                            keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].sysCount
                                                    = SYS_TIME_CounterGet();
                                            keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData++;
                                        }
                                    
                                        /* Update the report offset */
                                        currentReportOffsetTemp = 
                                            currentReportOffsetTemp + 
                                            (mainItem.globalItem)->reportSize;
                                        
                                    } while(count < (mainItem.globalItem)->reportCount);
                                    
                                    for(count = 0; count < 6U; count++)
                                    {
                                        /* If it is present in the past
                                         but not in current, it is key release */
                                        if(keyboardData[keyboardIndex].lastKeyCode[count]  >
                                                USB_HID_KEYBOARD_KEYPAD_KEYBOARD_ERROR_UNDEFINED)
                                        {
                                            for(i=0; i < 6U; i++)
                                            {
                                                if(keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData[i].keyCode
                                                        == keyboardData[keyboardIndex].lastKeyCode[count])
                                                {
                                                    lastKeyFound = true;
                                                    break;
                                                }
                                            }
                                            if(lastKeyFound == false)
                                            {
                                                keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].keyCode
                                                    = (USB_HID_KEYBOARD_KEYPAD)keyboardData[keyboardIndex].lastKeyCode[count];
                                                keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].event
                                                    = USB_HID_KEY_RELEASED;
                                                temp_32 = SYS_TIME_CounterGet();
                                                keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData
                                                    [keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData].event
                                                    = (USB_HID_KEY_EVENT)temp_32;
                                                keyboardData[keyboardIndex].usageDriverData.nNonModifierKeysData++;
                                            }
                                            else
                                            {
                                                /* Reset the flag to false for
                                                 next iteration */
                                                lastKeyFound = false;
                                            }
                                        }
                                    }
                                    for(count = 0; count < 6U; count++)
                                    {
                                        /* Save the present key state for next
                                         * processing */
                                        keyboardData[keyboardIndex].lastKeyCode[count] = USB_HID_KEYBOARD_KEYPAD_RESERVED_NO_EVENT_INDICATED;
                                        if(keyboardData[keyboardIndex].usageDriverData
                                                .nonModifierKeysData[count].event == USB_HID_KEY_PRESSED)
                                        {
                                            keyboardData[keyboardIndex].lastKeyCode[count] =
                                                keyboardData[keyboardIndex].usageDriverData.nonModifierKeysData[count].keyCode;
                                        }
                                    }
                                }
                                else
                                {
                                    /* Do Nothing */
                                }

                            } /* Keyboard/Keypad page */
                            else
                            {
                                /* Do Nothing */
                            }
                        }/* Main item = INPUT */

                        else if((uint32_t)mainItem.tag ==
                                    (uint32_t)USB_HID_MAIN_ITEM_TAG_OUTPUT)
                        {
                            /* Change output report offset as they
                             * create data fields*/
                            if((mainItem.data.inputOptionalData.isConstant == 0U) &&
                                    (mainItem.data.inputOptionalData.isVariable != 0U) &&
                                    (mainItem.data.inputOptionalData.isRelative == 0U))
                            {
                                keyboardData[keyboardIndex].outputReportID = 
                                        (uint8_t)(mainItem.globalItem)->reportID;
                            }
                        }
                        else if((uint32_t)mainItem.tag ==
                                    (uint32_t)USB_HID_MAIN_ITEM_TAG_FEATURE)
                        {
                            /* Change feature report offset as they
                             * create data fields*/
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }/* Field found */

                    index++;
                    
                } while(result == USB_HOST_HID_RESULT_SUCCESS);
                
                keyboardData[keyboardIndex].buffer[counter].tobeDone = false;
                
                if(appKeyboardHandler != NULL)
                {
                    appKeyboardHandler((USB_HOST_HID_KEYBOARD_HANDLE)handle,
                                USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED,
                                &keyboardData[keyboardIndex].usageDriverData);
                }

            }/* end of report processing */
            break;
        default:
            /* Do Nothing */
            break;
    }
}/* End of USB_HOST_HID_KEYBOARD_Task() */

