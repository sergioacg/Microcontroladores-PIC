#INCLUDE <16f887.h>
#fuses XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#use delay(clock=4000000) //Crystal Interno 4MHz
//#use delay(clock=4000000,crystal)//Crystal Externo 4MHz
#byte PORTB=6
#byte PORTC=7
#define SW PORTC,0

INT CONT;
byte const HORARIO[4] = {  0b1100,
                           0b0110,
                           0b0011,
                           0b1001};
                           
byte const ANTIH[4] ={     0b1001,
                           0b0011,
                           0b0110,
                           0b1100};
                           
void main()
{
   SET_TRIS_C(0B11111111);                //Configura el puerto C
   SET_TRIS_B(0B00000000);                //Configura el puerto B
   WHILE(TRUE)
   {
      IF(BIT_TEST(SW))                    // Pregunta si SW esta encendido
      {
         CONT=0;                          //Se pone Cont en cero
         WHILE((CONT<4)&&(BIT_TEST(SW)))  //Mientras que cont sea menor a 4 
                                          //y SW=1(encendido)
         {
            PORTB=(HORARIO[CONT]);        //Envíe al puerto B la información //de
                                          //la tabla de horario
            DELAY_MS(100);                //Retardo de 100 milisegundos
            CONT++;                       //Incremente la variable cont
         }
      }
      ELSE                                //de lo contrario
      {
         CONT=0;                          // la variable cont =0
         WHILE((CONT<4)&&(!BIT_TEST(SW))) //Mientras que cont sea menor a 4 
                                          //y SW=0(apagado)
         {
            PORTB=(ANTIH[CONT]);          //Envíe al puerto B la información de
                                          //la tabla de antihorario
            DELAY_MS(100);                //Retardo de 100 milisegundos
            CONT++;                       //Incremente la variable cont
         }
      }
   }
}
