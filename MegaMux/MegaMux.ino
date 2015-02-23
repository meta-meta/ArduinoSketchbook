#include <MuxShield.h>

MuxShield muxShield;
const int NUM_MUXES = 15;

char inByte;
String command;

int modes[15];

int vals[16] = {0};

void setup()
{ 
  for(int i = 0; i < NUM_MUXES; i++) {
    muxShield.setMode(i + 1, ANALOG_IN);
    modes[i] = ANALOG_IN;
  }

  Serial.begin(115200);
}

// 140000101100000100 => set output pins to 0,0,0,0,1,0,1,1,0,0,0,0,0,1,0,0 on Mux 14 
void digitalOut(String outputString)
{
  // Mux 00 - 14
  int mux = outputString.substring(0, 2).toInt();
  outputString = outputString.substring(2);

  for(int i = 0; i < 16; i++) {
    vals[i] = outputString[i] != '0' ? HIGH : LOW;
  }

  //MuxShield library is 1 based
  muxShield.digitalWriteMS(mux + 1, vals); 

}

// 140 => set Mux 14 to DIGITAL_IN mode
void setMode(String modeString)
{
  // Mux 00 - 14
  int mux = modeString.substring(0, 2).toInt();
  modeString = modeString.substring(2);

  int mode = modeString.toInt();
  modes[mux] = mode;

  muxShield.setMode(mux + 1, mode);
}

void loop()
{
  // Input serial information:
  while (Serial.available() > 0){
    inByte = Serial.read();
    
    // only input if a letter, number, =,?,+ are typed!
    if ((inByte >= 65 && inByte <= 90) || (inByte >=97 && inByte <=122) || (inByte >= 48 && inByte <=57) || inByte == 43 || inByte == 61 || inByte == 63) {
      command.concat(inByte);
    }
  }

  // Process command when NL/CR are entered:
  if (inByte == 10 || inByte == 13){
    inByte = 0;
    
    switch(command[0]) {
      case 'M':
        setMode(command.substring(1));
        break;
      case 'O':
        digitalOut(command.substring(1));
        break;
    }

    command = "";   
  }   
 
  // prints I14,0,0,3,10,240,1000,0,0,0,12,0,0,4,0,0,0 where 14 is mux14
  for(int i = 0; i < NUM_MUXES; i++) {
    if(modes[i] != ANALOG_IN)
      continue;

    Serial.print("I");
    Serial.print(i);
    Serial.print(",");

    for(int j = 0; j < 16; j++) {
      Serial.print(muxShield.analogReadMS(i + 1, j));

      if(j < 15)
        Serial.print(",");
    }

    Serial.println("");  
  }
  
  delay(18);

}
