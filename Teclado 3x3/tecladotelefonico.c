#INCLUDE <16F887.H>
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP,WRT
#USE DELAY(CLOCK=4000000)
Byte CONST display[10]= {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x67};
#DEFINE USE_PORTB_KBD
#INCLUDE <KBD.C>
#BYTE PORTC= 7
#BYTE PORTB= 6
#BYTE WPUB= 0X95 // declarar registro option
#BYTE OPTION_REG = 0X81

CHAR K;


VOID MAIN()
{
   //PORT_B_PULLUPS(TRUE);
   OPTION_REG = 0; //en el MAIN desactivas el registro OPTION
   WPUB= 0B11111111;// y seleccionas los pullups q vas a utilizar
   
   KBD_INIT(); //Inicializar la rutina del teclado
   SET_TRIS_C(0); //Configurar el puerto C como salida
   PORTC=(display[0]);//Muestre en el display el número cero
   WHILE (TRUE)
   {
      K=0; //Ponga la variable K en cero
      K=KBD_GETC(); //Captura cualquier tecla oprimida
      IF(K=='0') //Si la tecla que se oprime es igual al caracter cero
         PORTC=(display[0]);//Muestre en el display el número cero
      IF(K=='1') //Si la tecla que se oprime es igual al caracter uno
         PORTC=( display [1]);//Muestre en el display el número uno
      IF(K=='2') //Tecla que se oprime = al caracter dos
         PORTC=( display [2]); //Muestre en el display el número dos
      IF(K=='3') // Tecla que se oprime = al caracter tres
         PORTC=( display [3]);//Muestre en el display el número tres
      IF(K=='4') // Tecla que se oprime = caracter cuatro
         PORTC=( display [4]);//Muestre en el display el número cuatro
      IF(K=='5') // Tecla que se oprime = caracter cinco
         PORTC=( display [5]);//Muestre en el display el número cinco
      IF(K=='6') // Tecla que se oprime = caracter //seis
         PORTC=( display [6]);//Muestre en el display el número seis
      IF(K=='7') // Tecla que se oprime = caracter
      //siete
         PORTC=( display [7]);//Muestre en el display el número siete
      IF(K=='8') // Tecla que se oprime = caracter
      //ocho
         PORTC=( display [8]);//Muestre en el display el número ocho
      IF(K=='9') // Tecla que se oprime = caracter
      //nueve
         PORTC=( display [9]);//Muestre en el display el número nueve
   }
}
