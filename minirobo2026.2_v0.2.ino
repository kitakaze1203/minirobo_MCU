#include <SoftwareSerial.h>

//UART用
SoftwareSerial nanoSerial(A0, A1);
String uart_data = "";
char buf[256];
int i = 0;
int data[256];

//機構モーター用
int m1 = 3;
int m2 = 5;
int m3 = 6;
int m4 = 9;
int m5 = 10;
int m6 = 11;
int dir_m1 = 2;
int dir_m2 = 4;
int dir_m3 = 7;
int dir_m4 = 8;
int dir_m5 = 12;
int air_cylinder = 13;

int test_pwm = PWM_POWER;//仮のモーター出力

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
  //UART用
  nanoSerial.begin(9600);
  delay(100);
  Serial.println("UART_begin");
  
  //ピン
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(m5,OUTPUT);
  pinMode(dir_m1,OUTPUT);
  pinMode(dir_m2,OUTPUT);
  pinMode(dir_m3,OUTPUT);
  pinMode(dir_m4,OUTPUT);
  pinMode(dir_m5,OUTPUT);

  pinMode(air_cylinder,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  //UARTを受信
  if(nanoSerial.available())
  {
    uart_data = nanoSerial.readStringUntil('\n');
    uart_data.trim();

    //bufの初期化
    i = 0;
    while (i < 256)
    {
      buf[i] = 0;
     i++;
    }

    //Stringからchar配列に変換
    uart_data.toCharArray(buf, 256);

    //先頭の1データをtokenに分割
    char *token = strtok(buf,",");

    i = 0;
    while(token != NULL){
      //データをintに変換してdataに代入
      data[i] = atoi(token);
      i++;
      //前回の探索の続きからtokenに分割
      token = strtok(NULL,",");
    }

    Serial.print("0:");
    Serial.println(data[3]);
    Serial.print("1:");
    Serial.println(data[4]);
    Serial.print("2:");
    Serial.println(data[5]);
    Serial.print("3:");
    Serial.println(data[6]);
    Serial.print("4:");
    Serial.println(data[7]);
    Serial.print("5:");
    Serial.println(data[8]);
  }
  
  motor(m1,dir_m1,data[3]);
  motor(m2,dir_m2,data[4]);
  motor(m3,dir_m3,data[5]);
  motor(m4,dir_m4,data[6]);
  motor(m5,dir_m5,data[7]);

  if(data[8] == 1){
    digitalWrite(air_cylinder,HIGH);
  }else{
    digitalWrite(air_cylinder,LOW);
  }
}

//モーター制御用関数 pwm出力のピン　dir出力のピン　pwm出力に使う値
void motor(int pwm_pin,int dir_pin,int pwm){
  if(pwm >= 0){
    digitalWrite(dir_pin,LOW);
  }else{
    digitalWrite(dir_pin,HIGH);
    pwm *= -1;
  }
  analogWrite(pwm_pin,pwm);
}