#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <math.h>
#include <SolarPosition.h>

float lat = 53.348511319754834;
float lon = 7.062443737385131;
SolarPosition knock(lat,lon);
/*  WLAN Tilli
const char* ssid = "WLAN-421077";
const char* password = "2869707528700678";
*/

// Wlan stube:
const char* ssid = "TP-Link_4DD8";
const char* password = "90650147";

// Zeitzone für Deutschland (Berlin): UTC+2 im Sommer, UTC+1 im Winter
const char* ntpServer = "pool.ntp.org";

WiFiUDP ntpUDP;
// UTC+2 (Mitteleuropäische Sommerzeit): 2 * 3600 = 7200 Sekunden
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);


String sunTime(int time) {
  String result;
  int hour = time / 60;
  int minute = time % 60;
  if (hour < 10) result += "0";
  result = (String)hour + ":";
  if (minute < 10) result += "0";
  result += (String)minute;
  return result;
}

time_t getTime(){
  timeClient.update();
  return timeClient.getEpochTime();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Verbindung zum WLAN...");
  }
  timeClient.begin();
  setTime(getTime());
  SolarPosition::setTimeProvider(getTime);
}

void loop() {
  if (timeClient.update()) {
    setTime(timeClient.getEpochTime());
  }

  Serial.println(timeClient.getFormattedTime());

  int currentYear = year();
  int currentMonth = month();
  int currentDay = day();


  Serial.print("Azimuth: ");
  Serial.println(knock.getSolarAzimuth());
  Serial.print("Altitude: ");
  Serial.println(knock.getSolarElevation());
  Serial.println();
  delay(1000);
}
