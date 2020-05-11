// ***********************************************************************//
// ***********************************************************************//
// *****       By: Sergio Andres Castaño Giraldo                    ******//
// *****       https://controlautomaticoeducacion.com/              ******//
// *****       Control Engineering                                  ******//
// ***********************************************************************//
// ***********************************************************************//

#include <18f4550.h>
#DEVICE ADC=10
#USE DELAY(crystal=20000000)
#FUSES HS,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP
#byte porta = 0xf80 // Identificador para el puerto A. 
#byte portb = 0xf81 // Identificador para el puerto B. 
#byte portc = 0xf82 // Identificador para el puerto C. 
#byte portd = 0xf83 // Identificador para el puerto D. 
#byte porte = 0xf84 // Identificador para el puerto E.
 
#use i2c(Master,Fast=100000, sda=PIN_B0, scl=PIN_B1,force_sw) 
#include <i2c_Flex_LCD.c>

double Level,Vout,P,Vs=4.7;
double aux;
double tolP=-0.27; // Ajusta la medida de presión

int i, rho = 997;
double g=9.8;

void main() {  
lcd_init(0x4E,16,2);
lcd_backlight_led(ON); //Enciende la luz de Fondo
lcd_clear();  //Limpia el LCD

setup_adc_ports(AN0); //Entradas analogas
setup_adc(adc_clock_internal);//Habilito el reloj interno de la conversion

 while (TRUE) { 
   set_adc_channel(0); //especifico q usare el puerto RA0
   //Voltaje del Sensor MPC5010DP
   aux=0;
   for(i=0;i<20;i++){
      aux = aux + (read_adc()*5.0/1023.0);
      delay_ms(5);
   }
   Vout=aux/20.0;
  
  //PresiÃ³n en Kpa segÃºn grÃ¡fica 4 del Datasheet
  P = ( Vout - 0.04*Vs ) / (0.09 * Vs) + tolP; //Kpa
   
  Level = ((P*1000)/(rho*g))*100;  //Medida de Nivel del tanque

  Level = 0.2327*Level + 0.8001;
  
  lcd_gotoxy(1, 1);
  printf(lcd_putc, "Presion: %2.2fkPa    ",P);
  lcd_gotoxy(1, 2);
  printf(lcd_putc, "Altura: %2.2fcm   ",Level); 
  delay_ms(3000);
  
 }
}
