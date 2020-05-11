// ESCLAVO Comunicación SPI 

//#include <18F4550.H> 
#include <16F887.H>
#device adc=8 
#fuses XT, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP 
#use delay(crystal=4000000) 
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7) 


#byte SSPBUF = 0x13  // Dirección del Registro para 16F877 
//#byte SSPBUF = 0xFC9  // Dirección del Registro para 18F4550


// Modos de comunicación SPI 
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H) 
#define SPI_MODE_1  (SPI_L_TO_H) 
#define SPI_MODE_2  (SPI_H_TO_L) 
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 

// Comandos SPI de este esclavo 
#define LEER_ADC          1 
#define LEER_CONTADOR     2 
#define LEER_INTERRUPTOR  3 

// Definición del PIN donde se conecta el interruptor. 
#define SWITCH_PIN  PIN_B4 

// Variable global para almacenar el ADC 
int8 pot; 

// Interrupción por SPI donde se analiza las solicitudes del Maestro
#int_ssp 
void ssp_isr(void) 
{ 
int8 comando; 
static int8 contador = 0; 

comando = SSPBUF; //Toma el valor del buffer SPI y lo almacena en comando

switch(comando)   // Verifica la instrucción del maestro
  { 
   case LEER_ADC: 
     SSPBUF = pot; 
     break; 

   case LEER_CONTADOR: 
     SSPBUF = contador; 
     contador++;          
     break;  

   case LEER_INTERRUPTOR: 
     SSPBUF = input(SWITCH_PIN); 
     break; 
  } 

} 


//====================================== 
void main() 
{ 
// Configura AN0 como analoga
//setup_adc_ports(AN0);    //18F4550
setup_adc_ports(sAN0); //16F887
setup_adc(ADC_CLOCK_DIV_8); 

set_adc_channel(0); 
delay_us(20); 
pot = read_adc(); 


//Inicializa el hardware SSP para ser un SPI esclavo en Modo 0
setup_spi(SPI_SLAVE | SPI_MODE_0); 

// Habilita las interrupciones
clear_interrupt(INT_SSP); 
enable_interrupts(INT_SSP); 
enable_interrupts(GLOBAL); 

// Actualiza la lectura analoga cada 100ms 
while(1) 
  { 
   pot = read_adc(); 
   delay_ms(100); 
  } 

}
