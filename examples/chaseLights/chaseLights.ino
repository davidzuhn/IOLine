/*
 * this sketch does a simple chase light across all of the outputs
 * defined.
 *
 * Copyright 2013, 2014 by david d zuhn <zoo@whitepineroute.org>
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/deed.en_US.
 *
 * You may use this work for any purposes, provided that you make your
 * version available to anyone else.
 */

#define IOLINE_USES_IOX 1
#include <IOLine.h>
#include <Metro.h>

// change this to suit whatever address you have defined your IOX board as
#define IOX16 0x24
#define IOX32 0x20

IOLine *outputs[] = {

    // yeah, I know, this looks funny, but it happens to match what my
    // hardware looks like today.   I needed to use A5 for an analog input,
    // and everything else stayed in place.  9 is already my pilot light.

    new Pin(8, OUTPUT),
    new Pin(A4, OUTPUT),
    new Pin(A3, OUTPUT),
    new Pin(A2, OUTPUT),
    new Pin(A1, OUTPUT),
    new Pin(A0, OUTPUT),
    new Pin(13, OUTPUT),
    new Pin(12, OUTPUT),
    new Pin(11, OUTPUT),
    new Pin(10, OUTPUT),

    // half of my IOX16 is outputs, but not even divided between
    // ports 0 & 1 -- this is more for purposes of demonstration
    // than for idealized wiring configuration.  

    new IOX(IOX16, 0, 4, OUTPUT),
    new IOX(IOX16, 0, 3, OUTPUT),
    new IOX(IOX16, 0, 2, OUTPUT),
    new IOX(IOX16, 0, 1, OUTPUT),
    new IOX(IOX16, 0, 0, OUTPUT),
    new IOX(IOX16, 1, 2, OUTPUT),
    new IOX(IOX16, 1, 1, OUTPUT),
    new IOX(IOX16, 1, 0, OUTPUT),


    // yeah, I have six open output lines on the left side 
    new IOX(IOX32, 0, 3, OUTPUT),
    new IOX(IOX32, 0, 2, OUTPUT),
    new IOX(IOX32, 0, 1, OUTPUT),
    new IOX(IOX32, 0, 0, OUTPUT),
    new IOX(IOX32, 1, 7, OUTPUT),
    new IOX(IOX32, 1, 6, OUTPUT),
    new IOX(IOX32, 1, 5, OUTPUT),
    new IOX(IOX32, 1, 4, OUTPUT),
    new IOX(IOX32, 1, 3, OUTPUT),
    new IOX(IOX32, 1, 2, OUTPUT),

    // more densely packed on the right side, with port 1, bits 5-0 as
    // inputs (and not used in this sketch)
    new IOX(IOX32 + 1, 0, 7, OUTPUT),
    new IOX(IOX32 + 1, 0, 6, OUTPUT),
    new IOX(IOX32 + 1, 0, 5, OUTPUT),
    new IOX(IOX32 + 1, 0, 4, OUTPUT),
    new IOX(IOX32 + 1, 0, 3, OUTPUT),
    new IOX(IOX32 + 1, 0, 2, OUTPUT),
    new IOX(IOX32 + 1, 0, 1, OUTPUT),
    new IOX(IOX32 + 1, 0, 0, OUTPUT),
    new IOX(IOX32 + 1, 1, 7, OUTPUT),
    new IOX(IOX32 + 1, 1, 6, OUTPUT),
};

#define outputCount NELEMENTS(outputs)



// we continue to blink an LED on pin 9 to indicate that we're working fine
#define PILOTLIGHT 9

Metro pilotLight(500);          // change state every 500 ms


// we want to make a light change every so often, and for chase lights that
// should be pretty frequently
Metro chase(50);

void setup()
{
    // set up the pilot light LED
    pinMode(PILOTLIGHT, OUTPUT);

    // initialize the mode for each of my output lines
    for (int i = 0; i < outputCount; i++) {
        outputs[i]->init();
        outputs[i]->digitalWrite(LOW);
    }
}


int maxtail = 4;
int tail = 0;
int head = 0;


void updateChaseLights()
{
    outputs[head % outputCount]->digitalWrite(HIGH);

    if ((head - tail) >= maxtail) {
        if (tail >= 0) {
            outputs[tail]->digitalWrite(LOW);
        }
        tail += 1;
    }

    if (tail == outputCount) {
        head = maxtail - 1;
        tail = 0;
    }
    
    head += 1;
}



void loop()
{

    if (pilotLight.check()) {
        static int pilotLightState = 0;

        pilotLightState = !pilotLightState;
        digitalWrite(PILOTLIGHT, pilotLightState);
    }


    if (chase.check()) {
        updateChaseLights();
    }

}
