#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);

#define BLYNK_V0 0
#define BLYNK_V1 1
#define BLYNK_V2 2
#define BLYNK_V3 3
#define BLYNK_V4 4
#define BLYNK_V5 5
#define BLYNK_V6 6
#define BLYNK_V7 7
#define BLYNK_V8 8
#define BLYNK_V9 9
#define BLYNK_V10 10
#define BLYNK_V11 11
#define BLYNK_V12 12
#define BLYNK_V13 13
#define BLYNK_V14 14
#define BLYNK_V15 15
#define BLYNK_V16 16
#define BLYNK_V17 17
#define BLYNK_V18 18
#define BLYNK_V19 19

     #define mqtt_server "broker.emqx.io"
     #define mqtt_port   1883
     
     typedef void (*i47Blynk_callbackF)(String data);
     static String blynk_token;
     static String topic_path;
     static String mqtt_client_id;
     static i47Blynk_callbackF list_callback[20];
     
     static void callback(char* topic, byte* payload, unsigned int length)  // Hàm call back để nhận dữ liệu từ app
     {
        String dat = "";
        for (int i = 0; i < length; i++)dat+= (char)payload[i];   
        int virtual_pin = (topic[25]-48)*10 + (topic[26]-48);
        if(virtual_pin==99)client.publish(blynk_token.c_str(),"online"); //gửi tin nhắn báo device đã online
        else if( list_callback[virtual_pin] != 0)list_callback[virtual_pin](dat);
     }
     static void reconnect() // Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
     {
        uint32_t static t_reconnect = 0;
      
        if(millis() - t_reconnect > 3000)
        {
          while (!client.connected()) // Chờ tới khi kết nối
          {
            // Thực hiện kết nối với mqtt user và pass
            if (client.connect(mqtt_client_id.c_str(),blynk_token.c_str(),0,0,"offline"))  //kết nối vào broker
            {
              Serial.println("Đã kết nối tới máy chủ");
              client.publish(blynk_token.c_str(),"online"); //gửi tin nhắn báo device đã online
              for(int i=0;i<20;i++)
              {
                String topic_virtual = String(blynk_token) + "deviceRX_V" + String(i/10) + String(i%10);
                client.subscribe(topic_virtual.c_str()); //đăng kí nhận dữ liệu từ topic ESP_doan_dieukhien
              }
              String topic_virtual = String(blynk_token) + "deviceRX_V99";
              client.subscribe(topic_virtual.c_str()); 
            }
            else 
            {
              Serial.print("Lỗi:, rc=");
              Serial.print(client.state());
              t_reconnect = millis();
            }
          }
        }
     }

    //------------------------------pubblic function--------------------------
    void i47Blynk_callback(int v, i47Blynk_callbackF action)
    {
      list_callback[v] = action;
    }
    void i47Blynk_begin(String _blynk_token)
    {
       blynk_token = _blynk_token;
       
       mqtt_client_id = String(WiFi.macAddress());
       mqtt_client_id.replace(":","");
       mqtt_client_id+= "_i47blynk";
       topic_path = String(blynk_token) + "deviceRX_V";
       client.setServer(mqtt_server, mqtt_port); 
       client.setCallback(callback);
    }
    void i47Blynk_loop()
    {
      if (!client.connected())reconnect();
      client.loop();  
    }
    void i47Blynk_write(int v,String dat)
    {
       String topic_virtual = String(blynk_token) + "appRX_V" + String(v/10) + String(v%10);
       client.publish(topic_virtual.c_str(),dat.c_str());
    }
    void i47Blynk_write(int v,int dat)
    {
       String topic_virtual = String(blynk_token) + "appRX_V" + String(v/10) + String(v%10);
       client.publish(topic_virtual.c_str(),String(dat).c_str());
    }
//----------------------------------------------------------
