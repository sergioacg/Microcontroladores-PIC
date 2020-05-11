#INCLUDE <18f4550.h>
#USE DELAY(CLOCK=4000000,crystal)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8)
#define TC_CLK     PIN_B1 
#define TC_CS      PIN_C0
#define TC_DATA    PIN_B0


#include "max6675.c"

void main() 
{ 
   
   char msg[32]; 
   delay_ms(50);      //allow oscillator to stabilise 

   while(1){ 
      delay_ms(1000); 
      //sprintf(msg,"%3.2f%cC\r\n",do_everything(),0xB0); 
      sprintf(msg,"I%3.2fFI%3.2fFI%3.2fF",do_everything(TC_CS),do_everything(TC_CS),do_everything(TC_CS)); 
      printf("%s",msg); 
   } 
} 
//!

