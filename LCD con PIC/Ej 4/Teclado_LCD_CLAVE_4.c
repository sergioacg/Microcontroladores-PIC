#INCLUDE <16f887.h>
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP

//Usadas para conectar el LCD y Teclado en el Puerto B
#DEFINE USE_PORTB_LCD TRUE
#DEFINE USE_PORTB_KBD TRUE

//Librerías del LCD y Teclado
#INCLUDE<KBD.C> 
#INCLUDE<LCD.C>

#BYTE PORTC=7
#DEFINE LED1 PORTC,7
#DEFINE LED2 PORTC,4

 //Variables Globales
CHAR K;
INT MIL,CEN,DEC,UNI,VAL;

//Función Teclado
VOID TECLADO()
{
   k=kbd_getc(); //Llamar la función del teclado y almacenar
   //el valor digitado en una variable tipo
   //carácter. Si no se oprime ninguna tecla el
   //teclado retornara el carácter nulo.*/
   WHILE(k=='\0') //si no se oprime ninguna tecla sigue llamando al teclado.
   {
      k=kbd_getc();
   }
   IF( (k!='\0'))
   {
      IF(K=='0')//Si K es igual a cero
         VAL=0;//Val es igual a cero
      IF(K=='1')
         VAL=1;
      IF(K=='2')
         VAL=2;
      IF(K=='3')
         VAL=3;
      IF(K=='4')
         VAL=4;
      IF(K=='5')
         VAL=5;
      IF(K=='6')
         VAL=6;
      IF(K=='7')
         VAL=7;
      IF(K=='8')
         VAL=8;
      IF(K=='9')
         VAL=9;
   }
}
VOID MAIN()
{
   LCD_INIT(); //Inicializa el LCD 
   KBD_INIT(); //Inicializa el Teclado 
   
   //Configura las Resistencias PULL-UP del Puerto B 
   PORT_B_PULLUPS(0xFF); //PIC16F887 
   //port_b_pullups(TRUE); //PIC16F877A
   
   //Configura el puerto C como Salida
   SET_TRIS_C(0);
   
   //Programa Principal
   WHILE(TRUE)
   {
      LCD_GOTOXY(1,1);
      LCD_PUTC(" BIENVENIDOS ");
      LCD_GOTOXY(1,2);
      LCD_PUTC(" DIGITE CLAVE ");
      DELAY_MS(1000);
      TECLADO();
      LCD_PUTC("\f");
      LCD_GOTOXY(1,1);
      IF((k!='#')&&(k!='*'))
      {
         lcd_putc('*');
         UNI=VAL;
      }
      TECLADO();
      IF((k!='#')&&(k!='*'))
      {
         lcd_putc('*');
         DEC=VAL;
      }
      TECLADO();
      IF((k!='#')&&(k!='*'))
      {
         lcd_putc('*');
         CEN=VAL;
      }
      TECLADO();
      IF((k!='#')&&(k!='*'))
      {
         lcd_putc('*');
         MIL=VAL;
      }
      TECLADO();
      WHILE((k!='#'))
      {
         TECLADO();
      }
      IF((UNI==1)&&(DEC==2)&&(CEN==3)&&(MIL==4)) //Aquí se compara si
      //los números digitados
      //están correctos.*/
      {
         LCD_PUTC("\f"); //Se borra LCD
         LCD_GOTOXY(1,1);//Se ubica en la posición 1,1
         LCD_PUTC(" CLAVE CORRECTA ");
         BIT_SET(LED1);
         DELAY_MS(2000);
         BIT_CLEAR(LED1);
      }
      ELSE
      {
         LCD_PUTC("\f");
         LCD_GOTOXY(1,2);
         LCD_PUTC(" CLAVE INVALIDA ");
         BIT_SET(LED2);
         DELAY_MS(4000);
         BIT_CLEAR(LED2);
      }
   }
}
