/*
 * IOLine implements a generic interface to multiple types of I/O lines
 *
 * Currently implemented I/O line types include Arduino pins (D0-D13,
 * A0-A5, etc), MCP23017 based IOX lines (attached via I2C interface),
 * and virtual lines that have no electronic interface but have a local 
 * state maintained in software.
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

#ifndef __ioline_h
#define __ioline_h

#include "Arduino.h"

// occasionally check to see if Arduino defines any other pin modes
#define INPUT_INVERTED        0x3
#define INPUT_PULLUP_INVERTED 0x4

#define NELEMENTS(_ar) (sizeof(_ar)/sizeof(_ar[0]))

class IOLine {
  public:
    virtual void digitalWrite(uint8_t value) = 0;
    virtual int digitalRead() = 0;
    virtual int analogRead() = 0;
    virtual void init() { };
};

class Pin:public IOLine {
  public:
    Pin(uint8_t address, uint8_t mode);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

  private:
    uint8_t mode;
    uint8_t address;
};


class IOX:public IOLine {
  public:
    IOX(uint8_t address, uint8_t port, uint8_t bit, uint8_t mode);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

  private:
    uint8_t address;
    uint8_t port;
    uint8_t bit;
    uint8_t mode;
};



class VirtualPin:public IOLine {
  public:
    VirtualPin(uint8_t mode);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

  private:
    uint8_t mode;
    uint8_t value;
};



class IOBounce {
  public:
    IOBounce();
    IOBounce(unsigned long interval);

    void attach(IOLine * ioline);
    void interval(unsigned long interval_millis);
    bool update();
    uint8_t read();

  protected:
    int debounce();
    unsigned long previous_millis, interval_millis;
    uint8_t debouncedState;
    uint8_t unstableState;
    uint8_t stateChanged;

    IOLine *ioline;
};



#endif
