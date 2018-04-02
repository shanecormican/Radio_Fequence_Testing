#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(3, 2);
Adafruit_GPS GPS(&MySerial);

uint8_t c;

void setup() {
  Serial.begin(115200);
  GPS.begin(9600);
  GPS.sendCommand("$PGCMD,33,0*6D");
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  delay(1000);
}

void loop() {
  // readGPS();
  derectionGPS();
}

void derectionGPS() {
  clearGPS();
  printLocation();
  float oldLat = GPS.latitudeDegrees;
  float oldLong = GPS.longitudeDegrees;
  delay(5000);
  if (GPS.latitudeDegrees == oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("tes1 ");
  }
  else {
    Serial.println("nothing ");
  }
  if (GPS.latitudeDegrees < oldLat && GPS.latitudeDegrees == oldLat) {
    Serial.println("test1 ");
  }
  else {
    Serial.println("nothing1 ");
  }
  if (GPS.longitudeDegrees < oldLong && GPS.latitudeDegrees == oldLat) {
    Serial.println("test2 ");
  }
  else {
    Serial.println("nothing2 ");
  }
  if (GPS.latitudeDegrees > oldLat && GPS.latitudeDegrees == oldLat) {
    Serial.println("test3 ");
  }
  else {
    Serial.println("nothing3 ");
  }
  if (GPS.longitudeDegrees > oldLong && GPS.latitudeDegrees == oldLat) {
    Serial.println("test4 ");
  }
  else {
    Serial.println("nothing4 ");
  }
  if (GPS.latitudeDegrees < oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("test5 ");
  }
  else {
    Serial.println("nothing5 ");
  }
  if (GPS.longitudeDegrees < oldLong && GPS.longitudeDegrees == oldLong) {
    Serial.println("test6 ");
  }
  else {
    Serial.println("nothing6 ");
  }
  if (GPS.latitudeDegrees > oldLat && GPS.longitudeDegrees == oldLong) {
    Serial.println("test7 ");
  }
  else {
    Serial.println("nothing7 ");
  }
  if (GPS.longitudeDegrees > oldLong && GPS.longitudeDegrees == oldLong) {
    Serial.println("test8 ");
  }
  else {
    Serial.println("nothing8 ");
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

