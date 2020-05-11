#INCLUDE <16F887.H>
#FUSES XT,NOLVP,NOWDT,PUT
#USE DELAY(CLOCK=4000000)
Byte CONST display[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
#BYTE PORTC = 7
#BYTE PORTB = 6
INT CONT=0;


#INT_EXT             //Funcion de la Interrupcion
VOID INTERRUPCION()
{
   DELAY_MS(200);    //Retardo
   CONT++;           //Incremente la variable CONT
   IF(CONT==10)      //Si la variable CONT es igual a 10
      CONT=0;        //Ponga en cero la variable CONT
}

VOID MAIN()
{
   SET_TRIS_B(0B11111111);       //Configurar el puerto B
   SET_TRIS_C(0);                //Configurar el puerto C
   ENABLE_INTERRUPTS(GLOBAL);    //Habilita todas las interrupciones
   ENABLE_INTERRUPTS(INT_EXT);   //Habilita la interrupción externa
   ext_int_edge(H_TO_L); 
   WHILE(TRUE)
   {
      PORTC= (display[CONT]);    //Muestra lo que hay en la variable CONT en el display
   }
}
