/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.145.0
        Device            :  dsPIC33CK256MP502
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36b
        MPLAB 	          :  MPLAB X v5.25
*/

/*
    (c) 2019 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include <stdio.h>
#include "pin_manager.h"

/**
 Section: File specific functions
*/
void (*ADC_RDY_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0020;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001C;
    TRISB = 0xF7C7;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x3CBC;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x001C;
    ANSELB = 0x0001;


    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    __builtin_write_RPCON(0x0000); // unlock PPS

    RPOR1bits.RP35R = 0x0008;    //RB3->SPI2:SDO2
    RPOR2bits.RP37R = 0x000A;    //RB5->SPI2:SS2OUT
    RPINR22bits.SCK2R = 0x0024;    //RB4->SPI2:SCK2OUT
    RPOR5bits.RP43R = 0x0005;    //RB11->SPI1:SDO1
    RPOR2bits.RP36R = 0x0009;    //RB4->SPI2:SCK2OUT
    RPINR20bits.SDI1R = 0x002C;    //RB12->SPI1:SDI1
    RPINR20bits.SCK1R = 0x002D;    //RB13->SPI1:SCK1IN
    RPINR21bits.SS1R = 0x002A;    //RB10->SPI1:SS1

    __builtin_write_RPCON(0x0800); // lock PPS

    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    CNEN1Bbits.CNEN1B15 = 1;    //Pin : RB15
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFBbits.CNFB15 = 0;    //Pin : RB15
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONBbits.CNSTYLE = 1;    //Config for PORTB
    CNCONBbits.ON = 1;    //Config for PORTB

    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS0bits.CNBIF = 0; //Clear CNBI interrupt flag
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void ADC_RDY_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    ADC_RDY_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}


/* Interrupt service routine for the CNBI interrupt. */
void __attribute__ (( interrupt, no_auto_psv )) _CNBInterrupt ( void )
{
    if(IFS0bits.CNBIF == 1)
    {
        // Clear the flag
        IFS0bits.CNBIF = 0;
        if(CNFBbits.CNFB15 == 1)
        {
            CNFBbits.CNFB15 = 0;  //Clear flag for Pin - RB15
            if(ADC_RDY_InterruptHandler) 
            { 
                ADC_RDY_InterruptHandler(); 
            }
        }
    }
}
