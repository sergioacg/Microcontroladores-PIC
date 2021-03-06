#INCLUDE <16F887.h>
#DEVICE ADC=10
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#DEFINE USE_PORTB_KBD   //Por defecto el teclado se conecta al puerto D,
                        //como el microcontrolador que se esta usando
                        //no tiene puerto D se conecta al puerto B.*/
#INCLUDE "LCD420D.C"
#INCLUDE <KBD4x4.C>  //Incluir en el encabezado el driver para
                      //manejar el teclado telef�nico MODIFICADO
#include <stdlib.h>
#include <string.h>

#use     standard_io(b) 
#define  KEYHIT_DELAY   1    //Tiempo de espera del teclado en milisegundos
#byte PORTB= 6
#byte PORTC= 7
#BYTE PORTA= 5
#BYTE PORTD= 8

int16 adc,control=0;
float R=1500;//Referencia de 150 �C por defecto
float yM=0,e=0.0,e_1=0.0,e_2=0.0,u=0.0,u_1=0.0,T=0.1;
float kp,ti,td,q0,q1,q2;
float k=0.535,tao=10,theta=1;
float TsMA,Wn,P1,P2;
char c;
int cont=0;


/*===========================================================================*/
/*=======================       FUNCION TECLA         =======================*/
/*===========================================================================*/
//Funcion encargada de esperar a que se presione una tecla 
char tecla(void)
{
   char c;
   do{ //espera hasta que se presione una tecla
      c=kbd_getc(); //Captura valor del teclado
     }
   while(c=='\0'); 
   return(c);
}

/*===========================================================================*/
/*=======================    FUNCION TECLA CON TIMER  =======================*/
/*===========================================================================*/
// Pregunta por una Tecla por un tiempo, si no hay actividad, deja de preguntar
// y deja que el PIC continue con su trabajo

char tecla_time(void) {
   char c='\0';
   unsigned int16 timeout;
   timeout=0;
   c=kbd_getc(); //Captura valor del teclado
   while(c=='\0' && (++timeout< (KEYHIT_DELAY*100)))
   {
      delay_us(10);
      c=kbd_getc(); //Captura valor del teclado
   }
   return(c);
}


/*===========================================================================*/
/*=======================    FUNCION DEL CONTROL PID  =======================*/
/*===========================================================================*/
void PID(void)
{
    
    e=1*(R-yM);
    // Controle PID
      u = u_1 + q0*e + q1*e_1 + q2*e_2; //Ley del controlador PID discreto
    
    if (u >= 5000.0)        //Saturo la accion de control 'uT' en un tope maximo y minimo
     u = 5000.0;
    
    if (u <= 0.0)
     u = 0.0;
     
     control=u/5;
     
     //Retorno a los valores reales
     e_2=e_1;
     e_1=e;
     u_1=u;
     
     //La accion calculada la transformo en PWM
     set_pwm1_duty(control);
     
}


/*===========================================================================*/
/*============   FUNCION PARA DIGITAR ESCAL�N/SETPOINT  =====================*/
/*===========================================================================*/
long escalon(int nd)
{
   //Esta funcion captura el escalon desde el teclado, si el proceso est� tomando
   //datos el escalon sirve para exitar el sistema, por otro lado si el sistema
   //est� controlando, el escalo sirve para establecer el setpoint del sistema
   
   long val;
   int i;
   char str[5]; //Variable tipo String
   
   str[0]='0';
   for(i=0;i<nd;i++)
   {
      
      c=tecla();  //Lee el valor del teclado y espera hasta que alguna tecla se pulse
      if(c!='*'){
         //Muestra el digito presionado en el LCD
         lcd_gotoxy(5+i,4);
         lcd_putc(c);
         //Almacena el dato presionado en la variable String
         str[i]=c;
      }
      else{i=nd;} //Si se presiona * sale del For      
   }
   val = atol(str); //Convierte el String en un valor numerico
   return(val);
}

void main()
{
   port_b_pullups (0xFF);  //Utiliza las resistencias PULL UP internas del puerto B
   
   set_tris_c(0);
   set_tris_d(0);
   setup_timer_2(t2_div_by_4,249,1);   //Configuracion de Timer 2 para establecer frec. PWM a 1kHz
   setup_ccp1(ccp_pwm);                //Configurar modulo CCP1 en modo PWM
   set_pwm1_duty(0);                   //Dejo en cero la salida PWM
   
   setup_adc_ports(sAN0);              //Configurar ADC (Lectura de temperatura)
   setup_adc(adc_clock_internal);      //Reloj interno para la conversion analoga digital)
   set_adc_channel(0);                 //Seleccionar Canal 0 para sensor de Temperatura
   
   
   LCD_INIT();                         //Inicializo el LCD
   LCD_PUTC("\f");                     //Limpio el LCD
   
   //*************************************************************************//
   //*************  DISE�O POR ASIGNACI�N DE 2 POLOS REALES   ****************//
   //*************************************************************************//
   
   TsMA=30;                    //Tiempo deseado en Lazo Cerrado    
   Wn=5.8335/(TsMA);               //Frecuencia natural del sistema
   
   //Ubicaci�n de 2 Polos reales
   P1=2*Wn;
   P2=Wn*Wn;
   
   kp=(P1*tao-1)/k;        //Calculo de Kc
   ti=(k*kp)/(P2*tao);     //Calculo de ti
   
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
      
   while(1)
   {      
         adc=read_adc(); //Leer ADC
         yM=adc*5000.0/1024.0;
         
         //Llama la funcion del controlador PID
         PID();
         //tiempo de muestreo
         delay_ms(28);
         
            lcd_gotoxy(2,1);
            lcd_putc("CONTROL PID HORNO");
            lcd_gotoxy(1,2);
            printf(lcd_putc,"Set-Point  : %3.1f C   ",R/10);
            lcd_gotoxy(1,3);
            printf(lcd_putc,"Temperatura: %3.1f C",yM/10);            
            lcd_gotoxy(1,4);
            printf(lcd_putc,"Ley Control: %ld   ",control);
 
         ; 
         
      
         c=tecla_time();   //Lee el valor del teclado pero solo espera un tiempo determinado
         if(c=='D')
         {
            lcd_gotoxy(1,2);
            LCD_PUTC("Referencia(0-300):");
            lcd_gotoxy(1,3);
            LCD_PUTC("y presione *");
            lcd_gotoxy(1,4);
            LCD_PUTC("SP:                     ");
            R=escalon(3); //Llama la funcion para digitar el escalon de exitacion
            //Valida si R esta entre 0 y 100 (Esto es otra forma de usar el if - else)
            R =(R > 300) ? 3000:R*10;
            
            //Muestra el SETPOINT en pantalla
            lcd_gotoxy(1,4);
            printf(lcd_putc,"SP: %3.1f         ",R/10);
            delay_ms(2000);
            LCD_PUTC("\f");
         }
   }
}
