#include <Arduino.h>
#include <DHT.h>
#include <SoftwareSerial.h>
//定义DHT11针脚
#define DHTPIN 13 
//定义类型，DHT11或者其它
#define DHTTYPE DHT11
#define ledPin 13       
#define hallPin 5 

// 超声测距模块初始化参数
int trigPin = 11;    //Trig
int echoPin = 12;    //Echo
long duration, cm, inches;
// 初始化DHT11
DHT dht(DHTPIN, DHTTYPE);
// HC-SR501 红外热释电 初始化参
int irSensor = 2;
// 建立SoftwareSerial对象，RX引脚2, TX引脚3
SoftwareSerial BTserial(8, 9); 
 

void setup() {
  // 设置
  // 初始化串口
  Serial.begin (9600);
  // 定义超声测距传感器的输入与输出
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin,OUTPUT); 
  // 设置红外热释电传感器
  pinMode(LED_BUILTIN, OUTPUT); //连接内置LED的引脚设置为输出模式
  pinMode(irSensor, INPUT);     //连接人体红外感应模块的OUT引脚设置为输入模式
  dht.begin(); //DHT开始工作

   // CC2541
  //  Serial.begin(9600);
  //  BTserial.begin(9600); // HC-06 默认波特率 9600
 
  //  Serial.println("HC-06 DEMO/TEST  ");
  
  //   BTserial.print("AT\r\n");   //可在此处输入设置HC-06蓝牙模块的AT指令。
  //                               //此AT指令须在HC-06未连接蓝牙状态输入。

  //   BTserial.print("AT+BAUD\r\n4");//设置蓝牙通信波特率
  //   delay(100);
  //   BTserial.print("AT+ROLE=M\r\n");//设置蓝牙为主机
  //   delay(100);
  //   BTserial.print("AT+NAMEJZDmaster\r\n");
  //   delay(100);//设置蓝牙名称JZDmaster
  //   BTserial.print("AT+PIN123456\r\n");//设置密码为123456
  //   delay(100);                    
}


// 报警函数
void Warn(int Distance, bool InfraredPyroelectric) {
  if (Distance < 50 && InfraredPyroelectric)
  {
    Serial.print("\n!! Warning! Creatures Approaching !!\n");
  } else if (Distance < 50 && !(InfraredPyroelectric)){
    Serial.print("\n!! Warning! Foreign Body Approaching !!\n");
  } else if (Distance >=50 && InfraredPyroelectric){
    Serial.print("\nPay attention to creatures nearby.\n");
  } else if (Distance >=50 && !InfraredPyroelectric){
    Serial.print("\nNormal.\n");
  }
  
}

void loop() {
  Serial.print("================================================================\n");
  /**
   * 红外热释电模块
   */
  bool sensorReading_h = digitalRead(irSensor);  //建立变量存储感应模块的输出信号
  if ( sensorReading_h ) {
    digitalWrite(LED_BUILTIN, HIGH);   // 模块感应到人.输出高电平.点亮LED并输出信息
    Serial.print("Nothing is approching.\n");
  }
  else {
    digitalWrite(LED_BUILTIN, LOW); 
    Serial.print("Something is approching.\n"); //  无人状态保持LED关闭,输出信息
  }

/**
 * 超声测距
 */
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // 从传感器读取信号:一个高脉冲
  // 持续时间是发送开始的时间(以微秒为单位)
  duration = pulseIn(echoPin, HIGH);
 
  // 把时间转换成距离
  cm = (duration/2) / 29.1;
  inches = (duration/2) / 74; 
  
  Serial.print("\nDistance is:");
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Warn(cm, sensorReading_h);
/**
 * 温湿度
 */
  // 读温度或湿度要用250毫秒
  float h = dht.readHumidity();//读湿度
  float t = dht.readTemperature();//读温度，默认为摄氏度
  Serial.print("\nEnvironment Parameter is: \nHumidity: ");//湿度
  Serial.println(h);
  Serial.print("Temperature: ");//温度
  Serial.print(t);
  Serial.println(" ℃ \n");

  /**
   * 霍尔传感器
   */
    if(digitalRead(hallPin) == LOW) {
      digitalWrite(ledPin,HIGH); 
      Serial.print("!! Abnormal magnetic field detected !!\n");
    }
  else { 
    digitalWrite(ledPin,LOW); 
    Serial.print("No abnormal magnetic field was detected.\n");
  }
  Serial.print("================================================================\n\n");
  delay(2000);
}