//R2-D2 triple axis Accelerometer and BB-8 LED Light for cx-3
//2016/7/14 BlueDK5FW

#include <Servo.h>
#define WAKEUP 3 //起動音再生の出力ピン番号
#define WORRY 4 //心配音再生の出力ピン番号
#define WORRY2 5 //心配音2再生の出力ピン番号
#define SERVO 7 //Servoの(入)出力ピン番号
#define REDLED 9 //RedLEDの出力ピン番号
#define BB8LED 12 //BB8LEDの出力ピン番号
#define BLUELED 13 //BlueLEDの出力ピン番号
#define ACCX 3 //加速度センサーX軸の入力ピン番号
#define ACCY 5 //加速度センサーY軸の入力ピン番号
#define ACCZ 7 //加速度センサーZ軸の入力ピン番号
#define BAUD 9600 //Serial通信 BAUDレート
#define NO_DELAY 3 //3(msec)
#define TIME0 5 //Time0(msec)
#define TIME1 10 //Time1(msec)
#define TIME2 100 //Time2(msec)
#define TIME3 1000 //Time3(msec)
#define WUTIME 5450 //WakeUpSoundTime(msec)
#define WRTIME 4200 //WorrySoundTime(msec)
#define WR2TIME 3000 //WorrySoundTime(msec)
#define FRONT 90 //90度（正面）
#define NO_MOVE 2 //過去3回 変動2度以内で消灯
#define DEAD 3 //3度（正面(X軸90度)付近のデッドゾーン）
#define OFFSET_X -14 //R2搭載時の水平X軸オフセット-14度
#define OFFSET_Y -8.5 //R2搭載時の水平Y軸オフセット-8.5度
#define OFFSET_Z 70  //R2搭載時の垂直Z軸オフセット70
////////  Global Scope  /////////////////////////////////////////////////////////////////////////////////////
Servo headservo;
int pos;
float lastx,lasty,lastz,last2x,last2y,last2z,last3x,last3y,last3z;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void myDigitalWrite(int pin,int timeA,int timeB){
  digitalWrite(pin,HIGH);
  delay(timeA) ;
  digitalWrite(pin,LOW);
  delay(timeB) ;
}
void headWakeUp(int delaytime,int headfrom,int headto){
  digitalWrite(BLUELED,HIGH);
  for (pos = headfrom; pos <= headto; pos++){
    headservo.write(pos);
    delay(delaytime);
  }
  digitalWrite(BLUELED,LOW);
  digitalWrite(REDLED,HIGH);
  for (pos = headto; pos >= headfrom; pos--){
    headservo.write(pos);
    delay(delaytime);
  }
  digitalWrite(REDLED,LOW);
}
void R2_Worry(){
  digitalWrite(REDLED,LOW);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  headWakeUp(NO_DELAY,0,180);
  myDigitalWrite(WORRY,WRTIME,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
void R2_Worry2(){
  digitalWrite(REDLED,LOW);
  delay(TIME2);
  myDigitalWrite(REDLED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  headWakeUp(NO_DELAY,30,150);
  myDigitalWrite(WORRY2,WR2TIME,TIME2);
  myDigitalWrite(REDLED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
void setup(){
  pos = 0;
  lastx = lasty = lastz = last2x = last2y = last2z = last3x = last3y = last3z = 0.0;
  Serial.begin(BAUD);
  pinMode(REDLED,OUTPUT);
  pinMode(BLUELED,OUTPUT);
  pinMode(BB8LED,OUTPUT);
  pinMode(WAKEUP,OUTPUT);
  pinMode(WORRY,OUTPUT);
  pinMode(WORRY2,OUTPUT);
  headservo.attach(SERVO);
  headservo.write(FRONT);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(WAKEUP,WUTIME,TIME2);
  headWakeUp(TIME0,0,180);
  headservo.write(FRONT);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
void loop(){
  int i;
  float x,y,z,sens;
  i = 0;
  x = y = z = sens = 0.0;
//加速度センサー値安定化（150回読込分を平均）
  for (i = 0; i < 150; i++){
    x += analogRead(ACCX);
    y += analogRead(ACCY);
    z += analogRead(ACCZ);
  }
  x /= 150;
  y /= 150;
  z /= 150;
//加速度センサー値を角度に変換
//X:267<331<418,418-267/180=0.83888 
//Y:277<340<405,405-277/180=0.71111
//Z:277<404<418,418-277/180=0.78333
  x = ((x - 267) / 0.83888);
  x -= OFFSET_X;
  if (x > 180) {
    x = 180;
  } else if (x < 0) {
    x = 0;
  }
  y = ((y - 277) / 0.71111);
  y -= OFFSET_Y;
  if (y > 180) {
    y = 180;
  } else if (y < 0) {
    y = 0;
  }
  z = ((z - 277) / 0.78333);
  z -= OFFSET_Z;
  if (z > 180) {
    z = 180;
  } else if (z < 0) {
    z = 0;
  }
//過去3ループで全センサー値変動が2より小さければREDLED消灯 & BB8LED点灯
  if ((abs(lastx - x) < NO_MOVE && abs(lasty - y) < NO_MOVE && abs(lastz - z) < NO_MOVE) &&
      (abs(last2x - x) < NO_MOVE && abs(last2y - y) < NO_MOVE && abs(last2z - z) < NO_MOVE) &&
      (abs(last3x - x) < NO_MOVE && abs(last3y - y) < NO_MOVE && abs(last3z - z) < NO_MOVE)){
    digitalWrite(REDLED,LOW);
    digitalWrite(BB8LED,HIGH);
  } else {
    digitalWrite(REDLED,HIGH);
    digitalWrite(BB8LED,LOW);
  }
//90度（正面）付近のsensitivityを上げ0度、180度付近は下げる(Trigonometric function)
//0度から44度まではsin(270度)～sin(360度)の曲線
  if (x >= 0 && x <= 44){
    sens = (sin(radians(2*x+270))+1)*45;
//45度から134度まではsin(0度)～sin(90度)の曲線
  } else if (x > 44 && x <= 134){
    sens = 45+(sin(radians(x-45)))*90;
//135度から180度まではsin(270度)～sin(360度)の曲線
  } else {
    sens = 135+(sin(radians(2*x))+1)*45;
  }

//DeadZone設定（止まってる時のプルプル首ふり防止）
  if (sens >= (FRONT - DEAD) && sens <= (FRONT + DEAD)) {
    if (abs(lastx - x) < DEAD && abs(last2x - x) < DEAD && abs(last3x - x) < DEAD) {
      sens = lastx;
    }
  }
  
  headservo.write(sens);

/*↓
  Serial.print("X:");
  Serial.print("\t");
  Serial.print(x);
  Serial.print("\t");
  Serial.print("Y:");
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print("Z:");
  Serial.print("\t");
  Serial.print(z);
  Serial.println("\t");
//↑*/

//急発進急停止検知
//↓ブレーキ 5/9G以上、アクセル1/2G以上で警告
  if ( y  > 140 or y < 45 ){
    R2_Worry();
  }
//急上昇急降下検知
//↓上昇 1/2G以上、下降 1/2G以上で警告
  if ( z > 135 or z < 45 ){
    R2_Worry2();
  }
  
  last3x = last2x;
  last3y = last2y;
  last3z = last2z;
  last2x = lastx;
  last2y = lasty;
  last2z = lastz;
  lastx = x;
  lasty = y;
  lastz = z;

  delay(TIME0);
}

