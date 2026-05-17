#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>

const char ssid[] = "minirobo2026_a";       // SSID
const char password[] = "minirobo";     // password
const int localPort = 22222;              // ポート番号
const IPAddress ip(192, 168, 11, 4);       // ServerのIPアドレス
const IPAddress gateway(192, 168, 11, 4);  // gatewayのIPアドレス
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

WiFiUDP udp;
char packetBuffer[256];

void setup() {
  Serial.begin(115200);
  delay(800);

  WiFi.softAP(ssid, password);
  delay(100);
  WiFi.softAPConfig(ip, gateway, subnet);

  Serial.println("Starting UDP");
  udp.begin(localPort);

  Wire.begin();
  delay(100);
  Wire.beginTransmission(0x08);
  byte error = Wire.endTransmission();
  if(error==0)
  {
    Serial.print("Starting I2C");
  }
  else
  {
    Serial.print("Initial sync check: ");
    Serial.println(error);
  }
}

void loop() {
  int i;

  while (1) {
    int packetSize = udp.parsePacket();
    if (packetSize) {
      for ( i = 0 ; i < 256 ; i++ ) packetBuffer[i] = 0;
      udp.read(packetBuffer, 256);
      Wire.beginTransmission(0x08);
      Wire.print(String(packetBuffer));
      byte error = Wire.endTransmission();
      if(error == 0)
      {
        Serial.println(String(packetBuffer));
      }
      else
      {
        Serial.print("I2C Error:");
        Serial.println(error);
      }
    }
  }
}