//R2-D2 triple axis Accelerometer and BB-8 LED Light for cx-3
//2016/8/22 BlueDK5FW
#include <Servo.h>
unsigned const int WAKEUP = 3; //起動音再生の出力ピン番号
unsigned const int WORRY = 4; //心配音再生の出力ピン番号
unsigned const int WORRY2 = 5; //心配音2再生の出力ピン番号
unsigned const int SERVO = 7; //Servoの(入)出力ピン番号
unsigned const int REDLED = 9; //RedLEDの出力ピン番号
unsigned const int BB8LED = 12; //BB8LEDの出力ピン番号
unsigned const int BLUELED = 13; //BlueLEDの出力ピン番号
unsigned const int ACCX = 3; //加速度センサーX軸の入力ピン番号
unsigned const int ACCY = 5; //加速度センサーY軸の入力ピン番号
unsigned const int ACCZ = 7; //加速度センサーZ軸の入力ピン番号
unsigned const int BAUD = 9600; //Serial通信 BAUDレート
unsigned const int AXIS_X = 1; //X軸
unsigned const int AXIS_Y = 2; //Y軸
unsigned const int AXIS_Z = 3; //Z軸
unsigned const int QUART_DEG = 45; //45度
unsigned const int QUART2_DEG = 90; //90度
unsigned const int QUART3_DEG = 135; //135度
unsigned const int QUART4_DEG = 180; //180度
unsigned const int THIRD_DEG = 30; //30度
unsigned const int THIRD5_DEG = 150; //150度
//X:267<331<404
//Y:270<342<407
//Z:335<403<471  403(1G)- 267(UpsideDown,-1G)=136(2G),403 - 68 = 335 ,403 + 68 = 471
unsigned const int MIN_X = 267; //★-1G時のX軸
unsigned const int ZERO_X = 331; //★0G時のX軸
unsigned const int MAX_X = 404; //★1G時のX軸
unsigned const int MIN_Y = 270; //★-1G時のY軸
unsigned const int ZERO_Y = 342; //★0G時のY軸
unsigned const int MAX_Y = 407; //★1G時のY軸
unsigned const int MIN_Z = 335; //★0GのZ軸
unsigned const int ONE_Z = 403; //★1G時のZ軸
unsigned const int MAX_Z = 471; //★2G時のZ軸
unsigned const int NO_DELAY = 3; //3(msec)
unsigned const int TIME0 = 5; //Time0(msec)
unsigned const int TIME1 = 10; //Time1(msec)
unsigned const int TIME2 = 100; //Time2(msec)
unsigned const int TIME3 = 1000; //Time3(msec)
unsigned const int WUTIME = 5350; //WakeUpSoundTime(msec)
unsigned const int WRTIME = 4200; //WorrySoundTime(msec)
unsigned const int WR2TIME = 3000; //WorrySoundTime(msec)
const float NO_MOVE = 0.5; //過去3回 変動0.5以内
const float OFFSET_X = 10.00; //車載時の水平X軸オフセット
const float OFFSET_Y = 3.00; //車載時の水平Y軸オフセット(ダッシュボードは少し傾いている)
const float OFFSET_Z = 1.30; //重力加速度はMAX,MINに織込
const float LPF_VAL = 0.90;  //LowpassFilterの感度
////////  Global Scope  /////////////////////////////////////////////////////////////////////////////////////
Servo headservo;
int pos;
unsigned long loopCnt;
float lastX,lastY,lastZ,last2X,last2Y,last2Z,last3X,last3Y,last3Z,differX,differY,differZ;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float convertToAngle(float val,unsigned int axis,float insens) {
  float angle;
  if (axis == AXIS_X) {  //x axis
    angle = (val - MIN_X) / (differX / QUART4_DEG);
  } else if (axis == AXIS_Y){  //y axis
    angle = (val - MIN_Y) / (differY / QUART4_DEG);
  } else {  //z axis
    angle = (val - MIN_Z) / (differZ / QUART4_DEG);
  }
  if (angle < QUART2_DEG) {
    angle = QUART2_DEG - ((QUART2_DEG - angle) / insens);
  } else {
    angle = QUART2_DEG + ((angle - QUART2_DEG) / insens);
  }
  if (angle > QUART4_DEG) {
    angle = QUART4_DEG;
  } else if (angle < 0) {
    angle = 0;
  }
  return angle;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void myDigitalWrite(int pin,int timeA,int timeB){
  digitalWrite(pin,HIGH);
  delay(timeA) ;
  digitalWrite(pin,LOW);
  delay(timeB) ;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void R2_Worry(){
  digitalWrite(REDLED,LOW);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  headWakeUp(NO_DELAY,THIRD_DEG,THIRD5_DEG);
  myDigitalWrite(WORRY,WRTIME,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void R2_Worry2(){
  digitalWrite(REDLED,LOW);
  delay(TIME2);
  myDigitalWrite(REDLED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  headWakeUp(NO_DELAY,QUART_DEG,QUART3_DEG);
  myDigitalWrite(WORRY2,WR2TIME,TIME2);
  myDigitalWrite(REDLED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup(){
  pos = 0;
  loopCnt = 0;
  lastX = lastY = lastZ = last2X = last2Y = last2Z = last3X = last3Y = last3Z = differX = differY = differZ = 0.0;
  Serial.begin(BAUD);
  pinMode(REDLED,OUTPUT);
  pinMode(BLUELED,OUTPUT);
  pinMode(BB8LED,OUTPUT);
  pinMode(WAKEUP,OUTPUT);
  pinMode(WORRY,OUTPUT);
  pinMode(WORRY2,OUTPUT);
  headservo.attach(SERVO);
  differX = MAX_X - MIN_X;
  differY = MAX_Y - MIN_Y;
  differZ = MAX_Z - MIN_Z;
  headservo.write(QUART2_DEG);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(WAKEUP,WUTIME,TIME2);
  headWakeUp(TIME0,THIRD_DEG,THIRD5_DEG);
  headservo.write(QUART2_DEG);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BLUELED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
  myDigitalWrite(BB8LED,TIME2,TIME2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop(){
  int i;
  float x,y,z,sens;
  i = 0;
  x = y = z = sens = 0.0;

  x = LPF_VAL * lastX + (1 - LPF_VAL) * analogRead(ACCX);  //CR LowpassFilter
  y = LPF_VAL * lastY + (1 - LPF_VAL) * analogRead(ACCY);
  z = LPF_VAL * lastZ + (1 - LPF_VAL) * analogRead(ACCZ);

//過去3ループで全センサー値変動が0.5より小さければREDLED消灯 & BB8LED点灯
  if ((abs(lastX - x) < NO_MOVE && abs(lastY - y) < NO_MOVE && abs(lastZ - z) < NO_MOVE) &&
      (abs(last2X - x) < NO_MOVE && abs(last2Y - y) < NO_MOVE && abs(last2Z - z) < NO_MOVE) &&
      (abs(last3X - x) < NO_MOVE && abs(last3Y - y) < NO_MOVE && abs(last3Z - z) < NO_MOVE)){
    digitalWrite(REDLED,LOW);
    digitalWrite(BB8LED,HIGH);
  } else {
    digitalWrite(REDLED,HIGH);
    digitalWrite(BB8LED,LOW);
  }

  last3X = last2X;
  last3Y = last2Y;
  last3Z = last2Z;
  last2X = lastX;
  last2Y = lastY;
  last2Z = lastZ;
  lastX = x;
  lastY = y;
  lastZ = z;

//加速度センサー値を角度に変換
  x = convertToAngle(x ,AXIS_X, 0.7);
  y = convertToAngle(y ,AXIS_Y, 1.0);
  z = convertToAngle(z ,AXIS_Z, 1.0);

  x += OFFSET_X;
  y += OFFSET_Y;
  z += OFFSET_Z;

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
  
  if (loopCnt > 200) {  //LowpassFilter値が安定してから
    headservo.write(sens);
    //急発進急停止検知
    //↓ブレーキ 5/9G以上、アクセル1/2G以上で警告
    if ( y  > 140 || y < QUART_DEG ){
      R2_Worry();
      lastY = ZERO_Y;
    }
    //急上昇急降下検知
    //↓上昇 1/2G以上、下降 1/2G以上で警告
    if ( z > QUART3_DEG || z < QUART_DEG ){
      R2_Worry2();
      lastZ = ONE_Z;
    }
  } else {
    headservo.write(QUART2_DEG);
  }

  loopCnt++;
  delay(TIME0);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.print(z);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(lastX);
  Serial.print("\t");
  Serial.print(lastY);
  Serial.print("\t");
  Serial.print(lastZ);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(last2X);
  Serial.print("\t");
  Serial.print(last2Y);
  Serial.print("\t");
  Serial.print(last2Z);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print(last3X);
  Serial.print("\t");
  Serial.print(last3Y);
  Serial.print("\t");
  Serial.print(last3Z);
  Serial.print("\t");
  Serial.println("\t");
//*/


