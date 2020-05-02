/**********************************************
 * ESP32 + ARDUINO + Blinker + XiaoAi
 * Remote boot by WOL
 * Creater：Faiz
 */

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <Blinker.h>
#include <WiFiUdp.h>
#include <WiFi.h>
char auth[] = "xxxxxxxxxx"; //密钥
char ssid[] = "Faiz"; //wifi名
char pswd[] = "xxxxxxxx"; //wifi密码

//The udp library class
WiFiUDP udp;

bool oState = false;
BlinkerButton Button1("awaking");

//awaking！
void button1_callback(const String & state)
{
   BLINKER_LOG("get button state: ", state);
   digitalWrite(LED_BUILTIN, HIGH);
   delay(50);
   digitalWrite(LED_BUILTIN,LOW);
   delay(50);
   pcawaking();
   digitalWrite(LED_BUILTIN, HIGH);
   delay(50);
   digitalWrite(LED_BUILTIN, LOW);
   delay(50);
   digitalWrite(LED_BUILTIN, HIGH);
   delay(50);
   digitalWrite(LED_BUILTIN, LOW);
   delay(50);
}


void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {
        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
         digitalWrite(LED_BUILTIN, HIGH);
         delay(25);
         digitalWrite(LED_BUILTIN, LOW);
         delay(25);
         pcawaking();
         digitalWrite(LED_BUILTIN, HIGH);
         delay(25);
         digitalWrite(LED_BUILTIN, LOW);
         delay(25);
         digitalWrite(LED_BUILTIN, HIGH);
         delay(25);
         digitalWrite(LED_BUILTIN, LOW);
         oState = false;  //force off！！
    }
    else if (state == BLINKER_CMD_OFF) {
        digitalWrite(LED_BUILTIN, LOW);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
            
    }
        digitalWrite(LED_BUILTIN, HIGH);
        delay(25);
        digitalWrite(LED_BUILTIN, LOW);
        delay(25);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(25);
        digitalWrite(LED_BUILTIN, LOW);
}

void pcawaking()
{
    int i=0;
    char mac[6]={0xXX,0xXX,0xXX,0xXX,0xXX,0xXX};  //mac地址
    char pac[102];
    char * Address = "xxx.xxx.xxx.255";//udp adress //群发最后为.255
    int Port = 3333;//udp port
     //make magicpacket
    for(i=0;i<6;i++)
    {
      pac[i]=0xFF;
    }
    for(i=6;i<102;i+=6)
    {
      memcpy(pac+i,mac,6);
    }
    udp.beginPacket(Address, Port);
    udp.write((byte*)pac, 102);//send pac to txbuffer
    udp.endPacket();//biubiubiu
}

void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);

    Button1.attach(button1_callback);
}

void loop()
{
    Blinker.run();
}
