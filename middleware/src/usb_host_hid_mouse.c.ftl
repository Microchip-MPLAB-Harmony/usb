/*******************************************************************************
  USB Host HID Mouse driver implementation.

  Company:
    Microchip Technology Inc.

  File Name:
    usb_host_hid_mouse.c

  Summary:
    This file contains implementations of both private and public functions
    of the USB Host HID Mouse driver.

  Description:
    This file contains the USB host HID Mouse driver implementation. This file 
    should be included in the project if USB HID Mouse devices are to be supported.
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
#include "usb/usb_host_hid_mouse.h"
#include "usb/src/usb_host_hid_mouse_local.h"
#include "usb/src/usb_external_dependencies.h"
#include "usb/usb_host_hid.h"


/* Mouse driver information on a per instance basis */
static USB_HOST_HID_MOUSE_DATA_OBJ mouseData[USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER];
static USB_HOST_HID_MOUSE_EVENT_HANDLER appMouseHandler;

/* MISRA C-2012 Rule 16.1 deviate: 1, and 16.3 deviate:1. 
  Deviation record ID - H3_MISRAC_2012_R_11_1_DR_1, H3_MISRAC_2012_R_11_8_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block \
(deviate:3 "MISRA C-2012 Rule 16.1" "H3_MISRAC_2012_R_16_1_DR_1" )\
(deviate:2 "MISRA C-2012 Rule 5.1" "H3_MISRAC_2012_R_5_1_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 16.3" "H3_MISRAC_2012_R_16_3_DR_1" )
</#if>
// *****************************************************************************
/* Function:
    USB_HOST_HID_MOUSE_RESULT USB_HOST_HID_MOUSE_EventHandlerSet
    (
        USB_HOST_HID_MOUSE_EVENT_HANDLER appMouseHandler
    )
 
  Summary:
   Function registers application event handler with USB HID Mouse driver
  
  Description:
   Function registers application event handler with USB HID Mouse driver
  
  Remarks:
    Function registered should be of type USB_HOST_HID_MOUSE_EVENT_HANDLER.
*/

USB_HOST_HID_MOUSE_RESULT USB_HOST_HID_MOUSE_EventHandlerSet
(
    USB_HOST_HID_MOUSE_EVENT_HANDLER appMouseEventHandler
)
{
    if(NULL == appMouseEventHandler)
    {
        return USB_HOST_HID_MOUSE_RESULT_INVALID_PARAMETER;
    }
    else
    {
        appMouseHandler = appMouseEventHandler;
    }
    return USB_HOST_HID_MOUSE_RESULT_SUCCESS;
} /* End of USB_HOST_HID_MOUSE_EventHandlerSet() */


// *****************************************************************************

/* Function:
    void USB_HOST_HID_MOUSE_EventHandler
    (
        USB_HOST_HID_OBJ_HANDLE handle,
        USB_HOST_HID_EVENT event,
        void * eventData
    )
 
  Summary:
    Mouse driver event handler function registered with USB HID client driver
  
  Description:
    Mouse driver event handler function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void USB_HOST_HID_MOUSE_EventHandler
(
    USB_HOST_HID_OBJ_HANDLE handle,
    USB_HOST_HID_EVENT event,
    void * eventData
)
{
    /* Start  of local variables */
    uint8_t loop = 0;
    /* End of local variables */
    
    if(handle != USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        switch(event)
        {
            case USB_HOST_HID_EVENT_ATTACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(!mouseData[loop].inUse)
                    {
                        /* Grab the pool */
                        mouseData[loop].inUse = true;
                        /* Reset necessary data structures */
                        mouseData[loop].handle = handle;
                        mouseData[loop].state = USB_HOST_HID_MOUSE_ATTACHED;
                        mouseData[loop].nextPingPong = false;
                        mouseData[loop].taskPingPong = false;
                        mouseData[loop].isPingReportProcessing = false;
                        mouseData[loop].isPongReportProcessing = false;
                        (void) memset((void *)&mouseData[loop].usageDriverData, 0,
                                sizeof(USB_HOST_HID_MOUSE_DATA));
                        (void) memset((void *)mouseData[loop].dataPing, 0,64);
                        (void) memset((void *)mouseData[loop].dataPong, 0,64);
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appMouseHandler != NULL)
                    {
                        appMouseHandler((USB_HOST_HID_MOUSE_HANDLE)handle,
                                USB_HOST_HID_MOUSE_EVENT_ATTACH,
                                NULL);
                    }
                }
                break;
            case USB_HOST_HID_EVENT_DETACH:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(mouseData[loop].inUse && 
                            (mouseData[loop].handle == handle))
                    {
                        /* Release the pool object */
                        mouseData[loop].inUse = false;
                        mouseData[loop].state = USB_HOST_HID_MOUSE_DETACHED;
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(appMouseHandler != NULL)
                    {
                        appMouseHandler((USB_HOST_HID_MOUSE_HANDLE)handle,
                                USB_HOST_HID_MOUSE_EVENT_DETACH,
                                NULL);
                    }
                }
                break;
            case USB_HOST_HID_EVENT_REPORT_RECEIVED:
                for(loop = 0; loop < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        loop++)
                {
                    if(mouseData[loop].inUse && 
                            (mouseData[loop].handle == handle))
                    {
                        /* Found the Mouse data object */
                        break;
                    }
                }
                if(loop != USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
                {
                    if(mouseData[loop].nextPingPong)
                    {
                        if((mouseData[loop].isPongReportProcessing == false))
                        {
                            /* There is no ongoing report processing for this Mouse
                            driver instance for Pong buffer. */
                            mouseData[loop].nextPingPong = false;
                            mouseData[loop].isPongReportProcessing = true;
                            (void) memcpy((void *)mouseData[loop].dataPong, (const void *)eventData,
                                    64);
                            mouseData[loop].state =
                                    USB_HOST_HID_MOUSE_REPORT_PROCESS;
                        }
                    }
                    else
                    {
                        if((mouseData[loop].isPingReportProcessing == false))
                        {
                            /* There is no ongoing report processing for this Mouse
                            driver instance for Ping buffer. */
                            mouseData[loop].nextPingPong = true;
                            mouseData[loop].isPingReportProcessing = true;
                            (void) memcpy((void *)mouseData[loop].dataPing, (const void *)eventData,
                                    64);
                            mouseData[loop].state =
                                    USB_HOST_HID_MOUSE_REPORT_PROCESS;
                        }
                    }
                }
            default:
                /* Do Nothing */
                break;
        }
    }
} /* End of USB_HOST_HID_MOUSE_EventHandler() */

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 16.1"
#pragma coverity compliance end_block "MISRA C-2012 Rule 16.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 5.1"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */
// *****************************************************************************
/* MISRA C-2012 Rule 21.15 deviated:1 Deviation record ID -  H3_MISRAC_2012_R_11_3_DR_1 */
<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
</#if>
#pragma coverity compliance block deviate:1 "MISRA C-2012 Rule 21.15" "H3_MISRAC_2012_R_21_15_DR_1"
</#if>
/* Function:
    void USB_HOST_HID_MOUSE_Task(USB_HOST_HID_OBJ_HANDLE handle)
 
  Summary:
    Mouse driver task routine function registered with USB HID client driver
  
  Description:
    Mouse driver task routine function registered with USB HID client driver
  
  Remarks:
    This is a local function and should not be called by application directly.
*/

void USB_HOST_HID_MOUSE_Task(USB_HOST_HID_OBJ_HANDLE handle)
{
    /* Start of local variables */
    USB_HOST_HID_LOCAL_ITEM localItem = {.delimiterBranch = 0};
    USB_HOST_HID_GLOBAL_ITEM globalItem = {.reportSize = 0};
    USB_HOST_HID_MAIN_ITEM mainItem = {.localItem = NULL};
    
    uint64_t mouseDataBufferTemp = 0;
    uint8_t * mouseDataBuffer = NULL;
    uint8_t *ptr = NULL;
    uint8_t dataTemp[64] ={0};
    
    uint32_t reportOffset = 0;
    uint32_t currentReportOffset = 0;
    uint32_t currentReportOffsetTemp = 0;
    uint32_t andMask = 0;
    uint32_t usage = 0;
    
    uint8_t index = 1;
    uint8_t loop = 0;
    uint8_t mouseIndex = 0;
    uint8_t i=0;
    bool tobeDone = false;
    USB_HOST_HID_RESULT result = USB_HOST_HID_RESULT_FAILURE;
    uint8_t temp_8;
    /* End of local variables */
    
    if(handle == USB_HOST_HID_OBJ_HANDLE_INVALID)
    {
        return;
    }
    for(mouseIndex = 0; mouseIndex < USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER;
                        mouseIndex++)
    {
        if(mouseData[mouseIndex].inUse && (mouseData[mouseIndex].handle == handle))
        {
            /* Found the Mouse data object */
            break;
        }
    }
    if(mouseIndex == USB_HOST_HID_USAGE_DRIVER_SUPPORT_NUMBER)
    {
        /* Mouse index corresponding to the handle not found */
        return;
    }
    mainItem.globalItem = &globalItem;
    mainItem.localItem = &localItem;
    
    switch(mouseData[mouseIndex].state)
    {
        case USB_HOST_HID_MOUSE_DETACHED:
            break;
        case USB_HOST_HID_MOUSE_ATTACHED:
            break;
        case USB_HOST_HID_MOUSE_REPORT_PROCESS:
            tobeDone = false;
            if(!mouseData[mouseIndex].taskPingPong)
            {
                if(mouseData[mouseIndex].isPingReportProcessing == true)
                {
                    mouseData[mouseIndex].taskPingPong = true;
                    /* Keep a temp backup of the data */
                    (void) memcpy(&dataTemp,
                            (const void *)mouseData[mouseIndex].dataPing, 64);
                    /* Reset global items only once as they are applicable through out */
                    (void) memset(&globalItem, 0,
                            (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                    tobeDone = true;
                }
            }
            else
            {
                if(mouseData[mouseIndex].isPongReportProcessing == true)
                {
                    mouseData[mouseIndex].taskPingPong = false;
                    /* Keep a temp backup of the data */
                    (void) memcpy(&dataTemp,
                            (const void *)mouseData[mouseIndex].dataPong, 64);
                    /* Reset global items only once as they are applicable through out */
                    (void) memset(&globalItem, 0,
                            (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                    tobeDone = true;
                }
            }
            if(tobeDone)
            {
                do
                {
                    /* Reset the field data except global items */
                    (void) memset(&localItem, 0, 
                            (size_t)sizeof(USB_HOST_HID_LOCAL_ITEM));
                    (void) memset(&(mainItem.data), 0,
                            (size_t)sizeof(USB_HID_MAIN_ITEM_OPTIONAL_DATA));
                    mainItem.tag = USB_HID_REPORT_TYPE_ERROR;
                    
                    result = USB_HOST_HID_MainItemGet(handle,index,&mainItem);
                    if(result == USB_HOST_HID_RESULT_SUCCESS)
                    {
                        /* Copy the data as while processing the last field
                         we have changed the data for numbered report. If
                         taskPingPong is true that means we need to process
                         Ping buffer, else Pong buffer */
                        if(mouseData[mouseIndex].taskPingPong)
                        {
                            (void) memcpy((void *)mouseData[mouseIndex].dataPing,
                                    (const void *)&dataTemp, 64);
                        }
                        else
                        {
                            (void) memcpy((void *)mouseData[mouseIndex].dataPong,
                                    (const void *)&dataTemp, 64);
                        }
                        if((uint32_t)mainItem.tag ==
                                (uint32_t)USB_HID_MAIN_ITEM_TAG_BEGIN_COLLECTION)
                        {    
                            /* Do not change report offset as they do not
                             * create data fields. Just reset the global item
                             * data */
                            (void) memset(&globalItem, 0,
                                    (size_t)sizeof(USB_HOST_HID_GLOBAL_ITEM));
                        }/* Main item = BEGIN COLLECTION */
                        
                        else if((uint32_t)mainItem.tag == 
                                (uint32_t)USB_HID_MAIN_ITEM_TAG_END_COLLECTION)
                        {
                            /* Do not change report offset as they do not
                             *  create data fields*/
                        }/* Main item = END COLLECTION */
                        
                        else if((uint32_t)mainItem.tag == (uint32_t)USB_HID_MAIN_ITEM_TAG_INPUT)
                        {
                            if(((mainItem.globalItem)->reportCount == 0U))
                            {
                                index++;
                                continue;
                            }
                            if(!((mainItem.globalItem)->reportID == 0U))
                            {
                                /* Numbered report */
                                if(mouseData[mouseIndex].taskPingPong)
                                {
                                    if(mouseData[mouseIndex].dataPing[0] !=
                                        (mainItem.globalItem)->reportID)
                                    {
                                        /* Report ID does not match */
                                        index++;
                                        continue;
                                    }
                                    /* Numbered Report. Shift right by 1 byte */
                                    for(loop = 0; loop <64U; loop ++)
                                    {
                                        if(loop == 63U)
                                        {
                                            mouseData[mouseIndex].dataPing[loop] = 0;
                                            break;
                                        }
                                        mouseData[mouseIndex].dataPing[loop] = 
                                                mouseData[mouseIndex].dataPing[loop + 1U];
                                    }
                                }
                                else
                                {
                                    if(mouseData[mouseIndex].dataPong[0] !=
                                        (mainItem.globalItem)->reportID)
                                    {
                                        /* Report ID does not match */
                                        index++;
                                        continue;
                                    }
                                    /* Numbered Report. Shift right by 1 byte */
                                    for(loop = 0; loop <64U; loop ++)
                                    {
                                        if(loop == 63U)
                                        {
                                            mouseData[mouseIndex].dataPong[loop] = 0;
                                            break;
                                        }
                                        mouseData[mouseIndex].dataPong[loop] = 
                                                mouseData[mouseIndex].dataPong[loop + 1U];
                                    }
                                }
                            }

                            if(mouseData[mouseIndex].taskPingPong)
                            {
                                mouseDataBuffer = mouseData[mouseIndex].dataPing;
                            }
                            else
                            {
                                mouseDataBuffer = mouseData[mouseIndex].dataPong;
                            }
                            
                            currentReportOffset = reportOffset;
                            currentReportOffsetTemp = currentReportOffset;
                            reportOffset = (reportOffset) + 
                                    ((mainItem.globalItem)->reportCount) *
                                    ((mainItem.globalItem)->reportSize);
                            
                            /* Mouse button handling logic*/
                            if((( (0xFF00U & (uint32_t)(mainItem.globalItem)->usagePage) >> 8 )
                                    ==  (uint32_t)USB_HID_USAGE_PAGE_BUTTON   ) || 
                                    ((0x00FFU & (uint32_t)(mainItem.globalItem)->usagePage)
                                        == (uint32_t)USB_HID_USAGE_PAGE_BUTTON))
                            {
                                if(mainItem.localItem->usageMinMax.valid)
                                {
                                    usage = mainItem.localItem->usageMinMax.min;
                                    loop = 0;
                                    while(usage <= 
                                            (mainItem.localItem->usageMinMax.max))
                                    {
                                        if(mouseData[mouseIndex].taskPingPong)
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPing;
                                        }
                                        else
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPong;
                                        }
                                        if(((0x00FFU & usage) == (uint32_t)USB_HID_USAGE_ID_BUTTON1) &&
                                                (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                        {
                                            mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                            temp_8 = ((((*mouseDataBuffer)) >> (currentReportOffsetTemp % 8U))
                                                        & 0x01U);
                                            mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                (USB_HID_BUTTON_STATE)temp_8;
                                            mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                    USB_HID_USAGE_ID_BUTTON1;
                                            loop++;
                                        }
                                        if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON2) && 
                                        (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                        {
                                            mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                            temp_8 = ((((*mouseDataBuffer)) >> (currentReportOffsetTemp % 8U))
                                                        & 0x01U);
                                            mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                (USB_HID_BUTTON_STATE) temp_8;                                                
                                            mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                    USB_HID_USAGE_ID_BUTTON2;
                                            loop++;
                                        }
                                        if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON3) && 
                                        (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                        {
                                            mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                            temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                        & 0x01U);
                                            mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                (USB_HID_BUTTON_STATE)temp_8;                                                
                                            mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                    USB_HID_USAGE_ID_BUTTON3;
                                            loop++;
                                        }
                                        if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON4) && 
                                        (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                        {
                                            mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                            temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                        & 0x01U);
                                            mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                (USB_HID_BUTTON_STATE)temp_8;                                                
                                            mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                    USB_HID_USAGE_ID_BUTTON4;
                                            loop++;
                                        }
                                        if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON5) && 
                                        (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                        {
                                            mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                            temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                        & 0x01U);
                                            mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                (USB_HID_BUTTON_STATE)temp_8;                                                
                                            mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                    USB_HID_USAGE_ID_BUTTON5;
                                            loop++;
                                        }
                                        /* Move to the next usage */
                                        usage++;
                                        /* Update the report offset */
                                        currentReportOffsetTemp = 
                                            currentReportOffsetTemp + 
                                            (mainItem.globalItem)->reportSize;
                                    } /* end of while(all usages) */
                                } /* Usage Min Max present */
                                else
                                {
                                    loop = 1;
                                    do
                                    {
                                        if(mouseData[mouseIndex].taskPingPong)
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPing;
                                        }
                                        else
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPong;
                                        }
                                        result = USB_HOST_HID_UsageGet
                                                (
                                                    handle,
                                                    index,
                                                    loop,
                                                    &usage
                                                );
                                        if(result == USB_HOST_HID_RESULT_SUCCESS)
                                        {
                                            if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON1) && 
                                            (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                            {
                                                mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                                temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                            & 0x01U);
                                                mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                    (USB_HID_BUTTON_STATE)temp_8;                                                    
                                                mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                        USB_HID_USAGE_ID_BUTTON1;
                                                loop++;
                                            }
                                            if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON2) && 
                                            (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                            {
                                                mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                                temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                            & 0x01U);
                                                mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                    (USB_HID_BUTTON_STATE)temp_8;                                                   
                                                mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                        USB_HID_USAGE_ID_BUTTON2;
                                                loop++;
                                            }
                                            if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON3) && 
                                            (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                            {
                                                mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                                temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                            & 0x01U);
                                                mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                    (USB_HID_BUTTON_STATE)temp_8;                                                  
                                                mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                        USB_HID_USAGE_ID_BUTTON3;
                                                loop++;
                                            }
                                            if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON4) && 
                                            (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                            {
                                                mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                                temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                            & 0x01U);
                                                mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                    (USB_HID_BUTTON_STATE)temp_8;                                                    
                                                mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                        USB_HID_USAGE_ID_BUTTON4;
                                                loop++;
                                            }
                                            if((0x00FFU & usage) == ((uint32_t)USB_HID_USAGE_ID_BUTTON5) && 
                                            (loop < (uint32_t)USB_HOST_HID_MOUSE_BUTTONS_NUMBER))
                                            {
                                                mouseDataBuffer = mouseDataBuffer + (currentReportOffsetTemp/8U);
                                                temp_8 = ((((*mouseDataBuffer)) >> currentReportOffsetTemp % 8U)
                                                            & 0x01U);
                                                mouseData[mouseIndex].usageDriverData.buttonState[loop] = 
                                                    (USB_HID_BUTTON_STATE)temp_8;                                                   
                                                mouseData[mouseIndex].usageDriverData.buttonID[loop] =
                                                        USB_HID_USAGE_ID_BUTTON5;
                                                loop++;
                                            }
                                            currentReportOffsetTemp = 
                                                                currentReportOffsetTemp + 
                                                                (mainItem.globalItem)->reportSize;
                                            loop++;
                                        } /* if usage found */
                                    } while(result == USB_HOST_HID_RESULT_SUCCESS);
                                    /* We have checked all the usages present
                                         in this field. Now reset the result.
                                         Otherwise the main while loop will
                                         exit. */
                                    result = USB_HOST_HID_RESULT_SUCCESS;
                                } /* Individual usage based field */
                            } /* Button page */

                            else if(( ((0xFF00U & ((uint32_t)(mainItem.globalItem)->usagePage)) >> 8 )
                                        == (uint32_t)USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS) || 
                                        ((0x00FFU & ((uint32_t)(mainItem.globalItem)->usagePage))
                                        == (uint32_t)USB_HID_USAGE_PAGE_GENERIC_DESKTOP_CONTROLS))
                            {
                                if(((mainItem.localItem)->usageMinMax.valid) == false)
                                {
                                    usage = 0;
                                    loop = 1;
                                    do
                                    {
                                        if(mouseData[mouseIndex].taskPingPong)
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPing;
                                        }
                                        else
                                        {
                                            mouseDataBuffer = mouseData[mouseIndex].dataPong;
                                        }
                                        result = USB_HOST_HID_UsageGet
                                                (
                                                    handle,
                                                    index,
                                                    loop,
                                                    &usage
                                                );
                                        if(result == USB_HOST_HID_RESULT_SUCCESS)
                                        {
                                            /* Usage obtained */
                                            usage = usage << 16;
                                            usage = usage >> 16;
                                            if((usage == USAGE_X) || 
                                                (usage == USAGE_Y) ||
                                                    (usage == USAGE_Z))
                                            {
                                                /* X/Y/Z found*/
                                                mouseDataBuffer = mouseDataBuffer + 
                                                        (currentReportOffsetTemp/8U);
                                                
                                                ptr = mouseDataBuffer;
                                                mouseDataBufferTemp = 0;
                                                for (i = 0; i < 5U; i++)
                                                {
                                                    mouseDataBufferTemp = mouseDataBufferTemp | (((uint64_t)ptr[i]) << (i * 8U));
                                                }
                                                
                                                mouseDataBufferTemp = (mouseDataBufferTemp) >> (currentReportOffsetTemp % 8U);
                                                
                                                mouseDataBufferTemp = (mouseDataBufferTemp) & 0xFFFFFFFFU;
                                                
                                                /* The logic here is AND with (2 to the power report size) - 1 */
                                                andMask = (1UL << (mainItem.globalItem)->reportSize) - 1U;
                                                mouseDataBufferTemp = (mouseDataBufferTemp & (uint64_t)andMask);

                                                
                                                if((mainItem.globalItem)->logicalMinimum < 0)
                                                {
                                                    int64_t readData = (int64_t)-1;
                                                    uint64_t readMousedata = (((mouseDataBufferTemp) & ((uint64_t)1U << (((mainItem.globalItem)->reportSize) - 1U))) != 0U) ? 
                                                        (mouseDataBufferTemp) | (((uint64_t)readData) << ((mainItem.globalItem)->reportSize))
                                                          : (mouseDataBufferTemp);
                                                    mouseDataBufferTemp = readMousedata;
                                                        
                                                }
                                                if(usage == USAGE_X)
                                                {
                                                    mouseData[mouseIndex].usageDriverData.xMovement =
                                                        (int16_t)(mouseDataBufferTemp);
                                                }
                                                else if(usage == USAGE_Y)
                                                {
                                                    mouseData[mouseIndex].usageDriverData.yMovement =
                                                        (int16_t)(mouseDataBufferTemp);
                                                }
                                                else if(usage == USAGE_Z)
                                                {
                                                    mouseData[mouseIndex].usageDriverData.zMovement =
                                                        (int16_t)(mouseDataBufferTemp);
                                                }
                                                else
                                                {
                                                    /* Do Nothing */
                                                }
                                            }
                                            currentReportOffsetTemp = 
                                                        currentReportOffsetTemp + 
                                                        (mainItem.globalItem)->reportSize;
                                            loop++;
                                        }/* if usage obtained */
                                    } while(result == USB_HOST_HID_RESULT_SUCCESS);
                                    /* We have checked all the usages present
                                         in this field. Now reset the result.
                                         Otherwise the main while loop will
                                         exit. */
                                    result = USB_HOST_HID_RESULT_SUCCESS;
                                }/* if usage min max used */
                            }/* if Generic Desktop Page */
                            else
                            {
                                /* Do Nothing */
                            }
                        }/* Main item = INPUT */
                        else if((uint32_t)mainItem.tag ==
                                    USB_HID_MAIN_ITEM_TAG_OUTPUT)
                        {
                            /* Change report offset as they
                             * create data fields*/
                            reportOffset = reportOffset +
                                        (mainItem.globalItem)->reportCount *
                                        (mainItem.globalItem)->reportSize;
                        }
                        else if((uint32_t)mainItem.tag ==
                                    USB_HID_MAIN_ITEM_TAG_FEATURE)
                        {
                            /* Change report offset as they
                             * create data fields*/
                            reportOffset = reportOffset +
                                    (mainItem.globalItem)->reportCount *
                                    (mainItem.globalItem)->reportSize;
                        }
                        else
                        {
                            /* Do Nothing */
                        }
                    }/* Field found */
                    index++;
                } while(result == USB_HOST_HID_RESULT_SUCCESS);
                
                if(appMouseHandler != NULL)
                {
                    appMouseHandler((USB_HOST_HID_MOUSE_HANDLE)handle,
                                USB_HOST_HID_MOUSE_EVENT_REPORT_RECEIVED,
                                (void *)&mouseData[mouseIndex].usageDriverData);
                }
                
                if(mouseData[mouseIndex].taskPingPong)
                {
                    mouseData[mouseIndex].isPingReportProcessing = false;
                }
                else
                {
                    mouseData[mouseIndex].isPongReportProcessing = false;
                }
            }/* end of report processing */
            break;
        default:
            /* Do Nothing */
            break;
    }
}/* End of USB_HOST_HID_MOUSE_Task() */

<#if core.COVERITY_SUPPRESS_DEVIATION?? && core.COVERITY_SUPPRESS_DEVIATION>
#pragma coverity compliance end_block "MISRA C-2012 Rule 21.15"
<#if core.COMPILER_CHOICE == "XC32">
#pragma GCC diagnostic pop
</#if>
</#if>
/* MISRAC 2012 deviation block end */