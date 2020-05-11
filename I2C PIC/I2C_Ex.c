#INCLUDE <16F887.h>
#DEVICE ADC=10
#USE DELAY(CLOCK=4000000)
#FUSES XT,NOPROTECT,NOWDT,NOBROWNOUT,PUT,NOLVP

//Configura direccion de memoria de los puertos A,B,C,D
#BYTE PORTA= 5
#BYTE PORTB= 6
#BYTE PORTC= 7
#BYTE PORTD= 8

#define EEPROM_SDA  PIN_C4
#define EEPROM_SCL  PIN_C3

#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7, bits=8, parity=N)
#use i2c(MASTER, sda=EEPROM_SDA, scl=EEPROM_SCL)

#include <lcd.c>
#include <EEPROM_24512.c>
#include <DS1307.c>
#include <TEMP_DS1621.c>
#include <floatee.c>

int dat_in=0, cnt, hr, min, sec;
int16 address=0;
int dat_serie[7];
float dato=0;

//Interrupción por el puerto serial
#int_rda
rda_isr()
{
   dat_in=getc();    //Lee el puerto Serial
   printf("%c\r",dat_in);
   if (dat_in=='2')  //Pregunta si es "2" para visualizar parametros de la EEPROM
   {
      for(cnt=address-7;cnt<=address;cnt++)  //Lee los primeros 7 bytes de la EEPROM
      {
         dat_serie[cnt]=read_ext_eeprom(cnt);
         printf("Byte %u=%3u \r", cnt, dat_serie[cnt]);
      }
   }
}

void main()
{
   lcd_init();
   set_tris_c(255);
   enable_interrupts(int_rda);
   enable_interrupts(global);
   address=0;
   
   //Crea un Menu para mostrar por el Serial
   printf("Pulse 1 para leer los datos\r");
   printf("Pulse 2 para visualizar los datos\r");
   
   while(1)
   {
      if(dat_in=='1') //Si es "1" inicie la lectura y grabado de la EEPROM
      {
         init_temp(0x03);  //Inicializa el DS1621
         rtc_get_time(hr,min,sec);  //Lee el tiempo del DS1307
         dato=read_full_temp(0x03); //Lee temperatura del DS1621
         write_float_ext_eeprom(address,dato);  //Guarda 4 bytes del float
         address=address+4;
         
         write_ext_eeprom(address++,hr); //Guarda byte de Hora
         write_ext_eeprom(address++,min); //Guarda byte de Minuto
         write_ext_eeprom(address++,sec); //Guarda byte de Segundo
         
         lcd_gotoxy(1,1);
         printf(lcd_putc,"Temp=%4.1f C    \n",dato);//Visualiza la temperatura
         printf(lcd_putc,"%2u:%2u:%2u",hr,min,sec);//Visualiza la hora
         if (address==0xffff)
            address=0; //Cuando se termina la EEPROM vuelve al principio
         dat_in=0;
      }
   }
}
