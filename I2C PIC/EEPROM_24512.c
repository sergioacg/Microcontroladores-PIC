
void write_ext_eeprom(long int address, BYTE data)
{
   short int status;
   i2c_start();            //Inicializa la Transmisión
   i2c_write(0xa0);        //Escribe la palabra de control(direccion
                           //0h + 0 para escritura)
   i2c_write(address>>8); //Parte alta de la dirección a escribir en la eeprom
   i2c_write(address);    //Parte baja de la dirección a escribir en la eeprom
   i2c_write(data);     //Dato a escribir
   i2c_stop();          //Finalización de la transmisión
   i2c_start();         //Reinicio
   status=i2c_write(0xa0); //Lectura del bit ACK, para evitar escrituras
                           //incorrectas
   while(status==1)     //Si es "1" se espera para responder al esclavo
   {
      i2c_start();
      status=i2c_write(0xa0);
   }
}

BYTE read_ext_eeprom(long int address) {
   BYTE data;
   i2c_start();            //Inicializa la Transmisión
   i2c_write(0xa0);        //Escribe la palabra de control(direccion
                           //0h + 0 para escritura)
   i2c_write(address>>8); //Parte alta de la dirección a escribir en la eeprom
   i2c_write(address);    //Parte baja de la dirección a escribir en la eeprom
   i2c_start();         //Reinicio
   i2c_write(0xa1);        //Escribe la palabra de control(direccion
                           //0h + 1 para escritura)
   data=i2c_read(0);    //Lectura del Dato
   i2c_stop();          //Finalización de la transmisión
   return(data);
}
