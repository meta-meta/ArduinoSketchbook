#include <MuxShield.h>
MuxShield muxShield;

#include <OSCBundle.h>
#include <OSCBoards.h>
#ifdef BOARD_HAS_USB_SERIAL
#include <SLIPEncodedUSBSerial.h>
SLIPEncodedUSBSerial SLIPSerial( thisBoardsSerialUSB );
#else
#include <SLIPEncodedSerial.h>
 SLIPEncodedSerial SLIPSerial(Serial);
#endif



void setup()
{ 
    
  muxShield.setMode(1,ANALOG_IN);
  muxShield.setMode(2,DIGITAL_OUT); 
  muxShield.setMode(3,DIGITAL_OUT);    
 
   for(int i = 0; i < 14; i++)
  {
    muxShield.digitalWriteMS(2,i, LOW );
   
  }      
       
   
  //begin SLIPSerial just like Serial
  SLIPSerial.begin(115200);   // set this as high as you can reliably run on your platform
  #if ARDUINO >= 100
    while(!Serial); //Leonardo "feature"
  #endif 
}

void loop()
{  
  //declare the bundle
  OSCBundle bndl;
  //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
  for (int i=0; i<5; i++)
  {
    //Analog read on IO1 L1-L5 
    String s = "/analog/L";
    s+=(i+1);
    char charBuf[11]; 
   s.toCharArray(charBuf, 11); 
    bndl.add(charBuf).add((int32_t)muxShield.analogReadMS(1,i));      
  }  

  SLIPSerial.beginPacket();
      bndl.send(SLIPSerial); // send the bytes to the SLIP stream
  SLIPSerial.endPacket(); // mark the end of the OSC Packet
  bndl.empty(); // empty the bundle to free room for a new one

  //delay(4);
}

