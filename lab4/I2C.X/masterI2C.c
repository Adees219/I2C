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
#include "LCD.h"
//**********************************************************************************************
//prototipo de funciones
//**********************************************************************************************
void setup(void);
uint8_t bcd_to_decimal(uint8_t bdc);
void pantalla(void);
//**********************************************************************************************
//variables
//**********************************************************************************************

#define _XTAL_FREQ 8000000

int temp1;
char Resadc[4];
char hora[9];
char fecha[9];
char sec;
char min;
char hour;
char day;
char month;
char year;
//**********************************************************************************************
//ISR
//**********************************************************************************************
void __interrupt() isr(void)
{

}


//**********************************************************************************************
//Codigo principal
//**********************************************************************************************
void main(void) {
    setup();
        
    char yr=00;
    char mt=01;
    char hr=00;
    char d = 01;
    
    while(1){

        
        /*modificacion RTC*/
        if(RB0==0){ //hora
            hr++;
            if(hr>=24){
                hr=00;
            }
            
            I2C_Master_Start();
            uint8_t hora_decimal = bcd_to_decimal(hr); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(0X02);
            I2C_Master_Write(hora_decimal);
            I2C_Master_RepeatedStart();  // Reiniciar el I2C
            I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
            hour = I2C_Master_Read(0);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        
         if(RB1==0){ //mes
            d++;
            if(d>=31){
                d=01;
            }
            I2C_Master_Start();
            uint8_t dia_decimal = bcd_to_decimal(d); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(0X03);
            I2C_Master_Write(dia_decimal);
            I2C_Master_RepeatedStart();  // Reiniciar el I2C
            I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
            day = I2C_Master_Read(0);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        
        if(RB2==0){ //mes
            mt++;
            if(mt>=12){
                mt=01;
            }
            I2C_Master_Start();
            uint8_t mes_decimal = bcd_to_decimal(mt); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(0X05);
            I2C_Master_Write(mes_decimal);
            I2C_Master_RepeatedStart();  // Reiniciar el I2C
            I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
            month = I2C_Master_Read(0);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        if(RB3==0){ //año
            yr++;
            if(yr>=99){
               yr=00;
            }
            I2C_Master_Start();
            uint8_t an_decimal = bcd_to_decimal(yr); //Conversion de dias a decimal
            I2C_Master_Write(0xD0);
            I2C_Master_Write(0X06);
            I2C_Master_Write(an_decimal);
            I2C_Master_RepeatedStart();  // Reiniciar el I2C
            I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
            year = I2C_Master_Read(0);
            I2C_Master_Stop();
             __delay_ms(200);
        }
        

        
        
       /*~~~~~~~comunicacion con el esclavo~~~~~*/
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        temp1 = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200); 

       /*~~~~~~~~~~hora RTC~~~~~~~~~~~~*/
        //segundos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x00);      // Seleccionar registro de segundos
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        sec = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        //minutos
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x01);      // Seleccionar registro de minutos
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        min = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        //horas
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x02);      // Seleccionar registro de hora
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        hour = I2C_Master_Read(0);
        I2C_Master_Stop();
        
       /*~~~~~~fecha RTC~~~~~~~~~~*/
        
        //dia
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x03);      // Seleccionar registro de dias
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        day = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        //mes
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x05);      // Seleccionar registro de mes
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        month = I2C_Master_Read(0);
        I2C_Master_Stop();
        
        //año
        I2C_Master_Start();          // Iniciar el I2C
        I2C_Master_Write(0xD0);      // Escribir la direccion del RTC para escritura
        I2C_Master_Write(0x06);      // Seleccionar registro de año
        I2C_Master_RepeatedStart();  // Reiniciar el I2C
        I2C_Master_Write(0xD1);      // Escribir la direccion del RTC para lectura
        year = I2C_Master_Read(0);
        I2C_Master_Stop();
        

        
        
        // Convertir el valor BCD a decimal
        uint8_t seconds_decimal = bcd_to_decimal(sec);
        uint8_t minutes_decimal = bcd_to_decimal(min);
        uint8_t hours_decimal = bcd_to_decimal(hour);
        sprintf(hora,"%d:%d:%d",hours_decimal,minutes_decimal ,seconds_decimal);
        
        uint8_t days_decimal = bcd_to_decimal(day);
        uint8_t months_decimal = bcd_to_decimal(month);
        uint8_t years_decimal = bcd_to_decimal(year);
        sprintf(fecha,"%d/%d/%d",years_decimal,months_decimal,days_decimal);
        
        
        pantalla();//funcion para mostrar datos en la LCD
        
        __delay_ms(200);
  
    }
    return;
}


//**********************************************************************************************
//Codigo de configuracion
//**********************************************************************************************
void setup(){
    
    configOsc(8); //configuracion del oscilador
    configPort(); //configuracion de puertos   
    I2C_Master_Init(100000);
    Lcd_Init();
    configIntbits();
    pullup();
    return;
}

/*******************************************************************/
//Funciones locales
/*******************************************************************/
uint8_t bcd_to_decimal(uint8_t bcd){
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void pantalla(void){
    
    //dato adc slave
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("S1:");
    Lcd_Set_Cursor(2,1);
    sprintf(Resadc,"%d",temp1);
    Lcd_Write_String(Resadc);
    
    //dato hora rtc
    Lcd_Set_Cursor(1,6);
    Lcd_Write_String(hora);
    
    
   //dato fecha rtc
    Lcd_Set_Cursor(2,6);
    Lcd_Write_String(fecha);
    return;
    
}