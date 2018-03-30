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
  int oldLat = 53.2411;
  int oldLong = -8.8621;
  delay(5000);
  if(GPS.latitudeDegrees < oldLat){
    Serial.println("test ");
  }
  else{
    Serial.println("nothing ");
  }
 if(GPS.longitudeDegrees< oldLong){
    Serial.println("test1 ");
  }
  else{
    Serial.println("nothing1 ");
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
  Serial.println(GPS.longitudeDegrees, 4);Serial.println(GPS.lon);
  Serial.println("");
}
void searchNMEA() {
  while (!GPS.newNMEAreceived()) {
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA());
}

