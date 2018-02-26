#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

void setup()
{
    Serial.begin(115200); // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    uint8_t buf[13]; // uint8_t = unsigned char
    uint8_t buflenght = sizeof(buf);
    if (driver.recv(buf, &buflenght)) // Non-blocking
    {
      if ((char*)buf == "Helow world") {
        Serial.println((char*)buf);
      }
      // Message with a good checksum received, dump it.
      //Serial.println((char*)buf);         
    }
   
}
