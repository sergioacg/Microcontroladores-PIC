#INCLUDE <16F887.H>
#device adc=10
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP
//#DEFINE USE_PORTB_LCD TRUE
#DEFINE USE_PORTB_KBD   //Por defecto el teclado se conecta al puerto D,
                        //como el microcontrolador que se esta usando
                        //no tiene puerto D se conecta al puerto B.*/
#INCLUDE <LCD.C>
#INCLUDE <KBD4x4.C>  //Incluir en el encabezado el driver para
                      //manejar el teclado telefónico MODIFICADO
#use     standard_io(b) 
#define  KEYHIT_DELAY   200    //Tiempo de espera del teclado en milisegundos
#byte PORTB= 6
#byte PORTC= 7
#BYTE PORTA= 5
#BYTE PORTD= 8
 
long bits;     //Variable almacena los bits
float tem;     //Almacena la temperatura
  
  INT DIR;

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
/*=======================       PROGRAMA PRINCIPAL    =======================*/
/*===========================================================================*/

VOID MAIN()
{
  CHAR K;
   
   port_b_pullups (0xFF);  //Utiliza las resistencias PULL UP internas del puerto B
   
   set_tris_a(0b00000001);          //Pongo el RA0 como entrada
   SET_TRIS_B(0);
   SET_TRIS_C(0);    //Puerto C como Salida
   
   setup_adc_ports(sAN0);     //Pongo RA0 como analogo
   setup_adc(adc_clock_internal);   //Selecciono reloj interno para conversion
   
   LCD_INIT();       //Inicializar el driver del lcd
   KBD_INIT();       //Inicializar el driver del teclado
   LCD_PUTC("\f");   //Limpia el LCD
   
   
   WHILE(1)
   {
      DIR=9;
      LCD_GOTOXY(1,2);  //Ubica el cursor del LCD
      LCD_PUTC("Teclas= ");
      LCD_GOTOXY(1,2);  //Ubica el cursor del LCD
      while (DIR<17)
      {
                     
         if(k!='\0'){
            LCD_GOTOXY(DIR,2);
            lcd_putc(k);
            k='\0';
            DIR++;
         }
 // ============================================================================= //        
         //A continuación se muestran diferentes formas para leer los valores
         //del teclado matricial, descomenta la forma de almacenar el dato
         //en la variable "k" y observa las diferencias de cada llamado
         
         
         
         //k=tecla();      //Lee el valor del teclado y espera hasta que alguna tecla se pulse
         k=tecla_time();   //Lee el valor del teclado pero solo espera un tiempo determinado
         //k=kbd_getc();   //Función del ccs c para leer el valor del teclado (sin control)
         
  // ============================================================================= //   
         if(DIR>16)
            LCD_PUTC("\f");
         
       //Lectura ADC
        set_adc_channel(0);          //Selecciono el canal 0 (RA0)
        delay_ms(1);                 //llamo retardo de 1 ms
        bits=read_adc();             //Guarde el dato del LM en tempe
       
        tem=bits*0.4882;              //Conversion de bits a temperatura
        lcd_gotoxy(1,1);             //Ubiquese en la posicion 2,2
        printf(lcd_putc,"Temp= %f    ",tem);  //Muestra el valor numerico de la conversionconversion

      }
   }
}
