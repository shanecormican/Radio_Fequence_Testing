#include <AltSoftSerial.h> //Include all relevant libraries - see above
#include <TinyGPS++.h>
#include <GSM.h>
#include <VirtualWire.h>
#define PINNUMBER "" // PIN Number for the SIM - leave blank unless your SIM has a pin, this is inserted between ""

static const uint32_t GPSBaud = 9600; //Baud rate for communication with the GPS, Adafruit GPS = 9600, your GPS may well be 4800, check the spec

TinyGPSPlus GPS; // The TinyGPS++ object for interfacing with the GPS

AltSoftSerial ss; // The serial connection object to the GPS device

String derection;
float oldLat = 53.27898;
float oldLong = -9.00984;;
float Calibrating = 0.0003;
int count = 0;

const int ledPin =  12;

GSM gsmAccess; // Initialise the library instances
GSM_SMS sms;

char phoneNumber[20] = "+353851262620";
char phoneNumber1[20] = "+353851262620";

void setup() {
  setuoFunction();
}

void loop() {

  voidLoop();
}

void setuoFunction() {
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
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) { // if it succeeds connecting
      notConnected = false; // connected
    }
    else
    {
      Serial.println("Not connected"); // print to the computer
      delay(1000); //delay
    }
  }
  Serial.println("Finished setting up!!!!!!"); // Print to the computer
}

void voidLoop() {
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    if (buf[0] == '1') {
      digitalWrite(ledPin, 1);
      sendTextPanicButton();
    }
    if (buf[0] == '0') {
      count++;
      digitalWrite(ledPin, 0);
      if (count == 500) {
        while (ss.available() > 0) //while there is stuff in the buffer
          if (GPS.encode(ss.read()) ) //if it can successfully decode it, do it. Else try again when more charachters are in the buffer

            getDirection();
        printDerectionTest();

        sendTextDirection();
        count = 0;
      }
    }

  }
}

void printDerectionTest() {
  Serial.print("direction: ");
  Serial.println(derection);
  Serial.print("lat: ");
  Serial.println(GPS.location.lat(), 5);
  Serial.print("lng: ");
  Serial.println(GPS.location.lng(), 5);
}

void sendTextPanicButton() {
  Serial.println("Panic Button !!!");
  sms.beginSMS(phoneNumber1); // begin an sms to the sender number
  sms.print("John Smith Farm Panic Button has been pressed!!! "); // append a comma
  sms.print("\nPlease check if John Smith is ok. "); // append a comma
  sms.print("\nBEWARE OF BULL!!! "); // append a comma
  sms.endSMS(); //send the sms
}

void sendTextDirection() {
  if ( derection.equals("Still")  || derection.equals("Calibrating")  ) {
    // do nothing
  }
  else {
    sms.beginSMS(phoneNumber); // begin an sms to the sender number
    sms.print("Alert Bull Escaped!!"); // append a comma
    sms.print("Heading:" + derection); // append a comma
    sms.print("\nlat: "); // append a comma
    sms.print(GPS.location.lat(), 5); // append a comma
    sms.print("\nlng: "); // append a comma
    sms.print(GPS.location.lng(), 5); // append a comma
    sms.endSMS(); //send the sms
  }
}

void getDirection() {
  if (GPS.location.lat() == 0.0 && GPS.location.lng() == 0.0 ) {
    derection = "Calibrating";
  }
  else if (GPS.location.lat() > oldLat + Calibrating && GPS.location.lng() > oldLong + Calibrating) {
    derection = "NorthEast";
  }
  else if (GPS.location.lat() > oldLat + Calibrating && GPS.location.lng() < oldLong - Calibrating) {
    derection = "NorthWest";
  }
  else if (GPS.location.lat() < oldLat - Calibrating && GPS.location.lng() < oldLong - Calibrating) {
    derection = "SouthWest";
  }
  else if (GPS.location.lat() < oldLat - Calibrating && GPS.location.lng() > oldLong + Calibrating ) {
    derection = "SouthEast";
  }
  else if (GPS.location.lng() < oldLong - Calibrating) {
    derection = "West";
  }
  else if (GPS.location.lng() > oldLong + Calibrating) {
    derection = "East";
  }
  else  if (GPS.location.lat() < oldLat - Calibrating) {
    derection = "South";
  }
  else if (GPS.location.lat() > oldLat + Calibrating) {
    derection = "North";
  }
  else
  {
    derection = "Still";
  }
}

