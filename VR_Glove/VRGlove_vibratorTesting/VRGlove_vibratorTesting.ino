#include <MuxShield.h>
MuxShield muxShield;

// milliseconds to pulse the vibrator on or off
const int PWM_BEAT_LENGTH = 25;
const int PWM_BEATS = 12;
const int PWM_CYCLE = PWM_BEAT_LENGTH * PWM_BEATS;

// pulse pattern for each of 9 strength levels
const bool PWM[11][12] = {
  {0,0,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,0,0,0,0,0,0},
  {1,0,0,0,0,0,1,0,0,0,0,0},
  {1,0,0,0,1,0,0,0,1,0,0,0},
  {1,0,0,1,0,0,1,0,0,1,0,0},
  {1,0,1,0,1,0,1,0,1,0,1,0},
  {1,1,0,1,1,0,1,1,0,1,1,0},
  {1,1,1,0,1,1,1,0,1,1,1,0},
  {1,1,1,1,1,0,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,0},
  {1,1,1,1,1,1,1,1,1,1,1,1}
};

// the beat we are on
int beat = 0; 
unsigned long millisAtBeat = 0;

// strength value for each vibrator
int leftVibrators[14];
int leftVibratorsNextVal[14];



const int PWM_CYCLES_PER_TEST_BEAT = 1;
const int TEST_BEATS = 24;
const int TEST_PROGRAM[14][TEST_BEATS] = {
  {0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,0},
  {0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,0},
  {0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,0},
  {0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,0},
  {0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,10,0,11,0,0},
  
  {11,0,10,0,9,0,8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0,0},
  {11,0,10,0,9,0,8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0,0},
  {11,0,10,0,9,0,8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0,0},
  {11,0,10,0,9,0,8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0,0},
  {11,0,10,0,9,0,8,0,7,0,6,0,5,0,4,0,3,0,2,0,1,0,0},

  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}

};

int testPwmCycle = 0;
unsigned long millisAtTestPwmCycle = 0;


void setup()
{       
    muxShield.setMode(2,DIGITAL_OUT);

    // initialize leftVibrators to 0
    for(int i = 0; i < 14; i++)
    {
      leftVibrators[i] = 0;
      leftVibratorsNextVal[i] = 0;
    }
    
    Serial.begin(9600);
}

void handleVibrators()
{
  if(millis() - millisAtBeat > PWM_BEAT_LENGTH)
  { 
    millisAtBeat = millis();

    for(int i = 0; i < 14; i++)
    {      
      muxShield.digitalWriteMS(2, i, PWM[leftVibrators[i]][beat] ? HIGH : LOW );        
    }   
    
    beat++;
    if(beat == PWM_BEATS)
    {
      beat = 0;
      for(int i = 0; i < 14; i++)
      {            
          leftVibrators[i] = leftVibratorsNextVal[i];
          leftVibratorsNextVal[i] = 0;                  
      }  
    }    
  }  
}


void loop()
{ 

  if(millis() - millisAtTestPwmCycle > PWM_CYCLE)
  {
    millisAtTestPwmCycle = millis();

    for(int i = 0; i < 14; i++)
    {
      leftVibratorsNextVal[i] = TEST_PROGRAM[i][testPwmCycle/PWM_CYCLES_PER_TEST_BEAT];
    }  

    testPwmCycle++;
    if(testPwmCycle == PWM_CYCLES_PER_TEST_BEAT * TEST_BEATS)
    {
      testPwmCycle = 0;
    }      
  }
  
 

  handleVibrators();
}
