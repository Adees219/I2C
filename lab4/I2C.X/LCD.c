/* 
 * File: LCD.c  
 * Se utiliz� y se adaptaron las librer�as de Ligo George 
 * de la p�gina www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

//LCD Functions Developed by electroSome

#include "LCD.h"

void Lcd_Port(char a) {
    PORTD = a;
}

void Lcd_Cmd(char a) { //bien
    RS = 0; // => RS = 0 // Dato en el puerto lo va interpretar como comando
    Lcd_Port(a);
    EN = 1; // => E = 1
    __delay_ms(4);
    //EN = 0; // => E = 0
    EN = 0;
}

void Lcd_Clear(void) { //ya
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) { //ya
    char direccion;
    if (a == 1) {
        direccion = 0x80 + b - 1; //1xxx xxxx comando: direccion DDRAM; x= dirección
        Lcd_Cmd(direccion);
    } else if (a == 2) {
        direccion = 0xC0 + b - 1; 
        Lcd_Cmd(direccion);
    }
}

void Lcd_Init(void) { //ya
    Lcd_Port(0x00);
    __delay_ms(20);
    Lcd_Cmd(0x30);
    __delay_ms(5);
    Lcd_Cmd(0x30);
    __delay_ms(11);
    Lcd_Cmd(0x30);
    /////////////////////////////////////////////////////
    Lcd_Cmd(0x3C);
    Lcd_Cmd(0x0C);
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a) { //ya
    RS = 1; // => RS = 1
    Lcd_Port(a); //Data transfer
    EN = 1;
    __delay_us(40);
    EN = 0;

}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++)
        Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left(void) {
    Lcd_Cmd(0x01);
    Lcd_Cmd(0x08);
}


