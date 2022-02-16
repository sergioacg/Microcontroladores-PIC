# Librería HX711 para Microcontrolador PIC en PIC C Compiler CCS C
By: Sergio Andres Castaño Giraldo
* website: https://controlautomaticoeducacion.com/                                  
* YouTube Chanel: https://www.youtube.com/channel/UCdzSnI03LpBI_8gXJseIDuw  

## Características
1. Proporciona una función *tare()*  para reestablecer la balanza a 0.

1. Proporciona una función *power_down()* para poner el ADC en un modo de bajo consumo. De acuerdo con la hoja de datos.
Caso de uso: Balanzas alimentadas por batería. En consecuencia, hay una función *power_up()* para despertar el chip del modo de bajo consumo.

1. Tiene una función set_gain(byte gain) que le permite establecer el factor de ganancia y seleccionar el canal. De acuerdo con la hoja de datos.

1. Las funciones *get_value( )* y  *get_units()* pueden recibir un parámetro adicional "times", y devolverán el promedio de múltiples lecturas en lugar de una sola lectura.
