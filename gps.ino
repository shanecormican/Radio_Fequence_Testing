#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial MySerial(3, 2);
Adafruit_GPS GPS(&MySerial);

String NMEA1;
String NMEA2;
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

  readGPS();
  delay(750);

}

void readGPS() {
  clearGPS();
  PrintTime();
  PrintLocation();
}

void clearGPS() {
  searchNMEA();
  searchNMEA();
  searchNMEA();
}
void PrintTime() {
  Serial.print("\nTime: ");
  Serial.print(GPS.hour + 1);
  Serial.print(':');
  Serial.print(GPS.minute); 
  Serial.print(':');
  Serial.println(GPS.seconds);
}
void PrintLocation() {
  Serial.print("Latitude: ");
  Serial.println(GPS.latitudeDegrees, 4);
  Serial.print("Longitude: ");
  Serial.println(GPS.longitudeDegrees, 4);
  Serial.println("");
}
void searchNMEA() {
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}

