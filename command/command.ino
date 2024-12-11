int CONNECTION = 2;

int FG = 3;
int FY = 4;
int FR = 5;

int BG = 9;
int BY = 10;
int BR = 11;

String msg = "";

//motors setup
int IUD = A0;
int ILR = A1;
int BUTTON = 8;

int UDMID = 0;
int UDMAX = 0;
int UDMIN = 0;

int LRMID = 0;
int LRMAX = 0;
int LRMIN = 0;

int UD = 0;
int LR = 0;
bool BUT = 0;

//manipulator setup
int INPUT_MANIPULATOR_LR = A3;
int INPUT_MANIPULATOR_UD = A2;
int INPUT_MANIPULATOR_BUT = 7;

int MANIPULATOR_LR_MID = 0;
int MANIPULATOR_LR_MAX = 0;
int MANIPULATOR_LR_MIN = 0;

int MANIPULATOR_UD_MID = 0;
int MANIPULATOR_UD_MAX = 0;
int MANIPULATOR_UD_MIN = 0;

int MANIPULATOR_LR = 0;
int MANIPULATOR_UD = 0;
bool MANIPULATOR_BUT = 0;

int MID = 10; //to check

int msgPosition = 0;
char incomingMsg[60];
char tempByte;
String parsedMsg = "";

unsigned long checkTimer = 0;
int checkDelay = 1000; 

void setup()
{
  delay(1000);

  pinMode(BUTTON, INPUT);

  pinMode(CONNECTION, OUTPUT);
  pinMode(FG, OUTPUT);
  pinMode(FY, OUTPUT);
  pinMode(FR, OUTPUT);
  pinMode(BG, OUTPUT);
  pinMode(BY, OUTPUT);
  pinMode(BR, OUTPUT);

  digitalWrite(CONNECTION, LOW);
  digitalWrite(FG, LOW);
  digitalWrite(FY, LOW);
  digitalWrite(FR, LOW);
  digitalWrite(BG, LOW);
  digitalWrite(BY, LOW);
  digitalWrite(BR, LOW);

  //motors calibration  
  UDMID = analogRead(IUD);
  UDMAX = 1023 - UDMID;
  UDMIN = 0 + UDMID;

  LRMID = analogRead(ILR);
  LRMAX = 1023 - LRMID;
  LRMIN = 0 + LRMID;
  
  //manipulator calibration
  MANIPULATOR_LR_MID = analogRead(INPUT_MANIPULATOR_LR);
  MANIPULATOR_LR_MAX = 1023 - MANIPULATOR_LR_MID;
  MANIPULATOR_LR_MIN = 0 + MANIPULATOR_LR_MID;

  MANIPULATOR_UD_MID = analogRead(INPUT_MANIPULATOR_UD);
  MANIPULATOR_UD_MAX = 1023 - MANIPULATOR_UD_MID;
  MANIPULATOR_UD_MIN = 0 + MANIPULATOR_UD_MID;

  Serial.begin(9600);
  Serial.setTimeout(20);
  while (!Serial){}

  checkSignal();
}

void checkSignal()
{
  checkTimer = millis();
  Serial.println("check;");
  digitalWrite(CONNECTION, LOW);

  while (parsedMsg != "connected")
  {
    if (millis() - checkTimer > checkDelay)
    {
      Serial.println("check;");
      checkTimer = millis();
    }
    if (Serial.available() > 0)
    {
      readSerial();
    }
  }
  digitalWrite(CONNECTION, HIGH);
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
    parsedMsg = String(incomingMsg);
    Serial.println(incomingMsg);
  }
}

void readMotorInput()
{
  int tempInt;

  UD = analogRead(IUD);
  LR = analogRead(ILR);

  BUT = digitalRead(BUTTON) ^ 1;

  UD = UD - UDMID;
  if(UD >= 0)
  {
    msg = msg + "9";
    UD = map(UD, 0, UDMAX, 0 ,255);
  }
  else {
    msg = msg + "8";
    UD = abs(UD);
    UD = map(UD, 0, UDMIN, 0 ,255);
  }
  tempInt = UD / 100;
  msg = msg + tempInt;
  tempInt = UD / 10 % 10;
  msg = msg + tempInt;
  tempInt = UD % 10;
  msg = msg + tempInt;

  LR = LR - LRMID;
  if(LR >= 0) {
    msg = msg + "9";
    LR = map(LR, 0, LRMAX, 0 ,255);
  }
  else {
    msg = msg + "8";
    LR = abs(LR);
    LR = map(LR, 0, LRMIN, 0 ,255);
  }
  tempInt = LR / 100;
  msg = msg + tempInt;
  tempInt = LR / 10 % 10;
  msg = msg + tempInt;
  tempInt = LR % 10;
  msg = msg + tempInt;

  msg = msg + BUT;
}

void readManipulatorInput()
{
  int tempInt;

  MANIPULATOR_LR = analogRead(INPUT_MANIPULATOR_LR);
  MANIPULATOR_UD = analogRead(INPUT_MANIPULATOR_UD);

  MANIPULATOR_BUT = digitalRead(INPUT_MANIPULATOR_BUT) ^ 1;

  MANIPULATOR_LR = MANIPULATOR_LR - MANIPULATOR_LR_MID;
  if(MANIPULATOR_LR >= 150)
  {
    msg = msg + "L";
  }
  else if (MANIPULATOR_LR <= -150)
  {
    msg = msg + "R";
  }
  else 
  {
    msg = msg + "S";
  }

  MANIPULATOR_UD = MANIPULATOR_UD - MANIPULATOR_UD_MID;
  if(MANIPULATOR_UD >= 150)
  {
    msg = msg + "D";
  }
  else if (MANIPULATOR_UD <= -150)
  {
    msg = msg + "U";
  }
  else 
  {
    msg = msg + "S";
  }

  if (MANIPULATOR_BUT == 1)
  {
    msg = msg + "P";
  }
  else
  {
    msg = msg + "S";
  }

}

void sendSignal()
{
  msg = msg + "C";
  readMotorInput();
  readManipulatorInput();
  msg = msg + ";";
  Serial.println(msg);
  msg = "";
}

void getSignal()
{
  if (Serial.available() > 0)
  {
    readSerial();
    if (parsedMsg == "no connection") 
    {
      checkSignal();
    }   
  }
}

void loop()
{
  sendSignal();
  getSignal();
}
