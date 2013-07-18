//This example shows how to use the Mux Shield for analog inputs

#include <MuxShield.h>

//Initialize the Mux Shield
MuxShield muxShield;

void setup()
{       
    muxShield.setMode(1,ANALOG_IN);
    Serial.begin(9600);
}

//Arrays to store analog values after recieving them
int IO1AnalogVals[5];


void loop()
{
  for (int i=0; i<5; i++)
  {
    //Analog read on IO1
    IO1AnalogVals[i] = muxShield.analogReadMS(1,i);   
  }
  
  //Print IO1 values for inspection
  Serial.print("IO1 analog values: ");
  for (int i=0; i<5; i++)
  {
    if(IO1AnalogVals[i] > 700)
    {
      Serial.print(" ");      
    }
    else if(IO1AnalogVals[i] > 600)
    {
      Serial.print(".");  
    }
     else if(IO1AnalogVals[i] > 500)
    {
      Serial.print("o");  
    }
     else if(IO1AnalogVals[i] > 400)
    {
      Serial.print("O");  
    }
     else if(IO1AnalogVals[i] > 300)
    {
      Serial.print("0");  
    }
     else if(IO1AnalogVals[i] > 200)
    {
      Serial.print("*");  
    }
     else if(IO1AnalogVals[i] > 100)
    {
      Serial.print("@");  
    }
    
    Serial.print("_");
  }
  Serial.println();
}

