// This project is use to controll car;
//declaration of pins, the 0 and 1 pin is reserved for bluetooth
const int LEDPin = 7;
const int servoPin = 8; //connect to the servo
const int frontPin = 5; //work with the backPin and connect to the h-bridge
const int backPin = 6; //work with the frontPin to control direction

int straightDegree = 90; //default degree
int rightDegree = 45; //turn left
int leftDegree = 135; //turn right

char data; //data read from the bluetooth

int sp; //the analogWrite speed 0-255

void setup() {
  // setup all the pins 
  pinMode(LEDPin, OUTPUT); //for digitalWrite
  pinMode(frontPin, OUTPUT); //for analogWrite
  pinMode(backPin, OUTPUT); //for analogWrite
  pinMode(servoPin, OUTPUT); //connect to pin 8 to control servo;

  Serial.begin(9600); //port associated with bluetooth

  //servo.write(straightDegree); //set to default degree
  servo(straightDegree);
  sp = 0; //default speed is 0
}

void loop() {
  //controlling the car
  if (Serial.available()>0) { //got some data
    data = Serial.read(); //read the data
    Serial.println(data); //for debugging purpose

    //responding to the data
    setSp(); //set up the speed
    LEDMethod(); //method controlling the car LED 
    FBMethod(); //method handling the front back 
    LRMethod(); //method handling the left right

    restore(); //method to set all value to default
  }
}

void servo(int angle) { //定义一个脉冲函数
  //发送50个脉冲
  for(int i=0;i<50;i++){
    int pulsewidth = (angle * 11) + 500; //将角度转化为500-2480的脉宽值
    digitalWrite(servoPin, HIGH);   //将舵机接口电平至高
    delayMicroseconds(pulsewidth);  //延时脉宽值的微秒数
    digitalWrite(servoPin, LOW);    //将舵机接口电平至低
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(100);
}


void restore() { //when disconnect the Serial port will received PAIRANLE, 
  if (data == 'E') { //which end with E
    sp = 0; //set the speed to 0
    digitalWrite(LEDPin, LOW); //turn off the LED
    analogWrite(frontPin, 0); //stop
    analogWrite(backPin, 0); //stop
//    servo.write(straightDegree); //go straight
    servo(straightDegree);
  }
}

void LEDMethod() {
  if (data == 'O') {
    digitalWrite(LEDPin, HIGH); //turn on the LED
  } else if (data == 'o') {
    digitalWrite(LEDPin, LOW); //turn off the LED
  }
}

void FBMethod() { //front back method
  if (data == 'F') { //front
    analogWrite(frontPin, sp);
    analogWrite(backPin, 0);
  } else if (data == 'f') {
    analogWrite(frontPin, 0);
  } else if (data == 'B') {
    analogWrite(backPin, sp);
    analogWrite(frontPin, 0);
  } else if (data == 'b') {
    analogWrite(backPin, 0);
//    digitalWrite(backPin, LOW);
  }
}

void LRMethod() { //left right method
  if (data == 'L') { //turn left 45 degree (90-45)
      servo(leftDegree);
  } else if (data == 'R') { //turn right 45 degree (90+45)
    servo(rightDegree);
  } else if (data == 'l' || data == 'r') { //straight default 90 degree
    servo(straightDegree);
  }
}

void setSp() { //set up the speed from 0-9 (max:m) to 0-255
  if (data=='0') {
    sp = 0;
  } else if (data=='1') {
    sp = 26;
  } else if (data=='2') {
    sp = 52;
  } else if (data=='3') {
    sp = 77;
  } else if (data=='4') {
    sp = 102;
  } else if (data=='5') {
    sp = 128;
  } else if (data=='6') {
    sp = 153;
  } else if (data=='7') {
    sp = 179;
  } else if (data=='8') {
    sp = 204;
  } else if (data=='9') {
    sp = 230;
  } else if (data=='m') { //maximum speed
    sp = 255;
  }
}

