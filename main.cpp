#include "mbed.h"

BusOut rgb(LED_RED,LED_GREEN,LED_BLUE);

int main() {
    int i=0;   
    while(1) {
        rgb=i;
        wait (0.25);
        i++;            
     }
}