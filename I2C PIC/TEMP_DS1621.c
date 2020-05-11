
void init_temp(int address) { //address=0x03(en el ejemplo)
   i2c_start();
   //Ver pagina 8 datasheet
   i2c_write(0x90|(address<<1)); //Genera primer byte (1001A2A1A0W) "1" lectura "0" escritura
   i2c_write(0xee); //Inicia conversion
   i2c_stop();
}


float read_full_temp(int address) {  
   float tura;
   BYTE datah;
   BYTE datal;
   int data;
   

   i2c_start();
   i2c_write(0x90|(address<<1)); //Genera primer byte (1001A2A1A0-W)
   i2c_write(0xaa);        //Lee temperatura
   i2c_start();
   i2c_write(0x91|(address<<1)); //Genera primer byte (1001A2A1A0-R)
   datah=i2c_read();       //Lectura parte alta
   datal=i2c_read(0);      //Lectura parte alta y NACK
   i2c_stop();
   
   data=datah;
   if (datal==128)  
      tura=data+0.5;
   else
      tura=data;
   return(tura);
}
