/*
 * this sketch reads 10 pushbuttons, via direct Pin & IOX inputs, and
 * changes the state of 10 LEDs.  One push-button is tied to a matching
 * LED.  All button reads reads are done via the debounce interface.
 *
 * The outputs for this sketch are the same IOX outputs used in the IOblink
 * example.
 *
 * The inputs are wired up to a mix of the cpNode pins and the rest of the
 * pins for the IOX that we're not using as outputs.  Munge the list of
 * inputs to match where you've actually wired up input buttons.
 *
 * Each input is expected to be connected to GROUND when triggered.  This
 * example makes use of the INPUT_PULLUP capability of the Arduino and the
 * MCP23017 I/O expander chip in the IOX board.
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
 */

#include <IOLine.h>
#include <Metro.h>

// change this to suit whatever address you have defined your IOX board as
#define IOX_ADDRESS 0x20

IOLine *outputs[] = {
    new IOX(IOX_ADDRESS, 0, 7, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 6, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 5, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 4, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 3, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 2, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 1, OUTPUT),
    new IOX(IOX_ADDRESS, 0, 0, OUTPUT),
    new IOX(IOX_ADDRESS, 1, 7, OUTPUT),
    new IOX(IOX_ADDRESS, 1, 6, OUTPUT)
};



IOLine *inputs[] = {
    new Pin(7, INPUT_PULLUP),
    new Pin(6, INPUT_PULLUP),
    new Pin(5, INPUT_PULLUP),
    new Pin(4, INPUT_PULLUP),

    new IOX(IOX_ADDRESS, 1, 5, INPUT_PULLUP),   // note that these are the rest of the bits on port 1
    new IOX(IOX_ADDRESS, 1, 4, INPUT_PULLUP),   // bits are individually configured as IN or OUT, not a 
    new IOX(IOX_ADDRESS, 1, 3, INPUT_PULLUP),   // whole port at a time
    new IOX(IOX_ADDRESS, 1, 2, INPUT_PULLUP),
    new IOX(IOX_ADDRESS, 1, 1, INPUT_PULLUP),
    new IOX(IOX_ADDRESS, 1, 0, INPUT_PULLUP)
};


#define outputCount (sizeof(outputs)/sizeof(outputs[0]))
#define inputCount (sizeof(inputs)/sizeof(inputs[0]))


// one IObounce per input line
IOBounce bouncers[inputCount];

// we continue to blink an LED on pin 9 to indicate that we're working fine
Metro pilotLight(250);
#define PILOTLIGHT 9

void setup()
{
    // set up the pilot light LED
    pinMode(PILOTLIGHT, OUTPUT);

    // initialize the mode for each of my output lines
    for (int i = 0; i < outputCount; i++) {
        outputs[i]->init();
        outputs[i]->digitalWrite(LOW);
    }

    // now initialize the mode for each of the input lines
    for (int i = 0; i < inputCount; i++) {
        inputs[i]->init();
    }

    // create a debounce object for each of the input lines
    // and connect it to the matching input pin.  
    for (int i = 0; i < inputCount; i++) {
        bouncers[i] = IOBounce();
        bouncers[i].attach(inputs[i]);
    }
}

int pilotLightState = 0;

void loop()
{
    // check each of my input lines for a button press

    // a good change would be to check only a single button during one
    // instance of loop(), and then go on to the next button on the next
    // call to loop().  This reduces the amount of time spent inside the
    // loop, or in any one part of the loop, reducing the chances that 
    // we'l do the timing check (via Metro.check()) well after its full
    // time has elapsed

    for (int i = 0; i < inputCount; i++) {

        // .update() returns true when a change occurs and we only care
        // about when the button is pressed, not when it's released
        // so check for .read() == HIGH

        if (bouncers[i].update() && bouncers[i].read() == HIGH) {
            // I want a latching function, so get the current state of the
            // light and change it, then set the new state

            uint8_t v = outputs[i]->digitalRead();
            v = !v;
            outputs[i]->digitalWrite(v);
        }
    }


    // if .check() is true, the full time of the timer has elapsed and we
    // should do whatever it is that we want to at that event.  In our case,
    // we change the state of the pilot light.

    if (pilotLight.check()) {
        pilotLightState = !pilotLightState;
        digitalWrite(PILOTLIGHT, pilotLightState ? HIGH : LOW);
    }
}
