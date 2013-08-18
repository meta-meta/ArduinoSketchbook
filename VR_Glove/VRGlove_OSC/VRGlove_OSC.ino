#include <MuxShield.h>
#include <OSCBundle.h>
#include <OSCBoards.h>

#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial);
#endif

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
       
   
  //begin SLIPSerial just like Serial
  SLIPSerial.begin(115200);   // set this as high as you can reliably run on your platform
  #if ARDUINO >= 100
    while(!Serial); //Leonardo "feature"
  #endif 
}

void routeVibrate(OSCMessage &msg, int addrOffset)
{
  if(msg.match("/l", addrOffset) || msg.match("/L", addrOffset))
  {
    if(msg.isInt(0) && msg.isInt(1))
    {
      int i = msg.getInt(0);

      if(i > 0 & i <= 15)
      {
        int val = msg.getInt(1);
        if(val<0)
          val = 0;
        if(val>=PWM_VALS)
          val = PWM_VALS;
        leftVibratorsNextVal[i-1] = val;
      }
    }
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
 // Read OSC messages for Vibrators
  OSCBundle bundleIn;
  int size;

  

  if( (size =SLIPSerial.available()) > 0)
  {
     while(size--)
        bundleIn.fill(SLIPSerial.read());

    while(!SLIPSerial.endofPacket())
    {
      if( (size =SLIPSerial.available()) > 0)
      {
         while(size--)
            bundleIn.fill(SLIPSerial.read());
      }
    }
  }

    
  if(!bundleIn.hasError())
  {        
    bundleIn.route("/vibrate", routeVibrate);
  }

  handleVibrators();  
 
  
  
  //declare the bundle
  OSCBundle bundleOut;
  
  for (int i=0; i<5; i++)
  {
    //Analog read on IO1 L1-L5 
    String s = "/bend/L";
    s+=(i+1);
    char charBuf[9]; 
    s.toCharArray(charBuf, 9); 
    bundleOut.add(charBuf).add((int32_t)muxShield.analogReadMS(1,i));      
  }  
  
//  for (int i=0; i<14; i++)
//  {    
//    String s = "/vib/L";
//    s+=(i+1);
//    char charBuf[9]; 
//    s.toCharArray(charBuf, 9); 
//    bundleOut.add(charBuf).add((int32_t)leftVibratorsNextVal[i]);      
//  }  
  
  SLIPSerial.beginPacket();
  bundleOut.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  bundleOut.empty(); // empty the bundle to free room for a new one  
 
}
