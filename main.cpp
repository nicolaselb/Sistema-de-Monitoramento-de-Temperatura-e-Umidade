#include "mbed.h"
#include <string.h>
#include <stdio.h>
#include "DHT.h"
#include "ModbusSlave.h"
#include "MODSERIAL.h"

MODSERIAL Modbus(USBTX,USBRX);

DigitalOut ENABLE(PTE0);

DigitalOut BUZZER(PTC1);
DigitalOut LED_VERM(PTC2);

DigitalOut RELE(PTB3);
DigitalOut LED_VERD(PTB2);


//////////////// MODBUS Registers (Slave) ///////////////////
enum 
{     
  // just add or remove registers and your good to go...
  // The first register starts at address 0
  TEMP,         // Pot analog read
  UMID,         // Temp analog read
  DATA,
  HOLDING_REGS_SIZE 
  // total number of registers for function 3 and 16 share the same register array
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array

////////////////////////////////////////////////////////////


DHT sensor(PTB0,DHT22);
Timer t;

int main()
{
    //CONFIGURE MODBUS  
    modbus_configure(&Modbus, 9600, 1, 2, &ENABLE, HOLDING_REGS_SIZE, holdingRegs);    
    wait(1);
    t.start();
    while (true)
    {
        modbus_update();

        if(t.read() >=  5){
        
            sensor.readData();
            holdingRegs[TEMP] = (int)(10*sensor.ReadTemperature(CELCIUS));
            holdingRegs[UMID] = (int)(10*sensor.ReadHumidity());
            
            t.reset();
         
        }
        
        int flag;
                
        flag = holdingRegs[DATA];
            
            if (flag == 1){
                RELE = 0;
                BUZZER = 1;
            } else if(flag == 2){
                RELE = 1;
                BUZZER = 0;
                }else{
                    BUZZER = 0;
                    RELE = 0;
                    }
     }
      
}