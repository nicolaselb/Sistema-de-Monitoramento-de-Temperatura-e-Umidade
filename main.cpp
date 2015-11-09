#include "mbed.h"
#include <string.h>
#include <stdio.h>

#include "ModbusSlave.h"

#include "MODSERIAL.h"

MODSERIAL Modbus(USBTX,USBRX);

SPI myspi(PTD2, PTD3, PTD1); // mosi, miso, sclk

//////////////// TERMOCOUPLERS //////////////////////////////

DigitalOut T1(PTC9);
DigitalOut T2(PTA13);
DigitalOut T3(PTD0);
DigitalOut T4(PTB3);

DigitalOut ENABLE(PTC1);

//DigitalOut LED(LED_RED);

float temp1 = 0;
float temp2 = 0;
float temp3 = 0;
float temp4 = 0;


//////////////// MODBUS Registers (Slave) ///////////////////
enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  TERM_1,         // Pot analog read
  TERM_2,         // Temp analog read
  TERM_3,         // Light analog read
  TERM_4,         // Moisture analog read
  HOLDING_REGS_SIZE 
  // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array

////////////////////////////////////////////////////////////



uint16_t thermoparRead(void)
{    
    uint16_t high = 0;
    uint16_t low = 0;
    
    bool scToVcc;
    bool scToGnd;
    bool opened;
    bool fail;
    
    high = ((myspi.write(0)<<8)&0xff00) +  (myspi.write(0)&0x00ff);
    low =  ((myspi.write(0)<<8)&0xff00) +  (myspi.write(0)&0x00ff);
    
    // Get errors from data
    scToVcc = low & 0x0004;
    scToGnd = low & 0x0002;
    opened = low & 0x0001;
    fail = high & 0x0001;
    
    if(opened) return 2;
    if(fail) return 1;
    return high&0xfffc;
}


int main()
{
    //CONFIGURE MODBUS  
    modbus_configure(&Modbus, 9600, 1, 2, &ENABLE, HOLDING_REGS_SIZE, holdingRegs);    

    while (true)
    {
        modbus_update();  
 
        T1 = 0;
        holdingRegs[TERM_1] = thermoparRead();
        T1 = 1;
        
        T2 = 0;
        holdingRegs[TERM_2] = thermoparRead();
        T2 = 1;
        
        T3 = 0;
        holdingRegs[TERM_3] = thermoparRead();
        T3 = 1;
        
        T4 = 0;
        holdingRegs[TERM_4] = thermoparRead();
        T4 = 1; 
     }
}



