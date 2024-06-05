#include <TimerOne.h>
#include <DFRobotHighTemperatureSensor.h>

int SlideSensor_PIN = 2;
int TurnSensor_PIN = 3;
int STEP_Turn_PIN = 4;
int DIR_Turn_PIN = 5;
int STEP_Add_PIN = 6;
int DIR_Add_PIN = 7;
int STEP_Slide_PIN = 8;
int DIR_Slide_PIN = 9;
int BEEP_PIN = 10;
int Off_Switch_PIN = 11;
int On_Switch_PIN = 12;
int On_Off_PIN = 13;
int TEMP_PIN = A0;
const float voltageRef = 3.300;
//const float voltageRef = 5.000;
DFRobotHighTemperature PT100 = DFRobotHighTemperature(voltageRef);

void setup()
{
  Serial.begin(9600);
  pinMode(STEP_Slide_PIN, OUTPUT);
  pinMode(STEP_Turn_PIN, OUTPUT);
  pinMode(STEP_Add_PIN, OUTPUT);
  pinMode(DIR_Slide_PIN, OUTPUT);
  pinMode(DIR_Turn_PIN, OUTPUT);
  pinMode(DIR_Add_PIN, OUTPUT);
  pinMode(SlideSensor_PIN, OUTPUT);
  pinMode(TurnSensor_PIN, OUTPUT);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(On_Switch_PIN, OUTPUT);
  pinMode(Off_Switch_PIN, OUTPUT);
  pinMode(On_Off_PIN, OUTPUT);
  digitalWrite(TurnSensor_PIN, LOW);
  digitalWrite(SlideSensor_PIN, LOW);
  digitalWrite(STEP_Slide_PIN, LOW);
  digitalWrite(STEP_Turn_PIN, LOW);
  digitalWrite(STEP_Add_PIN, LOW);
  digitalWrite(DIR_Slide_PIN, LOW);
  digitalWrite(DIR_Turn_PIN, LOW);
  digitalWrite(DIR_Add_PIN, LOW);
  digitalWrite(BEEP_PIN, LOW);
  digitalWrite(On_Switch_PIN, LOW);
  digitalWrite(Off_Switch_PIN, LOW);
  digitalWrite(On_Off_PIN, LOW);
  Timer1.initialize(10000);
  Timer1.attachInterrupt(Temp_Con);
}

static int Turn_Flag = 2;
static int Switch_Flag = 0;
static int TEMP = 0;
bool valid_cmd = false;
char cmd[10];

void loop()
{
  if (valid_cmd) //WiFi控制部分
  {
    if (strncmp(cmd, "on", 2) == 0)
    {
      Switch_Flag = 1;
      digitalWrite(On_Off_PIN, HIGH);
      Serial.println("Already on");
      Re();
    }
    else if (strncmp(cmd, "start", 5) == 0)
    {
      Switch_Flag = 0;
      Serial.println("Already started");
      Re();
      Fry();
      Music();
      Serial.println("Decocting completed");
      digitalWrite(On_Off_PIN, LOW);
    }
    else if (strncmp(cmd, "zero", 4) == 0)
    { 
      Serial.println("Start zeroing");
      Slide_Zero();
      Turn_Zero();
      Serial.println("Zero complete");
    }
    else
      Serial.println("Command not found");
    valid_cmd = false;
  }
  
  if(digitalRead(On_Switch_PIN)==HIGH) //按键控制部分
  {
    if(Switch_Flag==0) //开启电源
    {
      Switch_Flag=1;
      digitalWrite(On_Off_PIN,HIGH);
      Re();
      while(digitalRead(On_Switch_PIN));
      digitalWrite(On_Switch_PIN,LOW);
    }
    else if(Switch_Flag==1) //开始运行
    {
      Switch_Flag=0;
      Re();
      while(digitalRead(On_Switch_PIN));
      digitalWrite(On_Switch_PIN,LOW);
      Fry();
      Music();
      digitalWrite(On_Off_PIN,LOW);
    }
  }
}

void(* ResetFunc) (void) = 0;

void Temp_Con()
{
  /*TEMP=PT100.readTemperature(TEMP_PIN);
  Serial.print("temp: ");
  Serial.print(TEMP);
  Serial.println("^C");*/
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 10; i++)
      cmd[i] = '\0';
    for (int i = 0; i < 9; i++)
    {
      if (Serial.available() > 0)
      {
        cmd[i] = Serial.read();
        delay(1);
      }
      else
        break;
    }
    valid_cmd = true;
  }
  if(valid_cmd)
  {
    if (strncmp(cmd, "off", 3) == 0)
    {
      Re();
      Slide_Zero();
      digitalWrite(On_Off_PIN,LOW);
      digitalWrite(Off_Switch_PIN,LOW);
      for (int i = 0; i < 10; i++)
        cmd[i] = '\0';
      Serial.println("Already off");
      ResetFunc();
    }
  }
  if(digitalRead(Off_Switch_PIN)==HIGH)
  {
      Re();
      Slide_Zero();
      digitalWrite(On_Off_PIN,LOW);
      digitalWrite(Off_Switch_PIN,LOW);
      ResetFunc();
  }
}

void Slide_Zero()
{
  if(digitalRead(SlideSensor_PIN)==HIGH)
    return;
  else
  {
    digitalWrite(DIR_Slide_PIN,HIGH);//HIGH 滑台向前移动
    while(digitalRead(SlideSensor_PIN)!=HIGH)
    {
      digitalWrite(STEP_Slide_PIN,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP_Slide_PIN,LOW);
      delayMicroseconds(100);
     }
    for(int i=0;i<16*20;i++)  //距离修正
    {
      digitalWrite(STEP_Slide_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Slide_PIN,LOW);
      delayMicroseconds(1000);      
    }
  }
  delay(500);
}

void Turn_Zero()
{
  digitalWrite(DIR_Turn_PIN,LOW); //LOW 朝前转
  if(digitalRead(TurnSensor_PIN)==HIGH)
    return;
  else
  {
    while(digitalRead(TurnSensor_PIN)!=HIGH)
    {
      digitalWrite(STEP_Turn_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Turn_PIN,LOW);
      delayMicroseconds(1000);
    }
    for(int i=0;i<16*16;i++)  //角度修正
    {
      digitalWrite(STEP_Turn_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Turn_PIN,LOW);
      delayMicroseconds(1000);
    }
  }
  Turn_Flag=1;
  delay(500);
}

void Add()
{
  digitalWrite(DIR_Add_PIN,HIGH);
  for(int j=0;j<30;j++)
    for(int i=0;i<3200;i++)
    {
      digitalWrite(STEP_Add_PIN,HIGH);
      delayMicroseconds(50);
      digitalWrite(STEP_Add_PIN,LOW);
      delayMicroseconds(50);
    }
  delay(500);
  digitalWrite(DIR_Add_PIN,LOW);
  for(int k=0;k<3200*5;k++)
  {
    digitalWrite(STEP_Add_PIN,HIGH);
    delayMicroseconds(50);
    digitalWrite(STEP_Add_PIN,LOW);
    delayMicroseconds(50);
  }
  delay(500);
}

void Turn_deg(double deg)
{
  if(deg>0)
  {
    digitalWrite(DIR_Turn_PIN,HIGH); //HIGH 正转角度即向后转角度
    for(int i=0;i<deg/360*3200;i++)
    {
      digitalWrite(STEP_Turn_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Turn_PIN,LOW);
      delayMicroseconds(1000);
    }
  }
  else
  {
    digitalWrite(DIR_Turn_PIN,LOW);//LOW 反转角度即向前转角度
    for(int i=0;i<-(deg/360*3200);i++)
    {
      digitalWrite(STEP_Turn_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Turn_PIN,LOW);
      delayMicroseconds(1000);
    }
  }
  delay(500);
}

void Goto_Turn1()
{
  digitalWrite(DIR_Slide_PIN,HIGH);
  for(int i=0;i<14;i++)
    for(int j=0;j<3200*1;j++)
    {
      digitalWrite(STEP_Slide_PIN,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP_Slide_PIN,LOW);
      delayMicroseconds(100);
    }
}

void Goto_Turn2()
{
  Slide_Zero();
}

void Goto_Fry1()
{
  digitalWrite(DIR_Slide_PIN,LOW);//LOW 滑台向后移动
  for(int j=0;j<23;j++)
    for(int i=0;i<3200*1;i++)
    {
      digitalWrite(STEP_Slide_PIN,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP_Slide_PIN,LOW);
      delayMicroseconds(100);
    }
  delay(500);
}

void Goto_Fry2()
{
  digitalWrite(DIR_Slide_PIN,LOW);//LOW 滑台向后移动
  for(int j=0;j<8;j++)
    for(int i=0;i<3200*2;i++)
    {
      digitalWrite(STEP_Slide_PIN,HIGH);
      delayMicroseconds(100);
      digitalWrite(STEP_Slide_PIN,LOW);
      delayMicroseconds(100);
    }
  delay(500);
}

void Fry()
{
  Slide_Zero();
  Turn_Zero();
  do
  {
    TEMP=PT100.readTemperature(TEMP_PIN);
  }while(TEMP<60);//原来230
  Add();
  Turn_deg(180);
  Goto_Fry1();
  for(int i=0;i<300;i++)
    //delay(1000);
    delay(10);
  Goto_Turn1();
  Add();
  Goto_Turn2();
  Turn_deg(-180);
  Goto_Fry2();
  for(int i=0;i<300;i++)
    //delay(1000);
    delay(10);
  Goto_Turn2();
  Turn_deg(-30);
}

void Try_Fry()
{
  if(digitalRead(SlideSensor_PIN)&&digitalRead(TurnSensor_PIN))
    Goto_Fry1();
  else if(digitalRead(SlideSensor_PIN)!=HIGH)
  {
    if(!(Turn_Flag==0||Turn_Flag==1))
    {
      Do();
      Re();
      Do();
      Re();
      Do();
      Re();
    }
    else
    {
      Goto_Turn2();
      if(Turn_Flag==1)
      { 
        Turn_deg(180);
        Turn_Flag=0;
      }
      else if(Turn_Flag==0)
      {
        Turn_deg(-180);
        Turn_Flag=1;
      }
      Goto_Fry1();
    }
  }  
}

void Do()
{
  for(int i=0;i<200;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(567);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(567);
  }
  delay(200);
}

void Re()
{
  for(int i=0;i<224;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(505);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(505);
  }
  delay(200);
}

void Mi()
{
  for(int i=0;i<252;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(449);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(449);
  }
  delay(200);
}

void Fa()
{
  for(int i=0;i<266;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(424);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(424);
  }
  delay(200);
}

void So()
{
  for(int i=0;i<299;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(378);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(378);
  }
  delay(200);
}

void La()
{
  for(int i=0;i<336;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(336);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(336);
  }
  delay(200);
}

void Xi()
{
  for(int i=0;i<377;i++)
  {
    digitalWrite(BEEP_PIN,LOW);
    delayMicroseconds(300);
    digitalWrite(BEEP_PIN,HIGH);
    delayMicroseconds(300);
  }
  delay(200);
}

void Music()
{
  Mi();
  Re();
  Do();
  Re();
  Mi();
  Mi();
  Mi();
  Re();
  Re();
  Re();
  Mi();
  Mi();
  Mi();
  Mi();
  Re();
  Do();
  Re();
  Mi();
  Mi();
  Mi();
  Re();
  Re();
  Mi();
  Re();
  Do();
  delay(1000);
}
