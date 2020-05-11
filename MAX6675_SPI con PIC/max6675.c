/************************************************************************************** 
*   max6675.c - communicates with a MAX6675 thermcouple interface chip                * 
*   Copyright Jimbob's Ma 2006                                                        * 
*                                                                                     * 
*   This program is free software; you can redistribute it and/or                     * 
*   modify it under the terms of the GNU General Public License                       * 
*   as published by the Free Software Foundation version 2                            * 
*   of the License.                                                                   * 
*                                                                                     * 
*   This program is distributed in the hope that it will be useful,                   * 
*   but WITHOUT ANY WARRANTY; without even the implied warranty of                    * 
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                     * 
*   GNU General Public License for more details.                                      * 
*                                                                                     * 
*   You should have received a copy of the GNU General Public License                 * 
*   along with this program; if not, write to the Free Software                       * 
*   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.   * 
**************************************************************************************/ 

/* 
This is a diver for the MAX6675 K-type thermocouple interface chip. It implements an SPI 
bus without the need for dedicated hardware (aka a bit-banged interface). The result from 
toFloat_TC() is the temperature in degrees celcius of the thermocouple tip. The rest should 
be self-evident. Have a look at the end of the file for example usage. 
*/ 

#ifndef TC_CLK 
   #define TC_CLK     PIN_C3            //edit these pins as necessary 
#endif 

#ifndef TC_CS 
   #define TC_CS      PIN_C0 
#endif 

#ifndef TC_DATA 
   #define TC_DATA    PIN_C4 
#endif 


int1 thermocouple_error;         //a handy dandy global error flag to tell you if a thermocouple is connected or not 

void init_TC(int CS) 
{ 
   output_low(TC_CLK); 
   output_low(TC_DATA); 
   output_high(CS);            //if we idle high, the chip keeps doing conversions. Change this if you like 
} 

int16 read_TC(int CS)               //It takes 200ms (ish) for the MAX6675 to perform a conversion 
{ 
   int8 i; 
   int16 data; 

   output_low(CS);            //stop any conversion processes 
   delay_ms(1);               //and give it some time to power up (not very much, admittedly) 

   for (i=0;i<16;i++){ 
      shift_left(&data,2,input(TC_DATA));      //reads in 2 bytes to data from the pin TC_DATA 
      output_high(TC_CLK); 
      delay_us(1); //Descomentar si usa crystal mayor a 10MHz
      output_low(TC_CLK); 
      delay_us(1);
   } 

   thermocouple_error=bit_test(data,2);      //this is the thermocouple status bit 
   delay_ms(1);     
   output_high(CS); 
   return(data); 
} 

int16 sortout(int16 raw) 
{ 
    return(0x0FFF & (raw>>3));      //returns only the bits converning temperature 
} 

float toFloat_TC(int16 tmp) 
{ 
   //return((((float)tmp/4.9127+459)*9/5)+32);
   return((float)tmp/2.021142857-25);      //adjusts data to floating point format, and accounts for the decimal point 
   //return((float)tmp/2.021142857-30); 
   //return((float)tmp);
} 

float do_everything(int CS) 
{ 
   init_TC(CS); 
   delay_ms(200);               //200ms is a long time to be doing nothing. use a timer interrupt to avoid wasting time here 
   return(toFloat_TC(sortout(read_TC(CS)))); 
} 


