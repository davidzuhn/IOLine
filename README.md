IOLine
======

Arduino generalized I/O access library.  

IOLine currently supports the Arduino pins, IOX I/O lines via the MCP23017,
or virtual pins.

Any object derived from IOLine supports the digitalRead() and
digitalWrite() methods.  These methods should behave exactly as the Arduino
functions of the same name.

An IOBounce interface is provided to perform debounce operations using an
IOLine object.


Effects
-
Various IOLine classes have been defined which perform timing based effects
on other IOLine objects.

IOFlasher will flash a line at a specified interval

AlternatingFlasher will switch between two IOLines at specified intervals

IOChase takes a list of IOLines and runs a chase light effect across all
of them


Requirements
============

This library can make use of the MCP23017 library found at https://github.com/davidzuhn/MCP23017

    #define IOLINE_USES_IOX 1
    #include <IOLine.h>

will enable the IOX class support, which uses the Arduino Wire library and
the i2c communications pins on your Arduino board.

The example programs make use of the Metro library for timing operations.  This is a common
Arduino library and can be found at http://playground.arduino.cc/Code/Metro


Usage
=====

A typical usage might be:

    #define IOLINE_USES_IOX 1
    #include <IOLine.h>

    IOLine *outputs = {
      new Pin(4, OUTPUT),
      new IOX(0x20, 0, 1, OUTPUT),
      new VirtualPin (OUTPUT)
    };


    IOLine *inputs = {
      new Pin(5, INPUT),
      new IOX(0x21, 1, 7, INPUT_PULLUP_INVERTED),
      new IOX(0x21, 0, 1, INPUT_PULLUP)
    };

    void setup() {
      // call ->init() on each IOLine object
	}


	void loop() {
      // now use them

      outputs[0]->digitalWrite(HIGH);
      int val = inputs[1]->digitalRead();
    }

