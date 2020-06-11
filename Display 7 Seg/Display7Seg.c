#INCLUDE <16F887.H>
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
BYTE CONST DISPLAY[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
#BYTE PORTB=6
#BYTE PORTC=7
#DEFINE SW1 PORTC,0
#DEFINE SW2 PORTC,1
#DEFINE LED PORTC,2
INT CONTADOR;

MAIN()
{
   SET_TRIS_B(0B00000000);             //Configura el puerto B
   SET_TRIS_C(0B00000011);             //Configura el puerto B
   CONTADOR = 0;                       // Inicializa contador en cero
   WHILE(TRUE)                         // Haga por siempre
   {
      PORTB= DISPLAY[CONTADOR];
      BIT_CLEAR(LED);
      IF (BIT_TEST(SW1))
      {
         DELAY_MS(200);                // Antirrebote
         while (BIT_TEST(SW1));
         CONTADOR++;
         PORTB= DISPLAY[CONTADOR];
         IF(CONTADOR==10)
         {
            CONTADOR=0;
         }
      }
      IF (BIT_TEST(SW2))
      {
         DELAY_MS(200);             //Antirrebote
         BIT_SET(LED);
         WHILE (CONTADOR>0)
         {
            PORTB= DISPLAY[CONTADOR];
            DELAY_MS(1000);
            CONTADOR--;
         }
         BIT_CLEAR(LED);
      }
   }
}
