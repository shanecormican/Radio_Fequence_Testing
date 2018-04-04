#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(3, 2);
Adafruit_GPS GPS(&MySerial);

uint8_t c;
float oldLat = 0;
float oldLong = 0;
void setup() {
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

void loop() {
  // readGPS();
  derectionGPS();
}

void derectionGPS() {
  clearGPS();
  Serial.print("oldLat: ");
  Serial.println(oldLat, 8);
  Serial.print("Latitude: ");
  Serial.println(GPS.latitudeDegrees, 8);
  Serial.print("oldLong: ");
  Serial.println(oldLong, 8);
  Serial.print("Longitude: ");
  Serial.println(GPS.longitudeDegrees, 8);
  delay(10000);
  if (GPS.latitudeDegrees == oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("still ");
  }
  else {
    Serial.println("not still ");
  }
  if (GPS.longitudeDegrees < oldLong && GPS.latitudeDegrees == oldLat) {
     Serial.println("West ");
  }
  else {
    Serial.println("not West ");
  }
  if (GPS.longitudeDegrees > oldLong && GPS.latitudeDegrees == oldLat) {
    Serial.println("East ");
  }
  else {
    Serial.println("not East ");
  }
  if (GPS.latitudeDegrees < oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("South ");
  }
  else {
    Serial.println("not South ");
  }
  if (GPS.latitudeDegrees > oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("North ");
  }
  else {
    Serial.println("not North ");
  }
}

void readGPS() {
  clearGPS();
  printTime();
  printLocation();
  delay(4000);
}

void clearGPS() {
  searchNMEA();
  searchNMEA();
  searchNMEA();
}
void printTime() {
  Serial.print("\nTime: ");
  Serial.print(GPS.hour + 1);
  Serial.print(':');
  Serial.print(GPS.minute);
  Serial.print(':');
  Serial.println(GPS.seconds);
}
void printLocation() {
  Serial.print("Latitude: ");
  Serial.println(GPS.latitudeDegrees, 4); Serial.println(GPS.lat);
  Serial.print("Longitude: ");
  Serial.println(GPS.longitudeDegrees, 4); Serial.println(GPS.lon);
  Serial.println("");
}
void searchNMEA() {
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}

