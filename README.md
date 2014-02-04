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


Requirements
============

This library requires the MCP23017 library found at https://github.com/davidzuhn/MCP23017

The example programs make use of the Metro library for timing operations.  This is a common
Arduino library and can be found at http://playground.arduino.cc/Code/Metro


Usage
=====

A typical usage might be:

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

    // call ->init() on each IOLine object

    // now use them

    outputs[0]->digitalWrite(HIGH);
    int val = inputs[1]->digitalRead();

