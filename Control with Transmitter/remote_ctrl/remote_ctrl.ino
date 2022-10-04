#include "rpins.h"
#include <Servo.h>
#include <math.h>

Servo servo_obj[6];

const int WIDTH = 300;
const int LENGTH = 210;
const int servo_range[6][2] = {{10,160},{15,165},{12,168},{10,210},{10,194},{5,189}};
float servo_values[7] = {0,0,0,0,0,0,0};
float motor_values[6] = {0,0,0,0,0,0};

int read_sticks(int channel){
  int chvalue = pulseIn(channel, HIGH);
  if (chvalue>1900){
    chvalue=1900; 
  }
  else if (chvalue<1100){
    chvalue=1100;
  }
  return chvalue;
}

int read_switches(int channel){
  int chvalue = pulseIn(channel, HIGH);
  if (channel==CH5){
    chvalue = (chvalue>1500) ? 2 : 1;
  }
  else if (channel==CH6){
    if (chvalue<1250){
      chvalue = 1;
    }
    else if (chvalue>1900){
      chvalue = 3;
    }
    else{
      chvalue = 2;
    }
  }
  return chvalue;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);  

  for (int i=0;i<6;i++){
    servo_obj[i].attach(servo_pins[i]);
  }
}

int ch1value, ch2value, ch3value, ch4value, ch5value, ch6value;

void calculate(int angle, int throttle, int spot_throttle, int swA, int swC){
  if (angle>30 || angle<-30){
    digitalWrite(13, HIGH);
  }
  else{
    digitalWrite(13, LOW);
  }

  servo_values[6] = (angle>0) ? 1 : 0;

  //calc angles
  float y = LENGTH*tan(abs(angle)*PI/180);
  float x = LENGTH+WIDTH*tan(abs(angle)*PI/180);
  float angle2 = atan2(y,x)*180/PI;
  //store angles
  servo_values[0]=angle2+90;
  servo_values[1]=90;
  servo_values[2]=-angle2+90;
  servo_values[3]=abs(angle)+90;
  servo_values[4]=90;
  servo_values[5]=-abs(angle)+90;

  //speed calc
  float speed0, speed1, speed4;
  if (angle==0){
    speed0 = speed1 = speed4 = throttle;
  }
  else{
    speed0 = throttle * sin(abs(angle)*PI/180) / sin(angle2*PI/180);
    speed1 = throttle * sin(abs(angle)*PI/180) / tan(angle2*PI/180);
    speed4 = throttle * sin(abs(angle)*PI/180) / tan(abs(angle)*PI/180);
  }
  //store speed
  motor_values[0] = motor_values[2] = speed0;
  motor_values[3] = motor_values[5] = throttle;
  motor_values[1] = speed1;
  motor_values[4] = speed4;

  if (swA!=2){
    for (int i=0;i<6;i++){
      motor_values[i]=0;
    }
  }
  
  if (swC==1){
    servo_values[0]=servo_values[1]=servo_values[2]=servo_values[3]=servo_values[4]=servo_values[5]=0;
    if (swA==2){
      motor_values[0]=motor_values[1]=motor_values[2]=motor_values[3]=motor_values[4]=motor_values[5]=-spot_throttle*2;
    }
  }

  else if (swC==3){
    servo_values[6]=1;
    float spot_angle = atan2(LENGTH, WIDTH/2)*180/PI;
    servo_values[0]=servo_values[5] = 90+spot_angle;
    servo_values[2]=servo_values[3] = 90-spot_angle;
    servo_values[1]=servo_values[4] = 90;
    if (swA==2){
      //calc spot motor val using spot_throttle
      motor_values[0]=motor_values[2] = spot_throttle;
      motor_values[3]=motor_values[5] = -spot_throttle;
      motor_values[1]=spot_throttle * (WIDTH/2) / sqrt(pow(WIDTH/2,2)+pow(LENGTH,2));
      motor_values[4]=-spot_throttle * (WIDTH/2) / sqrt(pow(WIDTH/2,2)+pow(LENGTH,2));
    }
  }
}

void drive(float servos[7], float motors[6]){
  if (servos[6]==1){
    for (int i=0;i<6;i++){
      servos[i]=map(servos[i], 0, 180, servo_range[i][0], servo_range[i][1]);
      servo_obj[i].write(servos[i]);
      write_motors(motor_pins[i], motors[i]);
//      Serial.print("Servo ");Serial.print(i);Serial.print(" ");Serial.println(servos[i]);
//      Serial.print("Motor ");Serial.print(i);Serial.print(" ");Serial.println(motors[i]);Serial.println(" ");
    }
  }
  else{
    for (int i=0;i<6;i++){
      servos[5-i]=map(servos[5-i], 0, 180, servo_range[i][0], servo_range[i][1]);
      servo_obj[i].write(servos[5-i]);
      write_motors(motor_pins[i], motors[5-i]);
//      Serial.print("Servo ");Serial.print(i);Serial.print(" ");Serial.println(servos[5-i]);
//      Serial.print("Motor ");Serial.print(i);Serial.print(" ");Serial.println(motors[5-i]);Serial.println(" ");
    }
  }
}

void write_motors(int pins[3], float value){
  if (value>0){
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], LOW);
  }
  else if (value<0){
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], HIGH);
  }
  else{
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], LOW);
  }
  Serial.println(pins[0]);
  Serial.println(value);
  analogWrite(pins[0], abs(value));
}

void loop() {
  ch1value = read_sticks(CH1);
  ch2value = read_sticks(CH2);
  ch3value = read_sticks(CH3);
  ch4value = read_sticks(CH4);
  ch5value = read_switches(CH5);
  ch6value = read_switches(CH6);
  
  ch1value = map(ch1value, 1100, 1900, -45, 45);
  ch2value = map(ch2value, 1100, 1900, -100, 100);
  ch3value = map(ch3value, 1100, 1900, -100, 100);
  ch4value = map(ch4value, 1100, 1900, -100, 100);

  if (ch1value<10 && ch1value>-10) ch1value=0;
  if (ch2value<20 && ch2value>-20) ch2value=0;
  if (ch3value<20 && ch3value>-20) ch3value=0;
  if (ch4value<20 && ch4value>-20) ch4value=0;
//  Serial.println("");
  Serial.println(ch1value);
  Serial.println(ch2value);
//  Serial.println(ch2value);
//  Serial.println(ch3value);
//  Serial.println(ch4value);
//  Serial.println(ch5value);
//  Serial.println(ch6value);
  calculate(ch1value, ch2value, ch4value, ch5value, ch6value);
  drive(servo_values, motor_values);
}