/*
 MuxShield.cpp - Library for using Mayhew Labs' Mux Shield.
 Created by Mark Mayhew, December 29, 2012.
 Released into the public domain.
 */

#include "Arduino.h"
#include "MuxShield.h"

int _shiftReg[15][16] = {0};

// Default pins
int MuxShield::_S[4] = {
        2,
        4,
        6,
        7
    };

// Arduino Mega IO pins for software selecting Mux Mode in or out
// these could all be eliminated if you want to do the solder jumper thing (see MuxShield manual)        
int MuxShield::_IOS[15] = {
        35,
        36,
        37,

        38,
        39,
        40,

        41,
        42,    
        43,

        // 44 - 46 are PWM, so save them for something else
        //  pin 11 is also PWM so let's not use that for digital writes

        48,
        49,
        50,

        51,
        52,
        53
    };

// These are used to read and write to each mux
int MuxShield::_IO[15] = {
    // I messed up the wiring so not in order (faster to change code than wires)

        // Mux 1
        A12,
        A13,
        A14,

        // Mux 2
        A9,
        A10,
        A11,

        // Mux 3
        A6,
        A7,
        A8,

        // Mux 4
        A3,
        A4,
        A5,

        // Mux 5
        A0,
        A1,
        A2
    };

int MuxShield::_OUTMD = 8;

MuxShield::MuxShield()
{
    for(int i = 0; i < 4; i++) {
        pinMode(_S[i], OUTPUT);
    }

    pinMode(_OUTMD,OUTPUT);
    digitalWrite(_OUTMD,LOW);
    
    for(int i = 0; i < 15; i++) {
        pinMode(_IOS[i], OUTPUT);
    }
}

void MuxShield::setMode(int mux, int mode) 
{
    int i = mux - 1;

    switch (mode) {
        case DIGITAL_IN:
            pinMode(_IO[i],INPUT);
            digitalWrite(_IOS[i],LOW);
            break;
        case DIGITAL_IN_PULLUP:
            pinMode(_IO[i],INPUT_PULLUP);
            digitalWrite(_IOS[i],LOW);
            break;
        case DIGITAL_OUT:
            pinMode(_IO[i],OUTPUT);
            digitalWrite(_IOS[i],HIGH);
            break;
        case ANALOG_IN:
            digitalWrite(_IOS[i],LOW);
            break;
        default:
            break;
    }
}

// <1 milliseconds to write to 16 pins
void MuxShield::digitalWriteMS(int mux, int vals[])
{
    int i = mux - 1;
    int sIdx = i % 3;
    
    digitalWrite(_S[3],LOW);                              //S3 here is LCLK
    digitalWrite(_OUTMD,HIGH);                          //set to output mode


    for (int j = 15; j >= 0; j--) {
        digitalWrite(_S[sIdx], LOW);                  //S0 here is i/o1 _sclk
        
        _shiftReg[i][j] = vals[j];              //store value until updated again

        digitalWrite(_IO[i], _shiftReg[i][j]);       //put value
        digitalWrite(_S[sIdx], HIGH);                 //lactch in value
    }

    
    digitalWrite(_S[3],HIGH);                     //latch in ALL values
    digitalWrite(_OUTMD,LOW);                   //Exit output mode
}

// 6 milliseconds to write to 16 pins
void MuxShield::digitalWriteMS(int mux, int chan, int val)
{
    int i = mux - 1;
    int sIdx = i % 3;
    
    digitalWrite(_S[3],LOW);                              //S3 here is LCLK
    digitalWrite(_OUTMD,HIGH);                          //set to output mode
    

    _shiftReg[i][chan] = val;              //store value until updated again

    for (int j = 15; j >= 0; j--) {
        digitalWrite(_S[sIdx], LOW);                  //S0 here is i/o1 _sclk
        digitalWrite(_IO[i], _shiftReg[i][j]);       //put value
        digitalWrite(_S[sIdx], HIGH);                 //lactch in value
    }

    
    digitalWrite(_S[3],HIGH);                     //latch in ALL values
    digitalWrite(_OUTMD,LOW);                   //Exit output mode
}

int MuxShield::digitalReadMS(int mux, int chan)
{
    digitalWrite(_OUTMD,LOW);   //Set outmode off (i.e. set as input mode)
    
    digitalWrite(_S[0], (chan&1));    
    digitalWrite(_S[1], (chan&3)>>1); 
    digitalWrite(_S[2], (chan&7)>>2); 
    digitalWrite(_S[3], (chan&15)>>3); 

    return digitalRead(_IO[mux - 1]); 
}

int MuxShield::analogReadMS(int mux, int chan)
{
    digitalWrite(_OUTMD,LOW);

    digitalWrite(_S[0], (chan&1));    
    digitalWrite(_S[1], (chan&3)>>1); 
    digitalWrite(_S[2], (chan&7)>>2); 
    digitalWrite(_S[3], (chan&15)>>3); 
    
    return analogRead(_IO[mux - 1]); 
}