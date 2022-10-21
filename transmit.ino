#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN         D3
#define SS_PIN          D4
MFRC522 mfrc522(SS_PIN, RST_PIN);

#include <ESP8266WiFi.h>
#include <Arduino.h>
#include <espnow.h>
uint8_t peer1[] = {0x30, 0xC6, 0xF7, 0x22, 0x86, 0x64};
typedef struct message {
   int STT;
   String Name;
};
struct message mess;

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

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
//  mfrc522.PCD_DumpVersionToSerial();
  mess.STT=0;
}

void loop() {

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) 
  {
    String sv = conv(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(sv);
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    mess.STT++;
    mess.Name=sv;
    esp_now_send(peer1, (uint8_t *) &mess, sizeof(mess));
  }
}

String conv (byte *buffer, byte bufferSize) 
{
  String ss="";
  for (byte i = 0; i < bufferSize; i++) {
    ss+=String(buffer[i] < 0x10 ? "0" : "");
    ss+=String(buffer[i], HEX);
  }
  return ss;
}
