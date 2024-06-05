#include <Wire.h>
//#include <RSCG12864B.h>
//#include <DFRobotHighTemperatureSensor.h>
//#include "TimerOne.h"
#define I2C_ADDR   0x79
#define ASR_RESULT_ADDR           100
#define ASR_WORDS_ERASE_ADDR      101
#define ASR_MODE_ADDR             102
#define ASR_ADD_WORDS_ADDR        160

int TurnSensor_PIN=2;
int SlideSensor_PIN=3;
int STEP_Turn_PIN=4;
int DIR_Turn_PIN=5;
int STEP_Add_PIN=6;
int DIR_Add_PIN=7;
int STEP_Slide_PIN=8;
int DIR_Slide_PIN=9;
int BEEP_PIN=10;
int On_Switch_PIN=11;
int Off_Switch_PIN=12;
int On_Off_PIN=13;
//int GotoZero_PIN=10;
//int TMPC_PIN=12;
//RAYLIDLCD myLCD(busyPin);
//const int busyPin=10;
//int Fry_PIN=11;
//int ALL_PIN=13;
//int TEMP_PIN=A0;
//const float voltageRef=3.300;
//const float voltageRef=5.000;
//DFRobotHighTemperature PT100=DFRobotHighTemperature(voltageRef);

void setup()
{
  Serial.begin(9600);
  //myLCD.begin();
  //myLCD.setBrightness(255);
  uint8_t ASRMode = 2;//1：循环识别模式    2：口令模式，以第一个词条为口令    3按键模式，按下开始识别
  Wire.begin();
#if 1   //添加的词条和识别模式是可以掉电保存的，第一次设置完成后，可以将此段注释掉，即将1改为0，然后重新下载一次程序
  WireWriteDataArray(ASR_WORDS_ERASE_ADDR,NULL,0);
  delay(60);//擦除需要一定的时间
  ASRAddWords(1,"xiao niu tong xue");   //小牛同学
  ASRAddWords(2,"kai shi gui ling");    //开始归零
  ASRAddWords(3,"kai shi jia re");      //开始加热
  ASRAddWords(4,"ting zhi jia re");     //停止加热
  ASRAddWords(5,"qu jia re");           //去加热
  ASRAddWords(6,"zhi xing zheng ge liu cheng");  //执行整个流程
  ASRAddWords(7,"guan ji");             //关机
  if(WireWriteDataArray(ASR_MODE_ADDR,&ASRMode,1))
    Serial.println("ASR Module Initialization complete");
  else
    Serial.println("ASR Module Initialization fail");
#endif
  Serial.println("Start");
  pinMode(STEP_Slide_PIN,OUTPUT);
  pinMode(STEP_Turn_PIN,OUTPUT);
  pinMode(STEP_Add_PIN,OUTPUT);
  pinMode(DIR_Slide_PIN,OUTPUT);
  pinMode(DIR_Turn_PIN,OUTPUT);
  pinMode(DIR_Add_PIN,OUTPUT);
  pinMode(SlideSensor_PIN,OUTPUT);
  pinMode(TurnSensor_PIN,OUTPUT);
  pinMode(BEEP_PIN,OUTPUT);
  pinMode(On_Switch_PIN,OUTPUT);
  pinMode(Off_Switch_PIN,OUTPUT);
  pinMode(On_Off_PIN,OUTPUT);
  //pinMode(GotoZero_PIN,OUTPUT);
  //pinMode(TMPC_PIN,OUTPUT);
  //pinMode(Fry_PIN,OUTPUT);
  //pinMode(ALL_PIN,OUTPUT);
  digitalWrite(STEP_Slide_PIN,LOW);
  digitalWrite(STEP_Turn_PIN,LOW);
  digitalWrite(STEP_Add_PIN,LOW);
  digitalWrite(DIR_Slide_PIN,LOW);
  digitalWrite(DIR_Turn_PIN,LOW);
  digitalWrite(DIR_Add_PIN,LOW);
  digitalWrite(BEEP_PIN,LOW);
  digitalWrite(On_Switch_PIN,LOW);
  digitalWrite(Off_Switch_PIN,LOW);
  digitalWrite(On_Off_PIN,LOW);
  //digitalWrite(GotoZero_PIN,LOW);
  //digitalWrite(TMPC_PIN,LOW);
  //digitalWrite(Fry_PIN,LOW);
  //digitalWrite(ALL_PIN,LOW);
  //Timer1.initialize(500000);
  //Timer1.attachInterrupt(Temp_Con);
}

static int Run_Flag=2;
static int Switch_Flag=0;
//static int TEMP=0;
//static int start_Heat=0;
void loop()
{
  unsigned char result;
  delay(1);
  WireReadDataArray(ASR_RESULT_ADDR,&result,1);
  if(digitalRead(On_Switch_PIN)==HIGH)
  {
    if(Switch_Flag==0)
    {
      Switch_Flag=1;
      digitalWrite(On_Off_PIN,HIGH);
      digitalWrite(On_Switch_PIN,LOW);
    }
    else if(Switch_Flag==1)
    {
      Switch_Flag=0;
      digitalWrite(On_Switch_PIN,LOW);
      
    }
  }
  else if(digitalRead(Off_Switch_PIN)==HIGH)
  {
    digitalWrite(On_Off_PIN,LOW);
    digitalWrite(Off_Switch_PIN,LOW);
  }
  if(result)
  {
    Serial.print("ASR result is:");
    Serial.println(result);//返回识别结果，即识别到的词条编号
    switch(result)
    {
      case 1:
        Re();
        result=0;
        break;
      case 2:
        Re();
        result=0;
        Slide_Zero();
        Turn_Zero();
        Run_Flag=1;
        break;
      case 3:
        Re();
        result=0;
        //start_Heat=1;
        break;
      case 4:
        Re();
        result=0;
        //start_Heat=0;
        break;
      case 5:
        Re();
        result=0;
        Try_Fry();
        break;
      case 6:
        Re();
        result=0;
        Fry();
        break;
      case 7:        
        Re();
        result=0;
        digitalWrite(On_Off_PIN,LOW);
        break;
      default:
        result=0;
        break;
    }
   }
  /*if(digitalRead(GotoZero_PIN)==HIGH)
  {
    Slide_Zero();
    Turn_Zero();
    flag=1;
  }
  else if(digitalRead(Fry_PIN)==HIGH)
  {
    Turn_deg(45);
    /*if(digitalRead(SlideSensor_PIN)&&digitalRead(TurnSensor_PIN))
      Goto_Fry1();
    else if(digitalRead(SlideSensor_PIN)!=HIGH)
    {
      if(!(flag==0||flag==1));
      else
      {
        Goto_Turn2();
        if(flag==1)
        { 
          Turn_deg(180);
          flag=0;
        }
        else
        {
          Turn_deg(-180);
          flag=1;
        }
        Goto_Fry1();
      }
    }*
  }
  else if(digitalRead(ALL_PIN)==HIGH)
  {
    Slide_Zero();
    Turn_Zero();
    do
    {
      TEMP=PT100.readTemperature(TEMP_PIN);
    }while(TEMP<240);
    Add();
    Turn_deg(180);
    Goto_Fry1();
    for(int i=0;i<300;i++)
      delay(1000);
      //delay(10);
    Goto_Turn1();
    Add();
    Goto_Turn2();
    Turn_deg(-180);
    Goto_Fry2();
    for(int i=0;i<300;i++)
      delay(1000);
      //delay(10);
    Goto_Turn2();
    Turn_deg(-30);
  }
  Add();*/
}
/*void Temp_Con()
{
  int TEMP=PT100.readTemperature(TEMP_PIN);
  myLCD.clear();
  myLCD.print(0, 15, TEMP, VLARGE);
  /*Serial.print("temp: ");
  Serial.print(TEMP);
  Serial.println("^C");*
  if(start_Heat=1)
  {
    if(TEMP>255)   //设置煎制温度范围为240-255度
      digitalWrite(TMPC_PIN,LOW);
    else if(TEMP<240)
      digitalWrite(TMPC_PIN,HIGH);
  }
  else
    digitalWrite(TMPC_PIN,LOW);
}*/

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
    for(int i=0;i<16*19;i++)  //角度修正
    {
      digitalWrite(STEP_Turn_PIN,HIGH);
      delayMicroseconds(1000);
      digitalWrite(STEP_Turn_PIN,LOW);
      delayMicroseconds(1000);
    }
  }
  delay(500);
}

void Add()
{
  digitalWrite(DIR_Add_PIN,LOW);
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
  //Slide_Zero();
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
  /*do
  {
    TEMP=PT100.readTemperature(TEMP_PIN);
  }while(TEMP<240);*/
  Add();
  Turn_deg(180);
  Goto_Fry1();
  for(int i=0;i<300;i++)
    delay(1000);
    //delay(10);
  Goto_Turn1();
  Add();
  Goto_Turn2();
  Turn_deg(-180);
  Goto_Fry2();
  for(int i=0;i<300;i++)
    delay(1000);
    //delay(10);
  Goto_Turn2();
  Turn_deg(-30);
}

void Try_Fry()
{
  if(digitalRead(SlideSensor_PIN)&&digitalRead(TurnSensor_PIN))
    Goto_Fry1();
  else if(digitalRead(SlideSensor_PIN)!=HIGH)
  {
    if(!(Run_Flag==0||Run_Flag==1));
    else
    {
      Goto_Turn2();
      if(Run_Flag==1)
      { 
        Turn_deg(180);
        Run_Flag=0;
      }
      else
      {
        Turn_deg(-180);
        Run_Flag=1;
      }
      Goto_Fry1();
    }
  }  
}

bool WireWriteByte(uint8_t val)
{
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(val);
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    return true;
}

bool WireWriteDataArray(  uint8_t reg,uint8_t *val,unsigned int len)
{
    unsigned int i;

    Wire.beginTransmission(I2C_ADDR);
    Wire.write(reg);
    for(i = 0; i < len; i++) {
        Wire.write(val[i]);
    }
    if( Wire.endTransmission() != 0 ) {
        return false;
    }
    return true;
}

int WireReadDataArray(   uint8_t reg, uint8_t *val, unsigned int len)
{
    unsigned char i = 0;  
    if (!WireWriteByte(reg)) {
        return -1;
    }
    Wire.requestFrom(I2C_ADDR, len);
    while (Wire.available()) {
        if (i >= len) {
            return -1;
        }
        val[i] = Wire.read();
        i++;
    }
    return i;
}

bool ASRAddWords(unsigned char idNum,unsigned char *words)
{
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(ASR_ADD_WORDS_ADDR);
  Wire.write(idNum);
  Wire.write(words,strlen(words));
  if( Wire.endTransmission() != 0 ) {
        delay(10);
        return false;
    }
    delay(10);
    return true;
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
