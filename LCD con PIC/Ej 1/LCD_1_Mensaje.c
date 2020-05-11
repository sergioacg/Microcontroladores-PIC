#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#INCLUDE <LCD.C>
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP

void MAIN()
{
   LCD_INIT();       //Inicializa el LCD
   LCD_PUTC("\f");   //Borrar el contenido del LCD
   WHILE(TRUE)
   {
      LCD_GOTOXY(5,1);        //Se ubica En la columna 5 fila 1
      LCD_PUTC("Control");    //Muestre el mensaje “Control”
      LCD_GOTOXY(3,2);        //Ubicarse en la columna 3 fila 2
      LCD_PUTC("Automatico"); // Sacar el mensaje “Automatico”
   } //Cierra while true
}
