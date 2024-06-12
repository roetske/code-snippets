#include <esp_now.h>
#include <WiFi.h>
#include "esp_wifi.h" //extra library to change mac address

void setup() {
  Serial.begin(115200);
 

  uint8_t CustomMACaddress[] = {0xB0, 0xA7, 0x32, 0x12, 0x34, 0x56}; //Custom MAC address
  //Old code

  // WiFi.mode(WIFI_MODE_STA);
  // Serial.print("Original MAC address: ");
  // Serial.println(WiFi.macAddress());

  // esp_wifi_set_mac(WIFI_IF_STA, &CustomMACaddress[0]);
  // Serial.print("Custom MAC Address for ESP32:  ");
  // Serial.println(WiFi.macAddress());  /*Prints Custom MAC address*/

  //end old code

  //New version
  WiFi.mode(WIFI_STA);
  Serial.print("Before WiFi.STA.started MAC address: ");
  Serial.println(WiFi.macAddress());//mac address 0:0:0:0:0:0
  // esp_wifi_set_mac(WIFI_IF_STA, &CustomMACaddress[0]);//set custom mac address
  while (!WiFi.STA.started()) {
    delay(100);
  }

  //Original mac address is OK
  Serial.print("Original MAC address: ");//Doesn't work once custom address is set.
  Serial.println(WiFi.macAddress());

  
  // //esp_wifi_set_mac(WIFI_IF_STA, &CustomMACaddress[0]);//Does not set MAC address in v3.0.0 in this position
  // Serial.print("Custom MAC Address for ESP32:  ");
  // Serial.println(WiFi.macAddress());  /*Prints Custom MAC address*/
}

void loop() {
  // put your main code here, to run repeatedly:

}
