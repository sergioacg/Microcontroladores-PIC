#include <18F4550.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES HS

#use delay(crystal=20000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

#define TC_CLK     PIN_B1  
#define TC_CS      PIN_C0 
#define TC_DATA    PIN_B0


#include "max6675.c"
#include <stdlib.h>
#include <string.h>

long deg=0;
int muest=0;

int16 control=0;
float R=0;//Referencia de 150 �C por defecto
float yM=0,e=0.0,e_1=0.0,e_2=0.0,u=0.0,u_1=0.0,T=10;
float kp,ti,td,q0,q1,q2;
float k=3.0419 ,tao=201.45 ,theta=50;
float TsMA,Wn,P1,P2;


//Define la interrupci�n por recepci�n Serial
#int_RDA
void RDA_isr()
{
 int i=0,ini=0,fin=0;
 char dat[5];
 char degC[5];
 
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
     
     // valida que el escal�n est� ente 0 y 300 grados celcius
     
     if(deg>300)
         deg=300;
     if(deg<0)
         deg=0;
    
       R=deg;
 }
}
//! 

/*===========================================================================*/
/*=======================    FUNCION DEL CONTROL PID  =======================*/
/*===========================================================================*/
void PID(void)
{
    char msg[32];
    e=1*(R-yM);
    // Controle PID
      u = u_1 + q0*e + q1*e_1 + q2*e_2; //Ley del controlador PID discreto
      
//!     sprintf(msg,"e=%3.2f u=%3.2f",e,u); 
//!      printf("%s",msg);
      
      
    if (u >= 1000.0)        //Saturo la accion de control 'uT' en un tope maximo y minimo
     u = 1000.0;
    
    if (u <= 0.0)
     u = 0.0;
     
     control=u;
     
     //Actualizo las variables pasadas
     e_2=e_1;
     e_1=e;
     u_1=u;
     
     //La accion calculada la transformo en PWM
     set_pwm1_duty(control);
     output_toggle(PIN_B2);
//!     sprintf(msg,"e=%3.2f u=%3.2f C=%ld",e,u,control); 
//!      printf("%s",msg);
     // delay_ms(2000);
}


void main() 
{ 
   
   char msg[32]; 
   delay_ms(50);      //allow oscillator to stabilise 
   setup_timer_2(t2_div_by_4,249,1);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   set_pwm1_duty(0);                   //Dejo en cero la salida PWM
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   set_tris_b(0);
   
   
   //*************************************************************************//
   //*************  DISE�O POR ASIGNACI�N DE 2 POLOS REALES   ****************//
   //*************************************************************************//
   
//!   TsMA=900;                    //Tiempo deseado en Lazo Cerrado    
//!   Wn=3/(TsMA);               //Frecuencia natural del sistema
//!   
//!   //Ubicaci�n de 2 Polos reales
//!   P1=2*Wn;
//!   P2=Wn*Wn;
//!   
//!   kp=(P1*tao-1)/k;        //Calculo de Kc
//!   ti=(k*kp)/(P2*tao);     //Calculo de ti
   
   //*************************************************************************//
   //*****************   DISE�O POR CANCELACI�N DE POLOS    *******************//
   //*************************************************************************//
   /*
   TsMA=7.5;                  //Tiempo deseado en Lazo Cerrado 
   kp=(tao)/(TsMA*k);      //Calculo de Kc
   ti=tao;                  //Calculo de Ti (Igual a la constante de tiempo)
   td=0;
   */
   //*************************************************************************//
   //*****************   SINTONIA POR ZIEGLER y NICHOLS    *******************//
   //*************************************************************************//
   
   kp=(0.9*tao)/(k*theta); //Z-N Rapido
   //kp=kp/5;                //Z-N Lento
   ti=3.33*theta;
   td=0;
   
   //*************************************************************************//
   
  // Calculo do controle PID digital
   q0=kp*(1+T/(2*ti)+td/T);
   q1=-kp*(1-T/(2*ti)+(2*td)/T);
   q2=(kp*td)/T;
   

   while(1){ 
      yM=do_everything();
      //sprintf(msg,"%3.2f%cC\r\n",do_everything(),0xB0); 
      sprintf(msg,"I%3.2fFI%3.2fFC%ldRC%ldR",yM,yM,control/10,control/10); 
      printf("%s",msg); 
      //Si el muestreo cumple 11.5 seg Llama la funcion del controlador PID
      if(muest>7){
       muest=0;
       PID();
      }
      //tiempo de muestreo
      delay_ms(1000);
      muest++;
         
   } 
} 
