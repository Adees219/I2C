#include "config.h"

void configPort(void){ //configuracion de puertos
    
    ANSEL = 0b00000001; //RA0 analog, other digit
    ANSELH = 0;         //PORTB digit
    
    TRISA = 0b00000000;
    TRISB = 0b11111111;
    TRISC = 0b11111111;
    TRISD = 0b00000000;
    TRISE = 0b0000;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTE = 0;

    return;

}

/*******************************************************************************************************************/
void pullup(void){
    OPTION_REGbits.nRBPU = 0;
    WPUBbits.WPUB = 0b11111111;
    
    return;
}

/*******************************************************************************************************************/

void configIntbits(void){ //configuracion de bits de interrupcion    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
 
    return;
}

/*******************************************************************************************************************/

void configOsc(uint16_t freq){ //configuracion del oscilador (frequencia)
    if(freq == 8){
        OSCCONbits.IRCF = 0b111;
    }
    
    else if(freq == 4){
        OSCCONbits.IRCF = 0b110;
    }
    
    else if(freq == 2){
        OSCCONbits.IRCF = 0b101;
    }
    
    else if(freq == 1){
        OSCCONbits.IRCF = 0b100;
    }
    
    else if(freq == 500){
        OSCCONbits.IRCF = 0b011;
    }
    
    else if(freq == 250){
        OSCCONbits.IRCF = 0b010;
    }
        
    else if(freq == 125){
        OSCCONbits.IRCF = 0b001;
    }
     
    else if(freq == 31){
        OSCCONbits.IRCF = 0b000;
    }
    
    OSCCONbits.SCS = 1; //habilitando el oscilador del sistema como interno
    
    return;
}

/*******************************************************************************************************************/

void ioc_init(char pin){ //Interrupt on change (cadena de bits)   
    
    IOCBbits.IOCB = pin; 
    
    return;
}

/*******************************************************************************************************************/

void config_timer0(void){
       
    OPTION_REGbits.T0CS = 0;      //temporizador
    
    OPTION_REGbits.T0SE = 0;      //cambio de señal positivo
    OPTION_REGbits.PSA = 0;       //prescaler a temporizador
    
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;      //prescaler 1:256
    return;
}