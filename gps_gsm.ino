#include <AltSoftSerial.h> //Include all relevant libraries - see above
#include <TinyGPS++.h>
#include <GSM.h>
#include <VirtualWire.h>
#define PINNUMBER "" // PIN Number for the SIM - leave blank unless your SIM has a pin, this is inserted between ""

static const uint32_t GPSBaud = 9600; //Baud rate for communication with the GPS, Adafruit GPS = 9600, your GPS may well be 4800, check the spec

TinyGPSPlus GPS; // The TinyGPS++ object for interfacing with the GPS

AltSoftSerial ss; // The serial connection object to the GPS device

String derection;
//float oldLat = 53.27892;
//float oldLong = -9.00985;
float oldLat = 0;
float oldLong = 0;
float error_factor = 0.00002;

const int ledPin =  12;

GSM gsmAccess; // Initialise the library instances
GSM_SMS sms;

char phoneNumber[20] = "+353851262620";
char phoneNumber1[20] = "+353851262620";

void setup() {
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(50);
  vw_setup(4000); // Bits per sec
  pinMode(ledPin, OUTPUT);

  vw_rx_start(); // Start the receiver PLL running

  ss.begin(GPSBaud); // begin the GPS serial connection

  Serial.begin(9600); // begin Serial communication with the computer at 9600 baud rate

  Serial.println("Setting up!!!!!!"); // Print to the computer

  boolean notConnected = true; // connection state

  while (notConnected) // until it connects
  {
    Serial.println("0");
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) { // if it succeeds connecting
      notConnected = false; // connected
    }
    else
    {
      Serial.println("Not connected"); // print to the computer
      delay(1000); //delay
    }
  }
  Serial.println("4");
}

void loop() {

  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    if (buf[0] == '1') {
      digitalWrite(ledPin, 1);
      Serial.println("Panic Button !!!");
      sms.beginSMS(phoneNumber1); // begin an sms to the sender number
      sms.print("John Smith Farm Panic Button has been pressed!!! "); // append a comma
      sms.print("\nPlease check if John Smith is ok. "); // append a comma
        sms.print("\nBE WARE OF BULL!!! "); // append a comma
      sms.endSMS(); //send the sms
    }
    if (buf[0] == '0') {
      digitalWrite(ledPin, 0);
      while (ss.available() > 0) //while there is stuff in the buffer
        if (GPS.encode(ss.read()) ) //if it can successfully decode it, do it. Else try again when more charachters are in the buffer


          if (GPS.location.lng() < oldLong - error_factor && GPS.location.lat() == oldLat) {
            derection = "East";
          }
          else if (GPS.location.lng() > oldLong + error_factor && GPS.location.lat() == oldLat) {
            derection = "West";
          }
          else  if (GPS.location.lat() < oldLat - error_factor && GPS.location.lng() == oldLong) {
            derection = "South";
          }
          else if (GPS.location.lat() > oldLat + error_factor && GPS.location.lng() == oldLong) {
            derection = "North";
          }
          else if (GPS.location.lat() == 0.0 && GPS.location.lng() == 0.0 ) {
            derection = "calibrating";
          }
          else if (GPS.location.lat() > oldLat + error_factor && GPS.location.lng() > oldLong + error_factor) {
            derection = "NorthEast";
          }
          else if (GPS.location.lat() > oldLat + error_factor && GPS.location.lng() < oldLong - error_factor) {
            derection = "SouthWest";
          }
          else if (GPS.location.lat() < oldLat - error_factor && GPS.location.lng() < oldLong - error_factor) {
            derection = "NorthWest";
          }
          else if (GPS.location.lat() < oldLat - error_factor && GPS.location.lng() > oldLong + error_factor ) {
            derection = "SouthEast";
          }
          else
          {
            derection = "still";
          }
      Serial.print("direction: ");
      Serial.println(derection);
      Serial.print("lat: ");
      Serial.println(GPS.location.lat(), 5);
      Serial.print("lng: ");
      Serial.println(GPS.location.lng(), 5);

      //      if ( derection != "still" || derection != "calibrating") {
      //        sms.beginSMS(phoneNumber); // begin an sms to the sender number
      //        sms.print("Alert Bull Escaped!!"); // append a comma
      //        sms.print("Heading:" + derection); // append a comma
      //        sms.endSMS(); //send the sms
      //      }
    }
  }
  delay(1000);

}
