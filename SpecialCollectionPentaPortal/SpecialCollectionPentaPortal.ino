#include <color.h>

// 2 rgb leds to these pins
int led[] = {3,5,6,9,10,11}; 

Color cur_color = Color(1,1,1);
float hue = 0;
float sat = 0;
float satV = 0.0;

float lum = 0;
float lumV = 0.0;

void setup()  { 
  for(int i = 0 ; i < 6; i++ ){
    pinMode(led[i], OUTPUT); 
  } 
} 

void loop() // run over and over again                   
{
 
  rainbow();
}

void rainbow(){
  hue += 0.003;
  if ( hue >=1 ) hue = 0;
  
  sat += satV;
  if(sat >=1) satV = -0.002;
  if(sat <=0) satV = 0.002;
  
  lum += lumV;
  if(lum >=0.5) lumV = -0.005;
  if(lum <=0.01) lumV = 0.005;
  
  
  cur_color.convert_hcl_to_rgb(hue,sat,lum);
  display_color(cur_color, led[0], led[1], led[2]);
  
  float h = hue + 0.5;
  if(h >= 1) h -= 1;
  
  cur_color.convert_hcl_to_rgb(h,sat,lum);
  display_color(cur_color, led[3], led[4], led[5]);
  
  //delay(20);
}

void display_color(Color c, int r, int g, int b){
  analogWrite(r, c.red);
  analogWrite(g, c.green);
  analogWrite(b, c.blue);
  delay(10);
}



