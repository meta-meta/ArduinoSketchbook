//This example shows how to use the Mux Shield for analog inputs

#include <MuxShield.h>

#define B 48
#define REST 100

//Initialize the Mux Shield
MuxShield muxShield;

//Arrays to store analog values after recieving them
int IO1AnalogVals[5];

int beats=0;

int leftVibrators[14][B];

int beat = 0;


void setup()
{   
    muxShield.setMode(1,ANALOG_IN);
    muxShield.setMode(2,DIGITAL_OUT);
    
    for(int r = 0; r < 10; r++)
    {
      for(int i = 0; i < 14; i++)
      {
        muxShield.digitalWriteMS(2,i, HIGH );
        delay((r%10)*10- (r%5)*9);
        muxShield.digitalWriteMS(2,i, LOW );
       
        muxShield.digitalWriteMS(2,i, leftVibrators[i][beat] ); 
        
      }     
    }
    
    for(int i = 0; i < 14; i++)
      {
         
        for(int j = 0; j < B; j++)
        {
          leftVibrators[i][j] = LOW;     
        }
      }     
  

    Serial.begin(9600);
}



void loop()
{
  
  if(millis()%REST>REST-5)
  {
    for(int i = 0; i < 14; i++)
    {
      muxShield.digitalWriteMS(2,i, LOW );      
    }    
  }
  
  if(millis()%REST<REST/5)
  { 
    
    beat++;
    if(beat > B-1)
    {
      beat = 0;
    }
    
    
    for(int i = 0; i < 14; i++)
    {
      muxShield.digitalWriteMS(2,i, leftVibrators[i][beat] );      
    }
       
    
    if(beat == 0 || beat == 4)
    {
      Serial.print(beat);
      Serial.println("+++++++++++++++++++++++++");
      
    }
    else
    {
      Serial.print(beat);
      Serial.println("------------------------");
    }
    
    for (int i=0; i<5; i++)
    {
      //Analog read on IO1
      IO1AnalogVals[i] = muxShield.analogReadMS(1,i); 
      Serial.print(IO1AnalogVals[i]);
      Serial.print("  ");
      
      if(IO1AnalogVals[i] > 250)
      {
        leftVibrators[i][beat] = LOW;
        leftVibrators[i+5][beat] = LOW;
      }
      else if(IO1AnalogVals[i] > 200)
      {
        leftVibrators[i][beat] = HIGH;
        muxShield.digitalWriteMS(2,i, HIGH );
      }
      else
      {
        leftVibrators[i][beat] = HIGH;
        muxShield.digitalWriteMS(2,i, HIGH );
        leftVibrators[i+5][beat] = HIGH;
        muxShield.digitalWriteMS(2,i+5, HIGH );        
      }  
      
      
    }
    
    
  }
    

}

