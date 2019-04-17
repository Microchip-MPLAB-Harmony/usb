/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************

#define LEDG_ON()                    LED_GREEN_On()
#define LEDG_OFF()                   LED_GREEN_Off()
#define LEDG_TOGGLE()                LED_GREEN_Toggle()
  
#define LEDB_ON()                    LED_BLUE_On()
#define LEDB_OFF()                   LED_BLUE_Off()
#define LEDB_TOGGLE()                LED_BLUE_Toggle()
  
#define LEDR_ON()                    LED_RED_On()
#define LEDR_OFF()                   LED_RED_Off()
#define LEDR_TOGGLE()                LED_RED_Toggle()

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
