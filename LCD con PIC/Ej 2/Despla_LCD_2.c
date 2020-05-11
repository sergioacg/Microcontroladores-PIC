#include <16f887.h>
#fuses XT, NOPROTECT, NOWRT, NOPUT, NOWDT, NOLVP, NOCPD //Órdenes para el programador
#use delay (clock=4000000)
#include <lcd.c>  // Librería para el manejo del lcd              

const char mensaje[]= {"Control Automatico Educacion"};

///PROGRAMA

void main(void)
{
   char y=1; //índice fíla
   signed char x=16; //índice columnas (posiciones)
   lcd_init(); //inicializa lcd 

   while(TRUE){ //bucle...
      lcd_gotoxy(x,y) ; //ubica cursor para escribir mensaje
      lcd_putc(mensaje ) ; //muestra por pantalla el mensaje
      delay_ms(100);
      x--; //incremento indice de columnas
      if(x<-28)
      {
         //¿se ha mostrado mensaje entero en la primera fila?
         x=16; //SI, índice columna x=-16, para que inicie el último caracter en la primera posición
         y++; //incremento índice fila (mostrar texto en segunda columna)
         if(y>2) //¿se ha mostrado mensaje por segunda fila?
            y=1; //SI, restaurar índice de fila
      }
      lcd_putc("\f" ) ; //borra pantalla

   } //...infinito

}
