// MAESTRO en SPI

//#include <18F4550.H> 
#include <16F887.H>

#device ADC=10
#fuses XT, NOWDT, NOPROTECT, BROWNOUT, PUT, NOLVP 
#use delay(crystal=4000000) 
//#use rs232(baud=9600,xmit=PIN_D0,rcv=PIN_D1,stream=PC, parity=N, bits=8)
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7,stream=PC, parity=N, bits=8)

#define SPI_SS   PIN_C2 

// Modos de comunicación SPI 
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H) 
#define SPI_MODE_1  (SPI_L_TO_H) 
#define SPI_MODE_2  (SPI_H_TO_L) 
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 


// Comandos SPI que acepta el esclavo 
#define LEER_ADC          1 
#define LEER_CONTADOR     2 
#define LEER_INTERRUPTOR  3 

//====================================== 
void main() 
{ 
char c; 
int8 resultado; 

output_high(SPI_SS);  // Inicializa el Pin SS en alto para deshabilitar el esclavo

//Inicializa el hardware SSP para ser un SPI Maestro en Modo 0
setup_spi(SPI_MASTER | SPI_MODE_0 | SPI_CLK_DIV_4); 

printf("Comandos para el Esclavo:\n\r"); 
printf("A: Leer el ADC\n\r"); 
printf("B: Leer el interruptor\n\r"); 
printf("C: Leer el contador\n\r"); 
printf("Presione la tecla comando\n\r"); 

// Si presiona la tecla correcta manda la orden al esclavo
while(1) 
  { 
   c = getc(); //Captura el caracter del puerto serial RS232
   c = toupper(c); //Convierte el caracter en mayusculo
  
   switch(c) 
    { 
     case 'A':     // Leer ADC en el Esclavo 
       output_low(SPI_SS); 
       spi_write(LEER_ADC);  // Envia el comando 
       output_high(SPI_SS); 

       delay_us(100);  // Tiempo para que el esclavo responda

       output_low(SPI_SS); 
       resultado = spi_read(0);     // Lee la respuesta del esclavo 
       output_high(SPI_SS); 
    
       printf("Valor ADC = %X \n\r", resultado); 
       break; 

     case 'B':     // Lee el interruptor en el esclavo 
       output_low(SPI_SS); 
       spi_write(LEER_INTERRUPTOR); // Envia el comando 
       output_high(SPI_SS); 

       delay_us(100); 

       output_low(SPI_SS); 
       resultado = spi_read(0);     //  Lee la respuesta del esclavo 
       output_high(SPI_SS); 
    
       printf("Interruptor = %X \n\r", resultado); 
       break; 


     case 'C':     // Lee el contador en el esclavo 
       output_low(SPI_SS); 
       spi_write(LEER_CONTADOR); // Envia el comando  
       output_high(SPI_SS); 

       delay_us(100); 

       output_low(SPI_SS); 
       resultado = spi_read(0);     //  Lee la respuesta del esclavo  
       output_high(SPI_SS); 
    
       printf("Countador = %X \n\r", resultado); 
       break; 

      
     default: 
       printf("%c es un caracter invalido.\n\r", c); 
       break; 
    } 

  } 

}
