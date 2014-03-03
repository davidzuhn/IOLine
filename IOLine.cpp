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

#include "IOLine.h"
#include "MCP23017.h"

#include "Arduino.h"

////////////////////////////////////////////////////////////////////////////////
//
// Pin implementation

Pin::Pin(uint8_t address, uint8_t mode)
{
    this->address = address;
    this->mode = mode;
}

void Pin::init()
{
    pinMode(address, mode);
}


void Pin::digitalWrite(uint8_t value)
{
    ::digitalWrite(address, value);
}


int Pin::digitalRead()
{
    int rv =::digitalRead(address);

    if (mode == INPUT_INVERTED || mode == INPUT_PULLUP_INVERTED) {
        return !rv;
    }

    return rv;
}

int Pin::analogRead()
{
    int rv =::analogRead(address);
    return rv;
}


////////////////////////////////////////////////////////////////////////////////
//
// IOX Implementation

IOX::IOX(uint8_t address, uint8_t port, uint8_t bit, uint8_t mode)
{
    this->address = address;
    this->port = port;
    this->bit = bit;
    this->mode = mode;
}


void IOX::init()
{
    MCP23017.setMode(address, port, bit, mode);
}


void IOX::digitalWrite(uint8_t value)
{
    MCP23017.setGPIO(address, port, bit, value);
}


int IOX::digitalRead()
{
    return MCP23017.getGPIO(address, port, bit);
}

int IOX::analogRead()
{
    return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// VirtualPin implementation

VirtualPin::VirtualPin(uint8_t mode)
{
    this->mode = mode;
    this->value = 0;
}


void VirtualPin::init()
{
    // does not do anything
}

void VirtualPin::digitalWrite(uint8_t value)
{
    this->value = value;
}


int VirtualPin::digitalRead()
{
    if (mode == INPUT_INVERTED || mode == INPUT_PULLUP_INVERTED) {
        return !this->value;
    }

    return this->value;
}

int VirtualPin::analogRead()
{
    return 0;
}



////////////////////////////////////////////////////////////////////////////////
//
// IOFlasher implementation
//
// an IOFlasher is built atop any other IOLine type, and implements a timed
// flash of the light while it is in the HIGH state.  The amount of time
// given is the interval between state changes (e.g., 500 means the light
// is HIGH for 500ms, then LOW for 500ms, repeating).

IOFlasher::IOFlasher(IOLine * ioLine, unsigned long interval)
{
    this->ioLine = ioLine;
    setInterval(interval);
    currentFlashState = HIGH;
}


IOFlasher::IOFlasher(IOLine * ioLine, unsigned long interval, uint8_t initialState)
{
    this->ioLine = ioLine;
    setInterval(interval);
    currentFlashState = initialState;
}


void IOFlasher::init()
{
    value = LOW;
    previous_millis = millis();
    if (ioLine) {
        ioLine->init();
    }
}


void IOFlasher::setInterval(unsigned long interval)
{
    interval_millis = interval;
}


void IOFlasher::digitalWrite(uint8_t value)
{
    this->value = value;
    update();
}


void IOFlasher::update()
{
    if (ioLine) {
        if (value == HIGH) {
            previous_millis = millis();
            ioLine->digitalWrite(currentFlashState);
        } else {
            ioLine->digitalWrite(LOW);
        }
    }
}


bool IOFlasher::check()
{
    unsigned long now = millis();
    if (now - previous_millis >= interval_millis) {
        previous_millis += interval_millis;

        currentFlashState = (currentFlashState == HIGH) ? LOW : HIGH;
        update();

        return true;
    }

    return false;
}


int IOFlasher::digitalRead()
{
    return 0;
}


int IOFlasher::analogRead()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//
// IOBounce implementation
//
// these methods are meant to be identical to the Bounce library, except
// that they use IOLine objects for reading.

IOBounce::IOBounce()
{
    interval_millis = 10;
}

IOBounce::IOBounce(unsigned long interval)
{
    interval_millis = interval;
}

void IOBounce::attach(IOLine * ioline)
{
    if (ioline != NULL) {
        this->ioline = ioline;

        debouncedState = unstableState = ioline->digitalRead();
        previous_millis = millis();
    }
}


void IOBounce::interval(unsigned long interval_millis)
{
    this->interval_millis = interval_millis;
}


bool IOBounce::update()
{
    uint8_t currentState = false;

    if (ioline) {
        currentState = ioline->digitalRead();
    }

    stateChanged = false;

    if (currentState != unstableState) {
        previous_millis = millis();
    } else if (millis() - previous_millis >= interval_millis) {
        if (currentState != debouncedState) {
            debouncedState = currentState;
            stateChanged = true;
        }
    }

    unstableState = currentState;
    return stateChanged;
}


uint8_t IOBounce::read()
{
    return debouncedState;
}
