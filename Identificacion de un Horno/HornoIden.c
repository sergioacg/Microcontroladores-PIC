#INCLUDE <16f887.h>
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP,NOMCRL
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8)
#define TC_CLK     PIN_C3  
#define TC_CS      PIN_C0
#define TC_DATA    PIN_C4


#include "max6675.c"
#include <stdlib.h>
#include <string.h>

long deg=0;

short timeout_error;
char car[10];

char timed_getc() { 
   long timeout;
   
   timeout_error=FALSE; 
   timeout=0; 
   
   while(!kbhit()&&(++timeout<50000)) // 1/2 second 
      delay_ms(100); 

   if(kbhit()) 
      return(getc()); 
   else { 
      timeout_error=TRUE; 
      return(0); 
   } 
}

//Define la interrupción por recepción Serial
#int_RDA
void RDA_isr()
{
 int i=0,ini=0,fin=0;
 char dat[5];
 char degC[5];
 long duty;
 
 // Almacena 10 datos leidos del serial
  for(i=0;i<5;i++){
     dat[i]=getc();
     putc(dat[i]);
    }



//Busco el valor del escalon en los datos recibidos
for(i=0;i<5;i++){
  if(dat[i]=='S'){
    ini=i+1;
    i=10;
  }
 }
 for(i=ini;i<5;i++){
  if(dat[i]=='$'){
    fin=i-1;
    i=10;
  }
 }
 if(ini!=0 && fin!=0){
    // salvo en degC el caracter con el escalon
    for(i=ini;i<=fin;i++){
     degC[i-ini]=dat[i];
    }
    
     deg = atol(degC); //Convierte el String en un valor numerico
     
     // valida que el escalón esté ente 0 y 300 grados celcius
     
     if(deg>300)
         deg=300;
     if(deg<0)
         deg=0;
    
        duty=deg*10/3;
    set_pwm1_duty(duty);      // set PWM del PIC
 }
}
//! 

void main() 
{ 
   
   char msg[32]; 
   delay_ms(50);      //allow oscillator to stabilise 
   setup_timer_2(t2_div_by_4,249,1);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   set_pwm1_duty(0);                   //Dejo en cero la salida PWM
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);

   while(1){ 
      delay_ms(1000); 
      //sprintf(msg,"%3.2f%cC\r\n",do_everything(),0xB0); 
      sprintf(msg,"I%3.2fFI%3.2fFI%3.2fF",do_everything(),do_everything(),do_everything()); 
      printf("%s",msg); 
   } 
} 
