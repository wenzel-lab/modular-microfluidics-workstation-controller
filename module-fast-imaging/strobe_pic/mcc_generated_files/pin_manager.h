/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.77
        Device            :  PIC16F18857
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.05 and above
        MPLAB 	          :  MPLAB X 5.20	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set SDO1 aliases
#define SDO1_TRIS                 TRISBbits.TRISB0
#define SDO1_LAT                  LATBbits.LATB0
#define SDO1_PORT                 PORTBbits.RB0
#define SDO1_WPU                  WPUBbits.WPUB0
#define SDO1_OD                   ODCONBbits.ODCB0
#define SDO1_ANS                  ANSELBbits.ANSB0
#define SDO1_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define SDO1_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define SDO1_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define SDO1_GetValue()           PORTBbits.RB0
#define SDO1_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define SDO1_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define SDO1_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define SDO1_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define SDO1_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define SDO1_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define SDO1_SetAnalogMode()      do { ANSELBbits.ANSB0 = 1; } while(0)
#define SDO1_SetDigitalMode()     do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set SDI1 aliases
#define SDI1_TRIS                 TRISBbits.TRISB2
#define SDI1_LAT                  LATBbits.LATB2
#define SDI1_PORT                 PORTBbits.RB2
#define SDI1_WPU                  WPUBbits.WPUB2
#define SDI1_OD                   ODCONBbits.ODCB2
#define SDI1_ANS                  ANSELBbits.ANSB2
#define SDI1_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define SDI1_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define SDI1_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define SDI1_GetValue()           PORTBbits.RB2
#define SDI1_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define SDI1_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define SDI1_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define SDI1_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define SDI1_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define SDI1_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define SDI1_SetAnalogMode()      do { ANSELBbits.ANSB2 = 1; } while(0)
#define SDI1_SetDigitalMode()     do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set SCK1 aliases
#define SCK1_TRIS                 TRISBbits.TRISB3
#define SCK1_LAT                  LATBbits.LATB3
#define SCK1_PORT                 PORTBbits.RB3
#define SCK1_WPU                  WPUBbits.WPUB3
#define SCK1_OD                   ODCONBbits.ODCB3
#define SCK1_ANS                  ANSELBbits.ANSB3
#define SCK1_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define SCK1_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define SCK1_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define SCK1_GetValue()           PORTBbits.RB3
#define SCK1_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define SCK1_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define SCK1_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define SCK1_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define SCK1_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define SCK1_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define SCK1_SetAnalogMode()      do { ANSELBbits.ANSB3 = 1; } while(0)
#define SCK1_SetDigitalMode()     do { ANSELBbits.ANSB3 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set SS1 aliases
#define SS1_TRIS                 TRISCbits.TRISC6
#define SS1_LAT                  LATCbits.LATC6
#define SS1_PORT                 PORTCbits.RC6
#define SS1_WPU                  WPUCbits.WPUC6
#define SS1_OD                   ODCONCbits.ODCC6
#define SS1_ANS                  ANSELCbits.ANSC6
#define SS1_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define SS1_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define SS1_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define SS1_GetValue()           PORTCbits.RC6
#define SS1_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define SS1_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define SS1_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define SS1_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define SS1_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define SS1_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define SS1_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define SS1_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()              PORTCbits.RC7
#define RC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetPullup()             do { WPUCbits.WPUC7 = 1; } while(0)
#define RC7_ResetPullup()           do { WPUCbits.WPUC7 = 0; } while(0)
#define RC7_SetAnalogMode()         do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()        do { ANSELCbits.ANSC7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/