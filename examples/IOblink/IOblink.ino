#include <Wire.h>
#include <MCP23017.h>
#include <IOLine.h>



IOLine *outputs[] = {
    new Pin(A5, OUTPUT),
    new Pin(A4, OUTPUT),
    new Pin(A3, OUTPUT),
    new Pin(A2, OUTPUT),
    new Pin(A1, OUTPUT),
    new Pin(A0, OUTPUT),
    new Pin(13, OUTPUT),
    new Pin(12, OUTPUT),
    new Pin(11, OUTPUT),
    new Pin(10, OUTPUT),
    new IOX(0x21, 0, 7, OUTPUT),
    new IOX(0x21, 0, 6, OUTPUT),
    new IOX(0x21, 0, 5, OUTPUT),
    new IOX(0x21, 0, 4, OUTPUT),
    new IOX(0x21, 0, 3, OUTPUT),
    new IOX(0x21, 0, 2, OUTPUT),
    new IOX(0x21, 0, 1, OUTPUT),
    new IOX(0x21, 0, 0, OUTPUT),
    new IOX(0x21, 1, 7, OUTPUT),
    new IOX(0x21, 1, 6, OUTPUT)
};

#define outputCount (sizeof(outputs)/sizeof(IOLine*))

void setup()
{
    pinMode(9, OUTPUT);

    Serial.begin(9600);

    while (!Serial);

    Serial.print("start with outputCount=");
    Serial.println(outputCount);


}

int blink = 1;

void loop()
{

    digitalWrite(9, blink);

    for (int i = 0; i < outputCount; i++) {
        outputs[i]->digitalWrite(HIGH);
        delay(50);
        outputs[i]->digitalWrite(LOW);
    }

    blink = !blink;

}
