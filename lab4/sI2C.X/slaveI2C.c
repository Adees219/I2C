/* 
 * File:   masterI2C.c
 * Author: USUARIO FINAL
 *
 * Created on 3 de agosto de 2023, 12:00 PM
 */

//**********************************************************************************************
//palabra de configuracion
//**********************************************************************************************

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

//**********************************************************************************************
//prototipos de funciones
//**********************************************************************************************
#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "config.h"
#include "I2C.h"
#include "ADC.h"

//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);

//**********************************************************************************************
//variables
//**********************************************************************************************

#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{
    if(ADIF){
        PORTD = adc_read();
        ADIF = 0;
    }
    
    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            PORTD = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTD;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();

    while(1){
        if(ADCON0bits.GO == 0){
                __delay_ms(10);
                ADCON0bits.GO = 1; 
            }
       __delay_ms(500); 
      
    }
    return;
}


//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos   
    I2C_Slave_Init(0x50);  
    adc_init(0,8);
    return;
}

/*******************************************************************/
//Funciones locales
/*******************************************************************/
