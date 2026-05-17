#include <Wire.h>
#include <SoftwareSerial.h>

//UART用
SoftwareSerial nanoSerial(A0, A1);

//I2C用
char receivedData[256];           
char sendData[256];
int data[5];
int i = 0;
volatile bool newData =false;
//モーター用
int p_fr = 3;
int p_fl = 9;
int p_rr = 10;
int p_rl = 11;
int dir_fr = 2;
int dir_fl = 4;
int dir_rr = 7;
int dir_rl = 8;

int stick_lx = 0;
int stick_ly = 0;
int stick_rx = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial_begin");
  //UART用
  nanoSerial.begin(9600);
  delay(100);
  Serial.println("UART_begin");

  //I2C設定
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Serial.println("Wire_begin");
  //PINとか
  //足回り
  pinMode(p_fr,OUTPUT);
  pinMode(p_fl,OUTPUT);
  pinMode(p_rr,OUTPUT);
  pinMode(p_rl,OUTPUT);
  pinMode(dir_fr,OUTPUT);
  pinMode(dir_fl,OUTPUT);
  pinMode(dir_rr,OUTPUT);
  pinMode(dir_rl,OUTPUT);
  Serial.println("pin_set");  

}

void loop() {
  // put your main code here, to run repeatedly:
  if(newData)
  {
    newData = false;
    nanoSerial.println(sendData);
    Serial.print("lx:");
    Serial.println(stick_lx);
    Serial.print("ly:");
    Serial.println(stick_ly);
    Serial.print("rx:");
    Serial.println(stick_rx);
  }
  //UART送信
  

  motor(p_fr,dir_fr,stick_ly + stick_lx + stick_rx);
  motor(p_fl,dir_fl,stick_ly - stick_lx - stick_rx);
  motor(p_rr,dir_rr,stick_ly - stick_lx + stick_rx);
  motor(p_rl,dir_rl,stick_ly + stick_lx - stick_rx);
}

void receiveEvent(int rec_data)
{
  int i = 0;
  for(i=0;i<256;i++)
  {
    receivedData[i] = 0;
    sendData[i] = 0;
  }
  i=0;
  while(Wire.available() && i < sizeof(receivedData) - 1)
  {
    receivedData[i] = Wire.read();
    sendData[i] = receivedData[i];
    i++;
  }
  
  char *token = strtok(receivedData,",");

  i = 0;
  while(token != NULL)
  {
    //データをintに変換してdataに代入
    
    if(i < 3)
    {
      data[i] = atoi(token);
      i++;
      //前回の探索の続きからtokenに分解
      token = strtok(NULL,",");
    }
    else 
    {
      break;
    }
  }
  stick_lx = data[0];
  stick_ly = data[1];
  stick_rx = data[2];
  newData = true;
}

//モーター制御用関数 pwm出力のピン　dir出力のピン　pwm出力に使う値
void motor(int pwm_pin,int dir_pin,int pwm)
{
  if(pwm >= 0){
    digitalWrite(dir_pin,LOW);
  }else{
    digitalWrite(dir_pin,HIGH);
    pwm *= -1;
  }
  analogWrite(pwm_pin,pwm);
}