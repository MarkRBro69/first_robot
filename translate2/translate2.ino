#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "robot"; // robot server
const char* password = "12345678"; // robot password

WiFiUDP Udp;

unsigned int localUdpPort = 4210;
IPAddress apIP(192,168,0,10);
IPAddress ip(192,168,0,15);
IPAddress gateway(192,168,0,10);
IPAddress subnet(255,255,255,0);

int translatePacketSize = 10; // C(connection check)|9000(directiond UD + velocity)|9000(directiond LR + velocity)|0(button)|;(terminator)
int recievePacketSize = 7; // R(connection check)|000(distance forward)|000(distance back)|;(terminator)

int msgPosition = 0;
char incomingMsg[60];
char tempByte;
String parsedMsg = "";
bool msgIsReady = false;

int checkDelay = 1000;
int checkTimeout = 0;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.setTimeout(20);
  while (!Serial){}

  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  Serial.println();
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("no connection;");
  }
  Serial.println("Connected...");
  Udp.begin(localUdpPort);
}

void checkConnection()
{
  if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("no connection;");
      setup();
    }
    else
    {
      Serial.println("connected;");
    }
}

void connectionStatus()
{
  if (millis() - checkTimeout > checkDelay)
  {
    checkTimeout = millis();
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("no connection;");
      setup();
    } 
  } 
}

void reportConnection(char incMsg[])
{
  parsedMsg = String(incMsg);
  if (parsedMsg == "check")
  {
    checkConnection();
  }
  parsedMsg = "";
}

void readSerial()
{
  msgIsReady = false;
  tempByte = Serial.read();
  if (tempByte != ';')
  {
    if (tempByte == '\n' || msgPosition > 20)
    {
      incomingMsg[0] = '\0';
      msgPosition = 0;
      tempByte = '\0';
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
    incomingMsg[msgPosition] = '\0';
    msgPosition = 0;
    tempByte = '\0';
    msgIsReady = true;
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
  else if (incomingMsg[0] == 'C')
  {
    int packetSize = Udp.parsePacket();

    if (!packetSize && msgIsReady)
    {
      Udp.beginPacket(apIP, localUdpPort);
      Udp.write(incomingMsg);
      Udp.endPacket();
      msgIsReady = false;
      incomingMsg[0] = '\0';
    }
  } 
}

void getSignal()
{
  int packetSize = Udp.parsePacket();

  if (packetSize)
  {
    char incomingMsg[60];
    int len = Udp.read(incomingMsg, 60);
    incomingMsg[len] = '\0';

    if (incomingMsg[0] == 'R' && len == recievePacketSize)
    {
      incomingMsg[len] = ';';
      incomingMsg[len + 1] = '\0';
      Serial.println(incomingMsg);
    }
  }
}

void loop()
{
  connectionStatus();
  sendSignal();
  getSignal();
}