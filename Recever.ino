#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

const int buttonPin = 2;     // the number of the pushbutton pin

int buttonState = 0;         // variable for reading the pushbutton status

void setup()
{
    Serial.begin(115200);   // Debugging only
    if (!driver.init())
         Serial.println("init failed");

         
    pinMode(buttonPin, INPUT);
}

void loop()
{
   int count = 0;
  buttonState = digitalRead(buttonPin);

   if (buttonState == HIGH && count == 0) {
    count++;
  } 
  else if (buttonState == HIGH && count == 1) {
    count == 0;
  } 
  else if (count == 1){
    const char *msg = "Hello World!!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1250);
  }
  else if(count == 0){
    const char *msg = "new world!!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1250);
  }
 
}
