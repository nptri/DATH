#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);


#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <espnow.h>
uint8_t peer1[] = {0x30, 0xC6, 0xF7, 0x22, 0x86, 0x64};


//definition of message
typedef struct message {
   int STT;
   String Name;
};
struct message mess;


//data receives from transmitter
void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  Serial.println("Message received: ");
  memcpy(&mess, incomingData, sizeof(mess));
  //Serial.print(conv(mess.rfid,4)); 
}

//start ESPnow
void init_ESPnow()
{
  WiFi.mode(WIFI_STA);
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("\nESP-Now Receiver");
  
  if (esp_now_init() != 0) {
    Serial.println("Problem during ESP-NOW init");
    return;
  }
}

// setup and loop running
void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  init_ESPnow();
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(peer1, ESP_NOW_ROLE_COMBO, 1, NULL, 0);
  esp_now_register_recv_cb(onDataReceiver);

}

void loop() {
  lcd.setCursor(0,0); lcd.print(mess.STT);
  lcd.setCursor(0,2); lcd.print(mess.Name);

}
