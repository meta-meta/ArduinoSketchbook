#include <MuxShield.h>

MuxShield muxShield;

// milliseconds to pulse the vibrator on or off
const int PWM_BEAT_LENGTH = 25;
const int PWM_BEATS = 12;
const int PWM_CYCLE = PWM_BEAT_LENGTH * PWM_BEATS;
const int PWM_VALS = 11;

// pulse pattern for each of 9 strength levels
const bool PWM[PWM_VALS][PWM_BEATS] = {
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
int beat[14] = {}; //initializes all elements to 0
unsigned long millisAtBeat = 0;

// strength value for each vibrator
int leftVibrators[14];
int leftVibratorsNextVal[14];

String command; // String input from command prompt
char inByte; // Byte input from command prompt

void setup()
{ 
    
  muxShield.setMode(1,ANALOG_IN);
  muxShield.setMode(2,DIGITAL_OUT); 
  muxShield.setMode(3,DIGITAL_OUT);    
 
  for(int i = 0; i < 14; i++)
  {
    muxShield.digitalWriteMS(2,i, LOW );

    leftVibrators[i] = 0;
    leftVibratorsNextVal[i] = 0;   
  }    

  Serial.begin(115200);
       
}

void vibrate(String vibrateString)
{
  if(vibrateString.length() != 28){
    Serial.println("V takes 28 characters of 14 double digit ints. received " + vibrateString.length());
    return;
  }

  for(int i = 0; i < 28; i+=2){   
    int val = vibrateString.substring(i, i+2).toInt();  
    
    if(val<0)
      val = 0;
    if(val>=PWM_VALS)
      val = PWM_VALS;
    leftVibratorsNextVal[i/2] = val;
  }
}

void handleVibrators()
{
  if(millis() - millisAtBeat > PWM_BEAT_LENGTH)
  { 
    millisAtBeat = millis();

    for(int i = 0; i < 14; i++)
    {      
      muxShield.digitalWriteMS(2, i, PWM[leftVibrators[i]][beat[i]] ? HIGH : LOW ); 
      beat[i]++;  

      // if we've reached the end of the vibration pattern, reset
      // or if we've received a new vibrator value, queue it up
      if(beat[i] == PWM_BEATS || leftVibratorsNextVal[i] > 0)
      {
        beat[i] = 0;
        leftVibrators[i] = leftVibratorsNextVal[i];
        leftVibratorsNextVal[i] = 0;  
      } 
    }           
  }  
}

void loop()
{
  // Input serial information:
  if (Serial.available() > 0){
    inByte = Serial.read();
    
    // only input if a letter, number, =,?,+ are typed!
    if ((inByte >= 65 && inByte <= 90) || (inByte >=97 && inByte <=122) || (inByte >= 48 && inByte <=57) || inByte == 43 || inByte == 61 || inByte == 63) {
      command.concat(inByte);
      }
  }

  // Process command when NL/CR are entered:
  if (inByte == 10 || inByte == 13){
    inByte = 0;
               
    if(command.startsWith("V")){
      vibrate(command.substring(1));
    }

    command = "";   
  }   
 
  handleVibrators();  
 
  Serial.print("B");
  for (int i=0; i<5; i++)
  {
    Serial.print(muxShield.analogReadMS(1,i));
    if(i < 4)
    {
      Serial.print(",");
    }       
  }
  Serial.println("");  
}
