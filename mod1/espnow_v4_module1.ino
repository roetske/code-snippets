
//test io signalen 
/*
device esp32 4mb
esp32 dev generic

1ste keer wifi netwerk bekijken met easy to use Wifi Analyser
dan juiste kanaal zetten
*/
/*comments
begint al goed problemen met code on receive
volgens opzoekingen verschil v2 v3 arduino 
Compiler 2..
In the older compiler it was formulated
(const uint8_t* mac, const uint8_t* incomingData, int len)
In the revised code, it is
(const esp_now_recv_info_t *info, const uint8_t *data, int len)
F 
you were using the mac address in your original receive function, 
you will need to extract it from the new "info" 
(esp_now_recv_info_t structure/variable). Otherwise the code worked fine.

rebooten door sta
flag insteken
bleuled blink so that you see com is working
result sent faulty ok and io frozen when i power off one esp
callback  onsent works correctly
watchdog 3 seconds
*/
#include <esp_now.h>

#include <esp_wifi.h>
#include <elapsedMillis.h>
#include <esp_task_wdt.h>

//watchdog
esp_task_wdt_config_t watchdogcfg;

//uitgangen
#define o_output1 27
#define o_output2 26
#define o_blueled 25 //heartbeat
//ingangen
#define i_input1 33
#define i_input2 32

// MAC Address of the second ESP32 module
const uint8_t MAC_MODULE2[] = { 0x08, 0xD1, 0xF9, 0xF8, 0x6C, 0xA4 };
// MAC Address of the first ESP32 module
const uint8_t MAC_MODULE1[] = { 0x40, 0x22, 0xD8, 0x04, 0x04, 0xC0 };
byte sendMacAddr[6];

// Wi-Fi channel
#define chan_ap 4

//status booleans
bool bbleuled = false;
bool bincoming_output1 = false;
bool bincoming_output2 = false;
bool bincoming_input1 = false;
bool bincoming_input2 = false;
bool bincoming_heartbeat = false;

elapsedMillis scan;
elapsedMillis cycletime;
elapsedMillis watchdog;
int lastcycletime =0;
int scaninterval = 1000; //in millis
elapsedMillis heartbeat;
bool bledstate = false;

//esp now
bool flagdata = false;
//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    bool input1;
    bool input2;
    bool output1;
    bool output2;
    bool heartbeat;
} struct_message;

// Create a struct_message called BME280Readings to hold sensor readings
struct_message localreadings;

// Create a struct_message to hold incoming sensor readings
struct_message incomingreadings;

esp_now_peer_info_t peerInfo;
// Variable to store if sending data was successful
bool bsucces = false;
// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0)
   {
    Serial.println("send:)");
    bsucces = true;
    }
  else
   { Serial.println("send Oeps:(");
     bsucces = false;//gebruiken voor safety zenden
   }
}

// Callback when data is received o
//void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) //idf4 of lager in arduino ide

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len)
{ //works like an interrupt code must be minimal in here lol
  memcpy(sendMacAddr, info->src_addr, 6);
  memcpy(&incomingreadings, incomingData, sizeof(incomingreadings));
  bincoming_input1= incomingreadings.input1;
  bincoming_input2 = incomingreadings.input2;
  bincoming_heartbeat = incomingreadings.heartbeat;
  flagdata = true;
}

//***********
void setup()
//***********
{
  Serial.begin(115200);
  
  setupio();
  //espnow
  //connection setup wifi sta mode and active
  Serial.print( "Setup wireless wifi mode.." ); 
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  delay(500);
  while (esp_wifi_start() !=ESP_OK)
     { delay(100);}; 
  //init esp now 
  Serial.print( "Initializing the ESP-NOW..." );
  if( esp_now_init() != ESP_OK )
    { 
      Serial.println("ERROR");
      delay(5000);  
      ESP.restart();
    }
  else
    { 
        delay(500); 
        Serial.println("OK"); 
    }
  // Set the channel for 2.4 GHz
    esp_err_t result = esp_wifi_set_channel(chan_ap, WIFI_SECOND_CHAN_NONE);
    if (result == ESP_OK) {
        Serial.println("Channel set successfully");
    } else {
        Serial.printf("Failed to set channel with error code: %d\n", result);
        // TODO: handle init error
    }
  
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, MAC_MODULE2, 6);
  peerInfo.channel = chan_ap;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
      Serial.println("Failed to add peer");
      return;
    }
  // Register for a callback function that will be called when data is received
  // works like an interrupt event triggered
  esp_now_register_recv_cb(OnDataRecv);
  //watchdog
  Serial.println("Configuring WDT...");
  watchdogcfg.timeout_ms = 3000;
  watchdogcfg.idle_core_mask= 1;
  watchdogcfg.trigger_panic = true;
  esp_task_wdt_init(&watchdogcfg); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch
}
//*************
void loop()
{ if (watchdog >(watchdogcfg.timeout_ms -1000))
    { Serial.println("Resetting WDT...");
      esp_task_wdt_reset();
      watchdog =0;
    }
  if (scan > scaninterval)
  { // Set values to send
    getlocalreading();
     // Send message via ESP-NOW
    esp_err_t result = esp_now_send(MAC_MODULE2, (uint8_t *) &localreadings, sizeof(localreadings));
    // if (result == ESP_OK) //dit werkte niet gaf ok bij spanning af
    //  {bsucces = true;
    //  }
    // else 
    //   {bsucces= false;}
    
    //handle incoming
    if (flagdata and bsucces)
     {  if (bincoming_input1 )
        {digitalWrite(o_output1,HIGH);}
      else
        {digitalWrite(o_output1,LOW);}
      if (bincoming_input2) 
        {digitalWrite(o_output2,HIGH);}
      else
        {digitalWrite(o_output2,LOW);} 
      if (bincoming_heartbeat)
          {digitalWrite(o_blueled,HIGH);}
      else
        {digitalWrite(o_blueled,LOW);}
      flagdata = false;
     }
     if (!bsucces)
       { bledstate = false;
         digitalWrite(o_output1,LOW);
         digitalWrite(o_output2,LOW);
         digitalWrite(o_blueled,LOW);
       }
    scan = 0;
  }
 
 
 //cyclustijd
 delay(10); //beveiliging 100%
 lastcycletime = cycletime;
 cycletime =0 ;

 if (lastcycletime > 100)
  {
   Serial.print("Cycletime=");
   Serial.println(lastcycletime);
  }
}
//++++++++++++++++++++++++++++++
void getlocalreading()
  {
     //io lezen
     //inputs
    localreadings.input1 = digitalRead(i_input1);
    localreadings.input2 = digitalRead(i_input2);
    localreadings.output1 = digitalRead(o_output1);
    localreadings.output2 = digitalRead(o_output2);
    //heartbeat
    if (bledstate == false)
         {bledstate = true;}
      else
       {bledstate= false;}
    localreadings.heartbeat = bledstate;
    Serial.println("inputs:");
    Serial.print(localreadings.input1);
    Serial.println(localreadings.input2);
    Serial.print("Heartbeat:=");
    Serial.println(localreadings.heartbeat);
    Serial.println("outputs:");
    Serial.print(localreadings.output1);
    Serial.println(localreadings.output2);

  }
void heartbeatcheck()
{ //locally control visual heartbeat
  if (heartbeat > 500)
    {  if (bledstate == false)
         {bledstate = true;}
      else
       {bledstate= false;}
      digitalWrite(o_blueled,bledstate);
      heartbeat = 0;
    }
}






