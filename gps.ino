#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(5, 4);
Adafruit_GPS GPS(&MySerial);

#include <GSM.h>
#define PINNUMBER ""
GSM gsmAccess;
GSM_SMS sms;

uint8_t c;
String derection;
float oldLat = 0;
float oldLong = 0;

char remoteNumber[20] = "+353851262620";

void setup() {
  gps_Setup();
  gsm_Setup();
}

void loop() {
  derectionGPS();
}

void derectionGPS() {
  clearGPS();
  printTestCode();
  delay(10000);

  if (GPS.longitudeDegrees < oldLong && GPS.latitudeDegrees == oldLat) {
    derection = "East";
  }
  else if (GPS.longitudeDegrees > oldLong && GPS.latitudeDegrees == oldLat) {
    derection = "West";
  }
  else  if (GPS.latitudeDegrees < oldLat && GPS.longitudeDegrees == oldLong) {
    derection = "South";
  }
  else if (GPS.latitudeDegrees > oldLat && GPS.longitudeDegrees == oldLong) {
    derection = "North";
  }
  else if (GPS.latitudeDegrees > oldLat && GPS.longitudeDegrees > oldLong) {
    derection = "NorthWest";
  }
  else if (GPS.latitudeDegrees > oldLat && GPS.longitudeDegrees < oldLong) {
    derection = "NorthEast";
  }
  else if (GPS.latitudeDegrees < oldLat && GPS.longitudeDegrees < oldLong) {
    derection = "SouthEast";
  }
  else if (GPS.latitudeDegrees < oldLat && GPS.longitudeDegrees > oldLong) {
    derection = "SouthWest";
  }
  else
  {
    derection = "still";
  }
  sendSMS();
  Serial.println(derection);
}

void clearGPS() {
  searchNMEA();
  searchNMEA();
  searchNMEA();
  searchNMEA();
}

void searchNMEA() {
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}

void printTestCode() {
  Serial.print("oldLat: ");
  Serial.println(oldLat, 8);
  Serial.print("Latitude: ");
  Serial.println(GPS.latitudeDegrees, 8);
  Serial.print("oldLong: ");
  Serial.println(oldLong, 8);
  Serial.print("Longitude: ");
  Serial.println(GPS.longitudeDegrees, 8);
}

void sendSMS() {

  // send the message
  sms.beginSMS(remoteNumber);
  sms.print(derection);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}

void gps_Setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand("$PGCMD,33,0*6D");
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);
  clearGPS();
  oldLat = GPS.latitudeDegrees;
  oldLong = GPS.longitudeDegrees;
}

void gsm_Setup() {
  boolean notConnected = true;

  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected)
  {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}
