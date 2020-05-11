#include <16f887.h>
#fuses hs,nowdt
#use delay(clock=4000000)
#byte portb=6

void main()
{
  //Puerto B como Salida
  set_tris_b(0);
  set_tris_c(0b11011111);
  portb = 0x3f;
  //spi_slave = Configura el PIC como Esclavo
  //spi_l_to_h = Flanco de subida en muestreo y transmisión
  setup_spi(spi_slave | spi_clk_div_16| SPI_XMIT_L_TO_H);
 for(;;)
 {
  if(spi_data_is_in())
       {
         portb = spi_read();
       }
 }
}
