//#include <Ultrasonic.h>
#include <Servo.h>

//Ultrasonic ultrasonic1(2, 3);
//Ultrasonic ultrasonic2(11, 12);

//manipulator setup
Servo servoLR;
Servo servoUD;
Servo servoOC;

float SERVO_LR_POSITION = 90;
float SERVO_UD_POSITION = 90;
bool SERVO_OC_FLAG = true;

int SERVO_BUTTON_TIMER = 0;
int SERVO_BUTTON_DELAY = 1000;

float SERVO_LR_DIRECTION = 0;
float SERVO_UD_DIRECTION = 0;

float MANIPULATOR_ACCELERATION = 0.3;

//int CONNECTION = 4;

//int distance1 = 0;
//int distance2 = 0;

//motror setup
int enA = 3;
int in1 = 9;
int in2 = 8;

int enB = 5;
int in3 = 7;
int in4 = 6;

int MOTOR_INPUT_MINIMUM = 30;

int SPEED_MINIMUM = 100;
int SPEED_MAXIMUM = 200;

float acceleration = 0.3;
float decceleration = 20;

float leftSpeed = 0;
float rightSpeed = 0;

float leftDirection = 0;
float rightDirection = 0;

long int data = 0;

int msgPosition = 0;
char incomingMsg[60];
char tempByte;
String parsedMsg = "";
bool massageIsReady = false;

unsigned long checkTimer = 0;
int checkDelay = 1000;

void setup()
{
  delay(1000);
  Serial.begin(9600);
  Serial.setTimeout(20);
  while (!Serial){}

  servoLR.attach(11);
  servoUD.attach(12);
  servoOC.attach(13);

  servoLR.write((int)SERVO_LR_POSITION);
  servoUD.write((int)SERVO_UD_POSITION);
  servoOC.write(0);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  //pinMode(CONNECTION, OUTPUT);
  //digitalWrite(CONNECTION, LOW);

  checkSignal();
}

void checkSignal()
{
  checkTimer = millis();
  Serial.println("check;");

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
      parsedMsg = String(incomingMsg);
    }
  }
  //digitalWrite(CONNECTION, HIGH);
  parsedMsg = "";
}

void readSerial()
{
  massageIsReady = false;  
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
    massageIsReady = true;
  }
}

void moveControler()
{
  int tempInt = 0;

  // left motor control
  if (leftDirection > SPEED_MINIMUM)
  {
    if(leftSpeed < SPEED_MINIMUM)
      {
        leftSpeed = SPEED_MINIMUM;
      }
    if (leftSpeed < leftDirection)
    {
      leftSpeed = leftSpeed + acceleration;
    }
    if (leftSpeed > leftDirection)
    {
      leftSpeed = leftDirection;
    }
  }
  else if (leftDirection < -SPEED_MINIMUM)
  {
    if(leftSpeed > -SPEED_MINIMUM)
      {
        leftSpeed = -SPEED_MINIMUM;
      }
    if (leftSpeed > leftDirection)
    {
      leftSpeed = leftSpeed - acceleration;
    }
    if (leftSpeed < leftDirection)
    {
      leftSpeed = leftDirection;   
    }
  }
  else
  {
    if (leftSpeed > SPEED_MINIMUM)
    {
      leftSpeed = leftSpeed - decceleration;
      if (leftSpeed < 0)
      {
        leftSpeed = 0;
      }
    }
    else if (leftSpeed < -SPEED_MINIMUM)
    {
      leftSpeed = leftSpeed + decceleration;
      if (leftSpeed > 0)
      {
        leftSpeed = 0;
      }
    }
    else
    {
      leftSpeed = 0;
    }
  }

  if (leftSpeed > SPEED_MAXIMUM)
  {
    leftSpeed = SPEED_MAXIMUM;
  }
  if (leftSpeed < -SPEED_MAXIMUM)
  {
    leftSpeed = -SPEED_MAXIMUM;
  }

  if (leftSpeed > SPEED_MINIMUM)
  {
    analogWrite(enA, (int)leftSpeed);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } 
  else if (leftSpeed < -SPEED_MINIMUM)
  {
    tempInt = abs((int)leftSpeed);
    analogWrite(enA, tempInt);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else
  {
    analogWrite(enA, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }

  // right motor control
  if (rightDirection > SPEED_MINIMUM)
  {
    if(rightSpeed < SPEED_MINIMUM)
      {
        rightSpeed = SPEED_MINIMUM;
      }
    if (rightSpeed < rightDirection)
    {
      rightSpeed = rightSpeed + acceleration;
    }
    if (rightSpeed > rightDirection)
    {
      rightSpeed = rightDirection;
    }
  }
  else if (rightDirection < -SPEED_MINIMUM)
  {
    if(rightSpeed > -SPEED_MINIMUM)
      {
        rightSpeed = -SPEED_MINIMUM;
      }
    if (rightSpeed > rightDirection)
    {
      rightSpeed = rightSpeed - acceleration;
    }
    if (rightSpeed < rightDirection)
    {
      rightSpeed = rightDirection;   
    }
  }
  else
  {
    if (rightSpeed > SPEED_MINIMUM)
    {
      rightSpeed = rightSpeed - decceleration;
      if (rightSpeed < 0)
      {
        rightSpeed = 0;
      }
    }
    else if (rightSpeed < -SPEED_MINIMUM)
    {
      rightSpeed = rightSpeed + decceleration;
      if (rightSpeed > 0)
      {
        rightSpeed = 0;
      }
    }
    else
    {
      rightSpeed = 0;
    }
  }

  if (rightSpeed > SPEED_MAXIMUM)
  {
    rightSpeed = SPEED_MAXIMUM;
  }
  if (rightSpeed < -SPEED_MAXIMUM)
  {
    rightSpeed = -SPEED_MAXIMUM;
  }

  if (rightSpeed > SPEED_MINIMUM)
  {
    analogWrite(enB, (int)rightSpeed);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } 
  else if (rightSpeed < -SPEED_MINIMUM)
  {
    tempInt = abs((int)rightSpeed);
    analogWrite(enB, tempInt);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  else
  {
    analogWrite(enB, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }  
  Serial.print("left dir: ");
  Serial.print(leftDirection);
  Serial.print("   left speed: ");
  Serial.println(leftSpeed);
  
  Serial.print("right dir: ");
  Serial.print(rightDirection);
  Serial.print("   right speed: ");
  Serial.println(rightSpeed);
  
}

void manipulatorControl()
{
  SERVO_LR_POSITION = SERVO_LR_POSITION + SERVO_LR_DIRECTION;
  if (SERVO_LR_POSITION > 135)
  {
    SERVO_LR_POSITION = 135;
  }
  if (SERVO_LR_POSITION < 0)
  {
    SERVO_LR_POSITION = 0;
  }
  SERVO_UD_POSITION = SERVO_UD_POSITION + SERVO_UD_DIRECTION;
  if (SERVO_UD_POSITION < 90)
  {
    SERVO_UD_POSITION = 90;
  }
  if (SERVO_UD_POSITION > 180)
  {
    SERVO_UD_POSITION = 180;
  }
  servoLR.write((int)SERVO_LR_POSITION);
  servoUD.write((int)SERVO_UD_POSITION);
  if (SERVO_OC_FLAG)
  {
    servoOC.write(0);  
  }
  else 
  {
    servoOC.write(180);  
  }
}

void parseMsg()
{
  if (incomingMsg[0] != 'C')
  {
    return;
  }
  if (incomingMsg[1] != '9' && incomingMsg[1] != '8')
  {
    return;
  }
  if (incomingMsg[5] != '9' && incomingMsg[5] != '8')
  {
    return;
  }
  if (incomingMsg[9] != '0' && incomingMsg[9] != '1')
  {
    return;
  }
  if (incomingMsg[12] != 'S' && incomingMsg[12] != 'P')
  {
    return;
  }
  Serial.println(incomingMsg);  
  parseMotorInput();
  parseManipulatorInput();
}

void parseMotorInput()
{
  leftDirection = 0;
  rightDirection = 0;

  int tempInt = 0;
  char tempChar[4];
  tempChar[0] = incomingMsg[2];
  tempChar[1] = incomingMsg[3];
  tempChar[2] = incomingMsg[4];
  tempChar[3] = NULL;
  tempInt = atoi(tempChar);

  if (incomingMsg[1] == '9')
  {
    leftDirection = leftDirection + tempInt;
    rightDirection = rightDirection + tempInt;
  }
  else if (incomingMsg[1] == '8')
  {
    leftDirection = leftDirection - tempInt;
    rightDirection = rightDirection - tempInt;
  }

  tempInt = 0;
  tempChar[0] = NULL;
  tempChar[0] = incomingMsg[6];
  tempChar[1] = incomingMsg[7];
  tempChar[2] = incomingMsg[8];
  tempChar[3] = NULL;
  tempInt = atoi(tempChar);

  if (incomingMsg[5] == '9')
  {
    leftDirection = leftDirection + tempInt;
    rightDirection = rightDirection - tempInt;
  }
  else if (incomingMsg[5] == '8')
  {
    leftDirection = leftDirection - tempInt;
    rightDirection = rightDirection + tempInt;
  }

  if (leftDirection > 240)
  {
    leftDirection = 240;
  }
  else if (leftDirection < -240)
  {
    leftDirection = -240;
  }
  else if (-MOTOR_INPUT_MINIMUM < leftDirection && leftDirection < MOTOR_INPUT_MINIMUM)
  {
    leftDirection = 0;
  }

  if (rightDirection > 240)
  {
    rightDirection = 240;
  }
  else if (rightDirection < -240)
  {
    rightDirection = -240;
  }
  else if (-MOTOR_INPUT_MINIMUM < rightDirection && rightDirection < MOTOR_INPUT_MINIMUM)
  {
    rightDirection = 0;
  }  
}

void parseManipulatorInput()
{
  SERVO_LR_DIRECTION = 0;
  SERVO_UD_DIRECTION = 0;

  if (incomingMsg[10] == 'L') 
  {
    SERVO_LR_DIRECTION = MANIPULATOR_ACCELERATION;
  }
  if (incomingMsg[10] == 'R') 
  {
    SERVO_LR_DIRECTION = -MANIPULATOR_ACCELERATION;
  }
  if (incomingMsg[10] == 'S') 
  {
    SERVO_LR_DIRECTION = 0;
  }
  if (incomingMsg[11] == 'U') 
  {
    SERVO_UD_DIRECTION = -MANIPULATOR_ACCELERATION;
  }
  if (incomingMsg[11] == 'D') 
  {
    SERVO_UD_DIRECTION = MANIPULATOR_ACCELERATION;
  }
  if (incomingMsg[11] == 'S') 
  {
    SERVO_UD_DIRECTION = 0;
  }
  if (incomingMsg[12] == 'P') 
  {
    if (millis() - SERVO_BUTTON_TIMER > SERVO_BUTTON_DELAY)
    {
      SERVO_OC_FLAG = !SERVO_OC_FLAG;
      SERVO_BUTTON_TIMER = millis();
    }
  }
}

void getSignal()
{
  if (Serial.available() > 0)
  {
    readSerial();
  }
  if (massageIsReady)
  {
    parseMsg();
    moveControler();
    manipulatorControl();
    massageIsReady = false;
    incomingMsg[0] = '\0';
  }
}

void sendSignal()
{
}

void loop()
{
  //distance1 = ultrasonic1.read();
  //distance2 = ultrasonic2.read();

  //sendSignal();
  getSignal();
  //Serial.print(leftDirection);Serial.print("  ");Serial.println(rightDirection);
}
