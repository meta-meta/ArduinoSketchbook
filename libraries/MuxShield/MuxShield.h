/*
 MuxShield.h - Library for using Mayhew Labs' Mux Shield.
 Created by Mark Mayhew, December 20, 2012.
 Released into the public domain.
 */
#ifndef MuxShield_h
#define MuxShield_h

#include "Arduino.h"

#define DIGITAL_IN   0
#define DIGITAL_OUT  1
#define ANALOG_IN    2
#define DIGITAL_IN_PULLUP    3


class MuxShield
{
public:
    MuxShield();
    void setMode(int mux, int mode);
    void digitalWriteMS(int mux, int chan, int val);
    void digitalWriteMS(int mux, int vals[16]);
    int digitalReadMS(int mux, int chan);
    int analogReadMS(int mux, int chan);
    
private:
    static int _OUTMD;
    static int _S[4];
    static int _IOS[15];
    static int _IO[15];
};

#endif
