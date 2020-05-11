// ***********************************************************************//
// ***********************************************************************//
// *****       By: Sergio Andres Castaño Giraldo                    ******//
// *****       https://controlautomaticoeducacion.com/              ******//
// *****       Control Engineering                                  ******//
// ***********************************************************************//
// ***********************************************************************//


#INCLUDE <18F4550.h>
#DEVICE ADC=10
#USE DELAY(crystal=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP


#include <lcd420.c>
//Configura direccion de memoria de los puertos A,B,C,D
#BYTE PORTA=0xF80
#BYTE PORTB=0xF81
#BYTE PORTC=0xF82
#BYTE PORTD= 0xF83
int veces=39,seg=0;
int16 adc,control=0;
float yM=0,R=1500.0,e=0.0,e_1=0.0,e_2=0.0,u=0.0,u_1=0.0;
float kp,ti,td,q0,q1,q2,T=0.1;
float k=0.535,tao=10.0,theta=1.0;
float TsMA,Wn,P1,P2;

//Con esta rutina de desbordamiento del timer 0
//Hago el conteo del tiempo de muestreo
#INT_RTCC //Rutina de interrupción por RTCC
RELOJ()
{

   VECES--;          //Decremento variable veces
   SET_RTCC(246);    //Cargo el timer con 243
   IF(VECES==0)      //La variable veces se carga con 30 y el timer con 243
   {                 //para que entre a la interrupcion cada 100 milisegundos
      VECES=39;
      seg++;         //y incremente el contador 'seg'
   }
   if(seg==1)
   {
    seg=0;
    output_toggle(pin_d0);
    //Comienzo la ley de control
    
    e=R-yM;
    // Controle PID
      u = u_1 + q0*e + q1*e_1 + q2*e_2; //Ley del controlador PID discreto
    
    if (u >= 5000.0)        //Saturo la accion de control 'uT' en un tope maximo y minimo
     u = 5000.0;
    
    if (u <= 0.0)
     u = 0.0;
     
     //escalizo la u de mV a bits, en el caso del PWM ser a 1Khz con una resolucion de 10
     //el valor máximo del pwm es de 1000
     control=u*1000/5000;
     
     //Retorno a los valores reales
     e_2=e_1;
     e_1=e;
     u_1=u;
     
     //La accion calculada la transformo en PWM
     set_pwm1_duty(control);
     
   }
}
void main()
{
   
   set_tris_c(0);
   set_tris_d(0);
   setup_timer_2(t2_div_by_4,249,1);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   set_pwm1_duty(0);                   //Dejo en cero la salida PWM
   
   setup_adc_ports(AN0);              //Configurar ADC (Lectura de temperatura)
   setup_adc(adc_clock_internal);      //Reloj interno para la conversion analoga digital)
   set_adc_channel(0);                 //Seleccionar Canal 0 para sensor de Temperatura
   
   //SET_RTCC(246);                               //Cargo el TIMER0 con 243
   //SETUP_COUNTERS(RTCC_INTERNAL, RTCC_DIV_256); //Configuro el TIMER0 con  prescaler 256
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_256|RTCC_8_bit);      //65.5 ms overflow
   
   ENABLE_INTERRUPTS(INT_RTCC);  //Habilito interrupcion por TIMER0
   enable_interrupts(GLOBAL);    //Habilito las interrupciones a nivel global
   
   
   LCD_INIT();                         //Inicializo el LCD
   LCD_PUTC("\f");                     //Limpio el LCD
   
   //*************************************************************************//
   //*************  DISEÑO POR ASIGNACIÓN DE 2 POLOS REALES   ****************//
   //*************************************************************************//
   
   TsMA=30;                    //Tiempo deseado en Lazo Cerrado    
   Wn=5.8335/(TsMA);               //Frecuencia natural del sistema
   
   //Ubicación de 2 Polos reales
   P1=Wn+Wn;
   P2=Wn*Wn;
   
   kp=(P1*tao-1)/k;        //Calculo de Kc
   ti=(k*kp)/(P2*tao);     //Calculo de ti
   
   //*************************************************************************//
   //*****************   DISEÑO POR CANCELACIÓN DE POLOS    *******************//
   //*************************************************************************//
   /*
   TsMA=7.5;                  //Tiempo deseado en Lazo Cerrado 
   kp=(tao)/(TsMA*k);         //Calculo de Kc
   ti=tao;                    //Calculo de Ti (Igual a la constante de tiempo)
   td=0;
   */
   //*************************************************************************//
   //*****************   SINTONIA POR ZIEGLER y NICHOLS    *******************//
   //*************************************************************************//
   /*
   kp=(1.2*tao)/(k*theta);
   ti=2*theta;
   td=0.5*theta;*/
   
   //*************************************************************************//
   
  // Calculo do controle PID digital
   q0=kp*(1+T/(2*ti)+td/T);
   q1=-kp*(1-T/(2*ti)+(2*td)/T);
   q2=(kp*td)/T;
      
   while(1)
   {

      adc=read_adc(); //Leer ADC
      yM=adc*5000.0/1024.0;

     
      //Muestra mensajes en LCD
      lcd_gotoxy(1,1);
      printf(lcd_putc,"Temp: %f",yM/10);
      lcd_gotoxy(1,2);
      printf(lcd_putc,"SetPoint: %2.2f C   ",R/10);
      lcd_gotoxy(1,3);
      printf(lcd_putc,"Error: %2.2f C   ",e/10);
      lcd_gotoxy(1,4);
      printf(lcd_putc,"u(ley Con): %ld   ",control);
      

      }
}
