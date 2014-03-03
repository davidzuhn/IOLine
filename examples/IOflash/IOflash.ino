/* This program is designed to blink a lot of LEDs, using the mechanisms in
 * the IOLine library.
 *
 * The hardware I have configured for this demo consists of a cpNode board
 * plus one IOX expander.  10 output lines from the cpNode board each are
 * connected to a single LED through a limiting resistor.  10 output lines
 * from the IOX expander are also connected each to an LED, also with a
 * limiting resistor.
 *
 * One additional LED is configured on the cpNode (as pin 9), except that I
 * don't use the IOLine library for it.  It serves as a pilot light, as it
 * should always blink when the board is on, even if the IOLine library
 * code contains a bug and does not function.
 *
 *
 * Copyright 2013, 2014 by david d zuhn <zoo@whitepineroute.org>
 *
 * This work is licensed under the Creative Commons Attribution-ShareAlike
 * 4.0 International License. To view a copy of this license, visit
 * http://creativecommons.org/licenses/by-sa/4.0/deed.en_US.
 *
 * You may use this work for any purposes, provided that you make your
 * version available to anyone else.  
 *
 */

#include "Wire.h"
#include "MCP23017.h"
#include "IOLine.h"


// change this to suit whatever address you have defined your IOX board as
#define IOX_ADDRESS 0x20

// 20 output lines, each on an LED for now
IOLine *outputs[] = {
    // Pin values are "normal" Arduino I/O lines, they're in whatever order
    // makes sense for you, instead of what makes sense for the computer
    new IOFlasher(new Pin(A5, OUTPUT), 500),
    new IOFlasher(new Pin(A4, OUTPUT), 650),
    new IOFlasher(new Pin(A3, OUTPUT), 333),
    new IOFlasher(new Pin(A2, OUTPUT), 250),
    new IOFlasher(new Pin(A1, OUTPUT), 490),
    new IOFlasher(new Pin(A0, OUTPUT), 500),
    new IOFlasher(new Pin(13, OUTPUT), 510),
    new IOFlasher(new Pin(12, OUTPUT), 1000),
    new IOFlasher(new Pin(11, OUTPUT), 2000),
    new IOFlasher(new Pin(10, OUTPUT), 5000),

    new Pin(9, OUTPUT),
    new IOFlasher(new Pin(8, OUTPUT), 500, LOW),
    new IOFlasher(new Pin(7, OUTPUT), 500),
    new IOFlasher(new Pin(6, OUTPUT), 500, LOW),
    new IOFlasher(new Pin(5, OUTPUT), 500),
    new IOFlasher(new Pin(4, OUTPUT), 500, LOW),


    // these next are defined in terms of the IOX i2c address (0x20-0x27), port # (0,1), and bit (0-7)
    new IOFlasher(new IOX(IOX_ADDRESS, 0, 3, OUTPUT), 500, HIGH),
    new IOFlasher(new IOX(IOX_ADDRESS, 0, 2, OUTPUT), 500, LOW),
    new IOFlasher(new IOX(IOX_ADDRESS, 0, 1, OUTPUT), 1000, HIGH),
    new IOFlasher(new IOX(IOX_ADDRESS, 0, 0, OUTPUT), 1000, LOW),
    new IOX(IOX_ADDRESS, 1, 7, OUTPUT),
    new IOX(IOX_ADDRESS, 1, 6, OUTPUT),
    new IOFlasher(new IOX(IOX_ADDRESS, 1, 5, OUTPUT), 333),
    new IOFlasher(new IOX(IOX_ADDRESS, 1, 4, OUTPUT), 490),
    new IOFlasher(new IOX(IOX_ADDRESS, 1, 3, OUTPUT), 500),
    new IOFlasher(new IOX(IOX_ADDRESS, 1, 2, OUTPUT), 510)
};

#define outputCount NELEMENTS(outputs)

void setup()
{
    Serial.begin(9600);
    while (!Serial);



    // set the mode on the pilot light
    pinMode(9, OUTPUT);

    // set the mode for each of the IOLine outputs
    for (int i = 0; i < outputCount; i++) {
        outputs[i]->init();
        outputs[i]->digitalWrite(HIGH);
    }
}

int pilot_light_state = 1;


int nextLightToCheck = 0;

void loop()
{
    outputs[nextLightToCheck]->check();
    nextLightToCheck = (nextLightToCheck + 1) % outputCount;
}
