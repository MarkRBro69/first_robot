#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

IPAddress apIP(192,168,0,10);
IPAddress targetIp(192,168,0,15);
IPAddress gateway(192,168,0,10);
IPAddress subnet(255,255,255,0);

const char* ssid = "robot";
const char* password = "12345678";

WiFiUDP Udp;
unsigned int localUdpPort = 4210;

int translatePacketSize = 7; // R(connection check)|000(distance forward)|000(distance back)|;(terminator)
int recievePacketSize = 13; // C(connection check)|9000(directiond UD + velocity)|9000(directiond LR + velocity)|0(button)|;(terminator)

int msgPosition = 0;
char incomingMsg[60];
char tempByte;
String parsedMsg = "";

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.setTimeout(20);
  while (!Serial){}
  Serial.println();
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP(ssid,password);
  delay(2000);
  Serial.println("AP Mode Activated");
  Serial.println( IPAddress(192, 168, 0, 10));
  Udp.begin(localUdpPort);
  delay(2000);
  Serial.printf("Now Streaming at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}

void reportConnection(char incMsg[])
{
  parsedMsg = String(incMsg);
  if (parsedMsg == "check")
  {
    if (WiFi.softAPgetStationNum() == 0)
    {
      Serial.println("no connection;");
    }
    else
    {
      Serial.println("connected;");
    }
  }
  parsedMsg = "";
}

void readSerial()
{
  tempByte = Serial.read();
  if (tempByte != ';')
  {
    if (tempByte == '\n' || msgPosition > 20)
    {
      incomingMsg[0] = NULL;
      msgPosition = 0;
      tempByte = NULL;
      return;
    }
    else
    {
      incomingMsg[msgPosition] = tempByte;
      msgPosition = msgPosition + 1;
    }
  }
  else
  {
    incomingMsg[msgPosition] = NULL;
    msgPosition = 0;
    tempByte = NULL;
  }
}

void sendSignal()
{
  if (Serial.available() > 0)
  {
    readSerial();
  }
  if (incomingMsg[0] == 'c')
  {
    reportConnection(incomingMsg);
  }
  else if (incomingMsg[0] == 'R')
  {
    int packetSize = Udp.parsePacket();

    if (!packetSize)
    {
      Udp.beginPacket(targetIp, localUdpPort);
      Udp.write(incomingMsg);
      Udp.endPacket();
    }
  }
}

void getSignal()
{
  int packetSize = Udp.parsePacket();

  if (packetSize)
  {
    char incomingXYB[60];
    int len = Udp.read(incomingXYB, 60);
    incomingXYB[len] = NULL;

    if (incomingXYB[0] == 'C' && len == recievePacketSize)
    {
      incomingXYB[len] = ';';
      incomingXYB[len + 1] = NULL;
      Serial.println(incomingXYB); 
    }
  }
}

void loop()
{      
  sendSignal();
  getSignal();
}
