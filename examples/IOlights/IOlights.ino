// treat this as -*- c++ -*-

#include <Wire.h>
#include <Metro.h>
#include <MCP23017.h>
#include <IOLine.h>


/*
 * this sketch reads 10 pushbuttons, via direct Pin & IOX inputs,
 * and changes the state of 10 LEDs.  One push-button is tied
 * to the matching LED.  All reads are done via the debounce 
 * interface.
 *
 */


IOLine *outputs[] = {
    new IOX(0x24, 0, 4, OUTPUT),
    new IOX(0x24, 0, 3, OUTPUT),
    new IOX(0x24, 0, 2, OUTPUT),
    new IOX(0x24, 0, 1, OUTPUT),
    new IOX(0x24, 0, 0, OUTPUT),

    new IOX(0x24, 1, 7, OUTPUT),
    new IOX(0x24, 1, 6, OUTPUT),
    new IOX(0x24, 1, 5, OUTPUT),
    new IOX(0x24, 1, 4, OUTPUT),
    new IOX(0x24, 1, 3, OUTPUT)
};

#define outputCount (sizeof(outputs)/sizeof(IOLine*))


IOLine *inputs[] = {
    new Pin(7, INPUT_PULLUP),
    new Pin(6, INPUT_PULLUP),
    new Pin(5, INPUT_PULLUP),
    new Pin(4, INPUT_PULLUP),
    new IOX(0x21, 1, 5, INPUT_PULLUP),
    new IOX(0x21, 1, 4, INPUT_PULLUP),
    new IOX(0x21, 1, 3, INPUT_PULLUP),
    new IOX(0x21, 1, 2, INPUT_PULLUP),
    new IOX(0x21, 1, 1, INPUT_PULLUP),
    new IOX(0x21, 1, 0, INPUT_PULLUP)
};

#define inputCount (sizeof(inputs)/sizeof(IOLine*))

IOBounce bouncers[inputCount];

Metro pilotLight(250);

void setup()
{
    // put your setup code here, to run once:

    pinMode(A5, OUTPUT);


    Serial.begin(9600);
    while (!Serial);

    delay(3000);

    for (int i = 0; i < outputCount; i++) {
        outputs[i]->init();
        outputs[i]->digitalWrite(LOW);
    }

    for (int i = 0; i < inputCount; i++) {
        inputs[i]->init();
    }

    for (int i = 0; i < inputCount; i++) {
        bouncers[i] = IOBounce();
        bouncers[i].attach(inputs[i]);
        Serial.print("iobounce ");
        Serial.print(i);
        Serial.println(" initialized");
    }
}

int blink = 1;

void loop()
{

    digitalWrite(A5, blink);

    for (int i = 0; i < inputCount; i++) {
        if (bouncers[i].update() && bouncers[i].read()) {

            uint8_t v = outputs[i]->digitalRead();
            outputs[i]->digitalWrite(!v);
        }
    }

    if (pilotLight.check()) {
        blink = !blink;
    }
}
