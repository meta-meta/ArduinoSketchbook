

#include <MuxShield.h>

MuxShield muxShield;
const int NUM_MUXES = 15;
const int muxesInitialMode[NUM_MUXES] = {
	ANALOG_IN,
	DIGITAL_OUT,
	DIGITAL_OUT,

	DIGITAL_OUT,
	DIGITAL_OUT,
	DIGITAL_OUT,

	DIGITAL_OUT,
	DIGITAL_OUT,
	DIGITAL_OUT,

	DIGITAL_OUT,
	DIGITAL_OUT,
	ANALOG_IN,

	DIGITAL_OUT,
	DIGITAL_OUT,
	DIGITAL_OUT,
};

char inByte;
String command;

int modes[15];

int vals[16] = { 0 };

#include "FastLED.h"
#define LED_PIN 33
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];

unsigned int rotationCount = 0;
unsigned long prevMillis = 0;

void setup()
{
	// set initial mode for each mux
	for (int i = 0; i < NUM_MUXES; i++) {
		muxShield.setMode(i + 1, muxesInitialMode[i]);
		modes[i] = muxesInitialMode[i];
	}

	FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);

	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i].setHSV(i * 255 / 12, 255, 255);
	}
	FastLED.show();

	attachInterrupt(4, onIrSensor, RISING); // pin 19 = interrupt 4. 20 and 21 are interrupts but are high by default  https://forum.arduino.cc/index.php?topic=158434.0

	Serial.begin(115200);
	Serial.println("yo yo");

}

void onIrSensor()
{
	rotationCount++;
}

// 140000101100000100 => set output pins to 0,0,0,0,1,0,1,1,0,0,0,0,0,1,0,0 on Mux 14
void digitalOut(String outputString)
{
	// Mux 00 - 14
	int mux = outputString.substring(0, 2).toInt();
	outputString = outputString.substring(2);

	for (int i = 0; i < 16; i++) {
		vals[i] = outputString[i] != '0' ? HIGH : LOW;
	}

	//MuxShield library is 1 based
	muxShield.digitalWriteMS(mux + 1, vals);
}

// HUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVALHUESATVAL
// 000255255000255255000255255000255255000255255000255255000255255000255255000255255000255255000255255000255255
void led(String ledString)
{	
	for (int i = 0; i < NUM_LEDS; i++)
	{
		leds[i].setHSV(
			ledString.substring(i * 9 + 0, i * 9 + 3).toInt(),
			ledString.substring(i * 9 + 1, i * 9 + 4).toInt(),
			ledString.substring(i * 9 + 2, i * 9 + 5).toInt()
		);
	}
	FastLED.show();
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

void sendBinary(int value)
{
	// send the two bytes that comprise an integer
	Serial.write(lowByte(value));  // send the low byte
	Serial.write(highByte(value)); // send the high byte
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
		  case 'L':
			led(command.substring(1));
			break;
	    }
	
	    command = "";
	  }

	  // prints I14,0,0,3,10,240,1000,0,0,0,12,0,0,4,0,0,0 where 14 is mux14
	  for(int i = 0; i < NUM_MUXES; i++) {
	    if(modes[i] != ANALOG_IN)
	      continue;
	
	   /* Serial.print("I");
	    Serial.print(i);
	    Serial.print(",");
	
	    for(int j = 0; j < 16; j++) {
	      Serial.print(muxShield.analogReadMS(i + 1, j));
	
	      if(j < 15)
	        Serial.print(",");
	    }
	
	    Serial.println("");*/

		// Mux inputs
		Serial.print("B");
		sendBinary(i);
		for (int j = 0; j < 16; j++) {
			sendBinary(muxShield.analogReadMS(i + 1, j));
		}
		Serial.print(";");
	  }

	  // Rotations - IR sensor
	  if (rotationCount > 0) {
		  Serial.print("R");
		  sendBinary(rotationCount);
		  Serial.print(";");
		  rotationCount = 0;
	  }

	//delay(100);
}
