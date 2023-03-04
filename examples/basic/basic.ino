#include <ESP8266WiFi.h>
#include "i47blynk.h"

//Khai báo thông tin wifi
const char* ssid = "qstech";
const char* password = "0123456789";

void setup() 
{
 Serial.begin(115200);
 pinMode(2,OUTPUT); //chân led xanh
 digitalWrite(2,1);
 //----------Kết nối vào mạng wifi----------
 Serial.println();          
 Serial.print("Dang ket noi den mang… ");
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED)
 {
    delay(500);
    Serial.print(".");
 }
 Serial.println("Da ket noi WiFi");
 Serial.println(WiFi.localIP());

 i47Blynk_begin("i47NLulK4JZTVX5"); //nhập token vào đây
 i47Blynk_callback(BLYNK_V0,[](String data){
  Serial.println(data);
 });
 i47Blynk_callback(BLYNK_V1,[](String data){
  Serial.println(data);
 });
}

uint32_t tSent;
void loop() 
{
 i47Blynk_loop();
 if(millis() - tSent > 1000)
 {
   tSent = millis();
   i47Blynk_write(BLYNK_V0,millis());
 }
}
