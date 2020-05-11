// Por: Sergio Andres Castaño
// https://controlautomaticoeducacion.com/
//___________________________________________________________________________

#include <16f887.h>
#device ADC=10 
#fuses XT,NOWDT,NOPROTECT,NOLVP,PUT,BROWNOUT,INTRC_IO  
#use delay(clock=4000000)  

int16 duty=0;
int Timer2,Poscaler;

void main() { 
   // Generemos una Señal cuadrada de 1 Khz
   Timer2=249;    //Se carga timer 2 con 249 como lo vimos en la pagina
   //Preescaler=4;  //Preescaler solo puede tomar valores de: 1 - 4 - 16
   //Para el preescaler colocamos "t2_div_by_4"
   Poscaler=1;  //Preescaler solo puede tomar valores de: 1
   
   setup_timer_2(t2_div_by_4,Timer2,Poscaler);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   setup_adc_ports(all_analog);        //Configurar ADC
   setup_adc(adc_clock_internal);
   while(1)
   {
        set_adc_channel(0);   
        delay_us(100);       
        duty=read_adc(); 
        set_pwm1_duty(duty);
        //delay_ms(5000);
   }
}
