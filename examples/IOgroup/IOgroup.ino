/*
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

#define IOLINE_USES_IOX 0
#include <IOLine.h>


// Use all of the Arduino pins except for the serial console lines (0, 1)

IOLine *output1[] = {
    new Pin(2, OUTPUT),
    new Pin(3, OUTPUT),
    new Pin(4, OUTPUT),
    new Pin(5, OUTPUT),
    new Pin(6, OUTPUT),
    new Pin(7, OUTPUT),
    new Pin(8, OUTPUT),
    new Pin(9, OUTPUT)
};

IOLine *output2[] = {
    new Pin(10, OUTPUT),
    new Pin(11, OUTPUT),
    new Pin(12, OUTPUT),
    new Pin(13, OUTPUT),
    new Pin(A0, OUTPUT),
    new Pin(A1, OUTPUT),
    new Pin(A2, OUTPUT),
    new Pin(A3, OUTPUT),
    new Pin(A4, OUTPUT),
    new Pin(A5, OUTPUT)
};

#define outputCount1 NELEMENTS(output1)
#define outputCount2 NELEMENTS(output2)

IOGroup *group1 = NULL;
IOGroup *group2 = NULL;
AlternatingFlasher *flasher;

void setup()
{
    group1 = new IOGroup(output1, outputCount1);
    group2 = new IOGroup(output2, outputCount2);
    if (group1 != NULL && group2 != NULL) {
	flasher = new AlternatingFlasher(group1, 500, group2, 500);
        if (flasher) {
            flasher->init();
            flasher->digitalWrite(HIGH);
        }
    }
}

void loop()
{
    if (flasher != NULL) {
      flasher->check();
    }
}
