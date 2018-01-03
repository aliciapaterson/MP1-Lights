
#include "config.h"
#include <ESP8266WiFi.h>

#define RED1_PIN   13
#define GREEN1_PIN 12
#define BLUE1_PIN  14

#define RED2_PIN   2
#define GREEN2_PIN 16
#define BLUE2_PIN  15

static int maxBrightness=254;

int strip1Red = 0;
int strip1Green = 0;
int strip1Blue = 0;

int strip2Red = 0;
int strip2Green = 0;
int strip2Blue = 0;

int timer = 0;
bool strip1Flashing = false;
int strip1fadeamt = 5;
int strip1Interval = 1000;
int colorLoop = 0;
int rgbSelect = 0;

// set up the 'color' feed
AdafruitIO_Feed *color1 = io.feed("mp1.color1");
AdafruitIO_Feed *color2 = io.feed("mp1.color2");
AdafruitIO_Feed *flash1 = io.feed("mp1.flash1");

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'color' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  color1->onMessage(strip1Color);
  color2->onMessage(strip2Color);
  flash1->onMessage(strip1Flash);
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // set analogWrite range for ESP8266
  #ifdef ESP8266
    analogWriteRange(255);
  #endif

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  timer = millis();

  if (strip1Flashing) {
     //loop through color values
     if (colorLoop>255) {
      //swap colors to increment
      rgbSelect ++;
      if (rgbSelect>3) { rgbSelect = 0; }
      colorLoop = 0;
     } 
     //change color values
     if (rgbSelect == 0) {
      //set red and green
      strip1Red -=1;
      strip2Red -=1;
      strip1Green +=1;
      strip2Green +=1;
     } else if (rgbSelect == 1){
       //set green and blue
       strip1Green -=1;
       strip2Green -=1;
       strip1Blue +=1;
       strip2Blue +=1;
     } else {
       //set blue and red
       strip1Blue -=1;
       strip2Blue -=1;
       strip1Red +=1;
       strip2Red +=1;
     }
     colorLoop+=strip1fadeamt;
  }
  setColour1(strip1Red, strip1Green, strip1Blue);
  setColour2(strip2Red, strip2Green, strip2Blue);  
}

// this function is called whenever a 'color' message
// is received from Adafruit IO. it was attached to
// the color feed in the setup() function above.
void strip1Color(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received:");  
  Serial.print("  - R: ");
  Serial.println(data->toRed());
  Serial.print("  - G: ");
  Serial.println(data->toGreen());
  Serial.print("  - B: ");
  Serial.println(data->toBlue());
  Serial.print("  - HEX: ");
  Serial.println(data->value());

  strip1Red   = data->toRed();
  strip1Green = data->toGreen();
  strip1Blue  = data->toBlue();
}

void strip2Color(AdafruitIO_Data *data) {

  // print RGB values and hex value
  Serial.println("Received:");  
  Serial.print("  - R: ");
  Serial.println(data->toRed());
  Serial.print("  - G: ");
  Serial.println(data->toGreen());
  Serial.print("  - B: ");
  Serial.println(data->toBlue());
  Serial.print("  - HEX: ");
  Serial.println(data->value());

  strip2Red   = data->toRed();
  strip2Green = data->toGreen();
  strip2Blue  = data->toBlue();
}

void strip1Flash(AdafruitIO_Data *data) {
  Serial.print("Recieved:");
  Serial.println(data->value());

  if (data->toPinLevel()==HIGH) {
    strip1Red = 255;
    strip2Red = 255;
    strip1Green = 0;
    strip2Green = 0;
    strip1Blue = 0;
    strip2Blue = 0;
    strip1Flashing = true;
    Serial.println("Strip1 flash");
  } else {
    strip1Flashing = false;
    Serial.println("Strip1 stop flash");
  }
}

void setColour1(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(RED1_PIN, red);
  analogWrite(GREEN1_PIN, green);
  analogWrite(BLUE1_PIN, blue);
 }

void setColour2(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(RED2_PIN, red);
  analogWrite(GREEN2_PIN, green);
  analogWrite(BLUE2_PIN, blue);
 }
