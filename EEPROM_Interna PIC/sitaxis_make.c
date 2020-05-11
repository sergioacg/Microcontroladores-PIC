   int8 Data_h, Data_L;
   int16 DataLong;
   
   Data_H=make8(DataLong,1);
   Data_L=make8(DataLong,0); 
   
   WRITE_EEPROM(0,Data_H);
   WRITE_EEPROM(1,Data_L);
   
   //Juntar Datos
   int8 Data_h, Data_L;
   int16 DataLong; 
   
   Data_H=READ_EEPROM(0);
   Data_L=READ_EEPROM(1); 

   DataLong=make16(Data_H,Data_L);
