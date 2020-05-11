#include <16f887.h>
#fuses hs,nowdt
#use delay(clock=4000000)


void main()
{
  int i;
  int tabla[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7c, 0x07, 0x7f, 0x67};
  set_tris_c(0b00010000);            // Configuración del puerto C para trabajar el spi
  setup_spi(spi_master | spi_l_to_h | spi_clk_div_16);
 while(1)
 {
  for(i = 0; i<10; i++)
       {
           spi_write(tabla[1]);
           delay_ms(1000);
       }
 }

}
