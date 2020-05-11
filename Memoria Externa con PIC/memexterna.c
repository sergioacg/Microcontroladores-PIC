/****************************************************************
*****************************************************************
******                                                  *********
******    Ejemplo del uso de la memoria interna EEPROM  *********
******                                                  *********
******    By: Sergio Andres Castaño Giraldo             *********
******    https://controlautomaticoeducacion.com/       *********
******                                                  *********
*****************************************************************
****************************************************************/

#INCLUDE <16f887.h>
#USE DELAY(CLOCK=4000000,crystal)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,NOPUT,NOLVP

//Definiciones para colocar el LCD y el teclado en el puerto B
#DEFINE USE_PORTB_LCD TRUE
#DEFINE USE_PORTB_KBD TRUE  
 
//Librerías del LCD y Teclado
#INCLUDE <LCD.C>
#INCLUDE <KBD.C> 

/*Definir SDA y SDL (PIN C4 y C3 por defecto en la librería)
Descomentar en caso de usar otros pines en el I2C*/
//#define EEPROM_SDA  PIN_C4
//#define EEPROM_SCL  PIN_C3

//Librería de la Memoria Externa
#INCLUDE <2404.C>


CHAR K;           //Defino la variable K como carácter
BYTE J=16,DIR=0;

//Programa principal
void main()
{
   // Inicializaciones
   LCD_INIT();
   KBD_INIT();
   INIT_EXT_EEPROM();
   
   //Configura las Resistencias PULL-UP del Puerto B
   port_b_pullups(0xFF); //PIC16F887
   //port_b_pullups(TRUE); //PIC16F877A

   FOR (J=12; J>=2; --J)
   {
      LCD_GOTOXY(J,1);
      LCD_PUTC("DIGITADO: ");
      DELAY_MS(100); //Retardo de 100 milisegundos
   }
   LCD_GOTOXY(12,1);    // Ubicarse en la columna 12 fila 1
   DIR=0;               // Poner la variable DIR en cero
   

   WHILE (DIR<4)                       // Mientras que DIR sea menor que 4
   {
      K=0;                             // Poner la variable K en cero
      K=KBD_GETC();
      IF( (K!='\0'))
      {
         LCD_PUTC(K);
         WRITE_EXT_EEPROM(DIR,K);
         DIR++;                     //Incrementa la variable DIR
      }
   }
   DELAY_MS(500);                   //Retardo de 500 milisegundos
   
   FOR (J=12; J>=2; --J)
   {
      LCD_GOTOXY(J,2); //Ubicarse en la fila 2 y la columna varía
      LCD_PUTC("RECUPERADO: ");
      DELAY_MS(100); // Retardo de 100 milisegundos
   }
   LCD_GOTOXY(13,2); // Ubicarse en la columna 13 fila 2
   DIR=0; // Poner la variable DIR en cero
   
   WHILE (DIR<4) // Mientras que DIR sea menor a 4
   {
      K=READ_EXT_EEPROM(DIR);
      LCD_PUTC(K);
      DIR++; //Incrementa la variable DIR
   }
}
