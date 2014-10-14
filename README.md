IOLine
======

Arduino generalized I/O access library.  

IOLine currently supports the Arduino pins, IOX I/O lines via the MCP23017,
or virtual pins.

Any object derived from IOLine supports the digitalRead() and
digitalWrite() methods.  These methods should behave exactly as the Arduino
functions of the same name.

IOGroup takes an array of IOLine objects and performs a digitalWrite
on them.  This is useful for controlling multiple lights from a single
control value.

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

Combinations
-
You can use these in various combinations.   For example, if yuu have a
grade crossing, with each LED of the flashers on a separate line, you 
can use AlternatingFlasher with two IOGroup objects, with each IOGroup
containing the appropriate sets of Pin or IOX objects.  

Not all combinations will make sense, and you may run into timing issues,
especially when using IOX outputs when trying to do animations (the control
time for I2C operations is orders of magnitude longer than for direct GPIO
such as Pin objects use).


Requirements
============

This library uses the MCP23017 library found at https://github.com/davidzuhn/MCP23017

The MCP23017 library uses the Wire module from Arduino, which uses the I2C pins from
your Arduino 

Some of the example programs make use of the Metro library for timing operations.  This is a common
Arduino library and can be found at http://playground.arduino.cc/Code/Metro


Usage
=====

A typical usage might be:


    #include "Wire.h"
    #include "MCP23017.h"
    #include "IOLine.h"

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

