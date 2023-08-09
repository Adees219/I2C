#include "ADC.h"

void adc_init(int channel, int clock){ //inicializacion del ADC (canal del ADC, frecuencia reloj)
  
    //activacion
    ADCON0bits.ADON = 1;
    
    //delay
    __delay_ms(10);
    
    //channel
  if(channel == 0)
  ADCON0bits.CHS = 0b0000;    //AN0
  else if(channel == 1)
  ADCON0bits.CHS = 0b0001;    //AN1
  else if(channel == 2)
  ADCON0bits.CHS = 0b0010;    //AN2
  else if(channel == 3)
  ADCON0bits.CHS = 0b0011;    //AN3
  else if(channel == 4)
  ADCON0bits.CHS = 0b0100;    //AN4
  else if(channel == 5)
  ADCON0bits.CHS = 0b0101;    //AN5
  else if(channel == 6)
  ADCON0bits.CHS = 0b0110;    //AN6
 
  //voltaje de referencia
 ADCON1bits.VCFG1 = 0;       //VSS
 ADCON1bits.VCFG0 = 0;       //VDD
 
 //conversion clock
 if(clock == 1)
     ADCON0bits.ADCS = 0b00;    //FOSC/2
 else if(clock == 4)            
     ADCON0bits.ADCS = 0b01;    //FOSC/8
 else if(clock == 8 || clock == 20)
     ADCON0bits.ADCS = 0b10;    //FOSC/32

 //interrupciones
 PIE1bits.ADIE = 1;
 PIR1bits.ADIF = 0;
 
 //justificacion
 ADCON1bits.ADFM = 0; 
 
 return;
}

/*******************************************************************************************************************/

int adc_read(){ //resultado de la conversion analogica
    return ADRESH;
}

/*******************************************************************************************************************/

void adc_change_channel(int channel){ //cambiar canal del adc(numero de canal)
    
   if(channel < 14)
    ADCON0bits.CHS = channel;  
   else 
    ADCON0bits.CHS = 0b0000;  
   
    return;
}

/*******************************************************************************************************************/

int adc_get_channel(){ //obtener canal activo del adc
    char canal = ADCON0bits.CHS; 
    return canal;
}
/************************************************************************************/



int map (int val, int min_in, int max_in, int min_out, int max_out)
{
    
    int vout = (long)(val-min_in)*(max_out-min_out)/(max_in-min_in)+ min_out; 
    return vout;
}


