/* -*- c++ -*-
 * IOLine implements a generic interface to multiple types of I/O lines
 *
 * Currently implemented I/O line types include Arduino pins (D0-D13,
 * A0-A5, etc), MCP23017 based IOX lines (attached via I2C interface),
 * and virtual lines that have no electronic interface but have a local 
 * state maintained in software.
 *
 * Several "effect" IOLines are also defined:  
 *   IOFlasher will flash a specified IOLine at a given interval
 *   AlternatingFlasher will switch two lines at a specifie interval
 *   IOChase will run a chase light along a list of IOLines
 *
 * These effect IOLines take "real" IOLines as configured parameters
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

#ifndef IOLINE_NO_MCP23017
#include "MCP23017.h"
#endif

#include "Arduino.h"


// occasionally check to see if Arduino defines any other pin modes
#define INPUT_INVERTED        0x3
#define INPUT_PULLUP_INVERTED 0x4
#define OUTPUT_INVERTED       0x5

#define NELEMENTS(_ar) (sizeof(_ar)/sizeof(_ar[0]))

class IOLine {
  public:
    virtual void digitalWrite(uint8_t value) = 0;
    virtual int digitalRead() = 0;
    virtual int analogRead() = 0;
    virtual void init() {
    };
    virtual bool check() {
        return false;
    };
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


class IOFlasher:public IOLine {
  public:
    IOFlasher(IOLine * ioLine, unsigned long interval);
     IOFlasher(IOLine * ioLine, unsigned long interval, uint8_t initialState);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

    bool check();
    void setInterval(unsigned long interval);

  private:
    void update();

    IOLine *ioLine;
    uint8_t value;
    uint8_t currentFlashState;
    unsigned long previous_millis, interval_millis;
};


class AlternatingFlasher:public IOLine {
  public:
    AlternatingFlasher(IOLine * io1, unsigned long interval1, IOLine * io2,
                       unsigned long interval2);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

    bool check();
    void setInterval1(unsigned long interval);
    void setInterval2(unsigned long interval);


  private:
    void update();

    IOLine *ioLine1;
    IOLine *ioLine2;
    unsigned long interval1;
    unsigned long interval2;
    uint8_t value;
    uint8_t currentFlashIOLine;
    unsigned long previous_millis;
    unsigned long currentInterval;
};


class IOBounce {
  public:
    IOBounce();
    IOBounce(unsigned long interval);

    void attach(IOLine * ioline);
    void interval(unsigned long interval_millis);
    unsigned long getChangeTime();
    bool update();
    uint8_t read();

  protected:
    int debounce();
    unsigned long previous_millis, interval_millis;
    uint8_t debouncedState;
    uint8_t unstableState;
    uint8_t stateChanged;
    unsigned long changeTime;

    IOLine *ioline;
};


class IOChase:public IOLine {
  public:
    IOChase(IOLine ** iolines, unsigned int ioCount, unsigned long interval);
    IOChase(IOLine ** iolines, unsigned int ioCount, unsigned long interval, unsigned int maxtail);

    void digitalWrite(uint8_t value);
    int digitalRead();
    int analogRead();
    void init();

    bool check();
    void setInterval(unsigned long interval);


  private:
    void update();

    IOLine **outputs;
    unsigned int outputCount;

    unsigned int head;
    unsigned int tail;
    unsigned int maxtail;

    unsigned long interval_millis;
    uint8_t value;
    unsigned int nextLight;
    unsigned long previous_millis;
};



class IOGroup:public IOLine {
public:
    IOGroup(IOLine **iolines, unsigned int ioCount);

    void digitalWrite(uint8_t value);
    int digitalRead() { return 0; };
    int analogRead() { return 0; }
    void init();

    bool check();

private:
    IOLine **ioLines;
    unsigned int ioCount;
};


#endif
