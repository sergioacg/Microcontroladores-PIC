#INCLUDE <16f887.h>
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP,NOMCLR
#use rs232(baud=9600, xmit=pin_c6, rcv=pin_c7, bits=8)
#define TC_CLK     PIN_C3  
#define TC_CS      PIN_C0
#define TC_DATA    PIN_C4


#include "max6675.c"
#include <stdlib.h>
#include <string.h>

long deg=0;

int16 adc,control=0;
float R=0000;//Referencia de 150 °C por defecto
float yM=0,e=0.0,e_1=0.0,e_2=0.0,u=0.0,u_1=0.0,T=1;
float kp,ti,td,q0,q1,q2;
float k=2.3553 ,tao=369.43 ,theta=30;
float TsMA,Wn,P1,P2;


//Define la interrupción por recepción Serial
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
     
     // valida que el escalón esté ente 0 y 300 grados celcius
     
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
    
    e=1*(R-yM);
    // Controle PID
      u = u_1 + q0*e + q1*e_1 + q2*e_2; //Ley del controlador PID discreto
    
    if (u >= 300.0)        //Saturo la accion de control 'uT' en un tope maximo y minimo
     u = 300.0;
    
    if (u <= 0.0)
     u = 0.0;
     
     control=u*10/3;
     
     //Retorno a los valores reales
     e_2=e_1;
     e_1=e;
     u_1=u;
     
     //La accion calculada la transformo en PWM
     set_pwm1_duty(control);
     
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
   
   //*************************************************************************//
   //*************  DISEÑO POR ASIGNACIÓN DE 2 POLOS REALES   ****************//
   //*************************************************************************//
   
   TsMA=1000;                    //Tiempo deseado en Lazo Cerrado    
   Wn=3/(TsMA);               //Frecuencia natural del sistema
   
   //Ubicación de 2 Polos reales
   P1=2*Wn;
   P2=Wn*Wn;
   
   kp=(P1*tao-1)/k;        //Calculo de Kc
   ti=(k*kp)/(P2*tao);     //Calculo de ti
   
   //*************************************************************************//
   //*****************   DISEÑO POR CANCELACIÓN DE POLOS    *******************//
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
   /*
   kp=(1.2*tao)/(k*theta);
   ti=2*theta;
   td=0.5*theta;
   */
   //*************************************************************************//
   
  // Calculo do controle PID digital
   q0=kp*(1+T/(2*ti)+td/T);
   q1=-kp*(1-T/(2*ti)+(2*td)/T);
   q2=(kp*td)/T;
   

   while(1){ 
      yM=do_everything();
      //sprintf(msg,"%3.2f%cC\r\n",do_everything(),0xB0); 
      sprintf(msg,"I%3.2fFI%3.2fFI%3.2fF",yM,yM,yM); 
      printf("%s",msg); 
      //Llama la funcion del controlador PID
      PID();
      //tiempo de muestreo
      delay_ms(700);
         
   } 
} 
