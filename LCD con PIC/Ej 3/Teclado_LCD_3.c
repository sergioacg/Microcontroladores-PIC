#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP

//Definiciones para colocar el LCD y el teclado en el puerto B
#DEFINE USE_PORTB_LCD TRUE
#DEFINE USE_PORTB_KBD TRUE  

//Librerías del LCD y Teclado
#INCLUDE <LCD.C>
#INCLUDE <KBD.C>  

// Variables Globales
CHAR K;
INT DIR;

//Programa Principal
VOID MAIN()
{
   lcd_init(); //Inicializa el LCD
   kbd_init(); //Inicializa el Teclado

   //Configura las Resistencias PULL-UP del Puerto B
   port_b_pullups(0xFF); //PIC16F887
   //port_b_pullups(TRUE); //PIC16F877A

   LCD_PUTC("\f"); //Limpia el LCD
   
   //Ciclo infinito
   WHILE(1)
   {
      DIR=0;
      LCD_PUTC("\f");
      while (DIR<17)
      {
         k=kbd_getc();  //Llamar la función del teclado y almacenar el
                        //valor digitado en una variable tipo carácter. Si
                        //no se oprime ninguna tecla el teclado
                        //retornara el carácter nulo.*/
         WHILE( (k=='\0')) //si no se oprime ninguna tecla sigue llamando al teclado.
         {
            k=kbd_getc();
         }
         if( (k!='*')&&(k!='#'))
         {
            lcd_putc(k);
            DIR++;
         }
      }
   }
}
