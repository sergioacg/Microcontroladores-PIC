int BCDaBIN(BYTE bcd)
{
   int varia;
   varia=bcd;
   varia>>=1;
   varia &= 0x78;
   return (varia + (varia>>2) + (bcd & 0x0f));
}

void rtc_get_time(BYTE& hr, BYTE& min, BYTE& sec) {
   i2c_start();   //Escritura
   i2c_write(0xD0);  //Codigo de escritura
   i2c_write(0x00);  //Puntero de la primera dirección
   i2c_start();   //Lectura
   i2c_write(0xD1);  //Codigo de lectura
   sec = BCDaBIN(i2c_read()&0x7f); //Lectura de los 7 bit de los segundos
   min = BCDaBIN(i2c_read()&0x7f); //Lectura de los 7 bit de los minutos
   hr = BCDaBIN(i2c_read(0)&0x3f); //Lectura de los 6 bit de las horas
   i2c_stop(); //Finaliza comunicación
}
