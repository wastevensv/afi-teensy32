
#include <core_pins.h>

extern void afi_main(void);
extern "C" int main(void) {

    char i = 1;
    pinMode(13, OUTPUT);

    afi_main();
    while(1)
    {
        digitalWrite(13, i);    
        delay(250);
        i ^= 1;
    }
}

