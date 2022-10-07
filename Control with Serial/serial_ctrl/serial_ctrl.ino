#include <Servo.h>
#include <math.h>
Servo servo_obj[6];
const int servo_pins[6] = {22,24,26,28,30,32};
const int motor_pins[6][3] = {{3,37,35},{2,23,25},{4,31,33},{6,27,29},{5,50,48},{7,42,40}};

String x = "";

const int WIDTH=300;
const int LENGTH=210;
const int BASE_SPEED=100;
const int servo_range[6][2] = {{10,160},{15,165},{12,168},{10,210},{10,194},{5,189}};// (0deg value, 180deg value)
float servo_values[7] = {0,0,0,0,0,0,0};
float motor_values[6] = {0,0,0,0,0,0};

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  for (int i=0;i<6;i++){
    servo_obj[i].attach(servo_pins[i]);
  }
}

//    float y = 210*tan((target)*3.14/180);
//    float x = 210+300*tan((target)*3.14/180);
//    target_a = atan2(y,x)*180/3.14;

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

void extract_data(String cmd){
    if (cmd.substring(0,1)=="R"){
      String dir = cmd.substring(1,3);
      int target = cmd.substring(3).toInt();
      
      if (target>30 || target<-30){
        digitalWrite(13, HIGH);
      }
      else{
        digitalWrite(13, LOW);
      }
      
      Serial.println(target);
      if (target>=0){
        servo_values[6]=1;
      }
      else{
        servo_values[6]=0;
      }
      
      float y = LENGTH*tan(abs(target)*PI/180);
      float x = LENGTH+WIDTH*tan(abs(target)*PI/180);
      float target2 = atan2(y,x)*180/PI;
//      Serial.println(target2);
      float speed0, speed1, speed4;
      if (target!=0){
        speed0 = BASE_SPEED * sin(abs(target)*PI/180) / sin(target2*PI/180);
        speed1 = BASE_SPEED * sin(abs(target)*PI/180) / tan(target2*PI/180);
        speed4 = BASE_SPEED * sin(abs(target)*PI/180) / tan(abs(target)*PI/180);
      }
      else if (target==0){
        speed0 = BASE_SPEED;
        speed1 = BASE_SPEED;
        speed4 = BASE_SPEED;
      }
      motor_values[3]=motor_values[5]=BASE_SPEED;
      motor_values[2]=motor_values[0]=speed0;
      motor_values[1]=speed1;
      motor_values[4]=speed4;

      servo_values[3]=abs(target)+90;
      servo_values[4]=90;
      servo_values[5]=-abs(target)+90;
      servo_values[0]=target2+90;
      servo_values[1]=90;
      servo_values[2]=-target2+90;
      
      if (dir=="FD"){
        Serial.println("Forward");
      }
      else if (dir=="BK"){
        Serial.println("Backward");
        for (int i=0;i<6;i++){
          motor_values[i]=-motor_values[i];
        }
      }
      
      else if (dir.substring(0,1)=="S"){
        float spot_angle = atan2(LENGTH,WIDTH/2)*180/PI;
        servo_values[0]=servo_values[5]=90+spot_angle;
        servo_values[2]=servo_values[3]=90-spot_angle;
        servo_values[1]=servo_values[4]=90;
        for(int i=0;i<6;i++){motor_values[i]=0;}
        if (dir.substring(1,2)=="R" || dir.substring(1,2)=="L"){
          motor_values[0]=motor_values[2]=motor_values[3]=motor_values[5]=BASE_SPEED;
          motor_values[1]=motor_values[4]=BASE_SPEED * (WIDTH/2) / sqrt(pow(WIDTH/2,2)+pow(LENGTH,2));
        }
        else{Serial.println("SpotMode");}
        servo_values[6]=1;
        if (dir=="SR"){
          Serial.println("SpotRight");
          for (int i=3;i<6;i++){
            motor_values[i]=-motor_values[i];
          }
        }
        else if (dir=="SL"){
          Serial.println("SpotLeft");
          for (int i=0;i<3;i++){
            motor_values[i]=-motor_values[i];
          }
        }
      }
    
      else{
        Serial.println("Default(STOP)");
        for (int i=0;i<6;i++){
          motor_values[i]=0;
        }
      }
    }
}

void drive(float servos[7], float motors[6]){
  if (servos[6]==1){
    for (int i=0;i<6;i++){
      servos[i] = map(servos[i], 0, 180, servo_range[i][0], servo_range[i][1]);
      servo_obj[i].write(servos[i]);
      write_motors(motor_pins[i], motors[i]);
//      Serial.print("Servo ");Serial.print(i);Serial.print(" ");Serial.println(servos[i]);
//      Serial.print("Motor ");Serial.print(i);Serial.print(" ");Serial.println(motors[i]);Serial.println(" ");
    }
  }
  else{
    for (int i=0;i<6;i++){
      servos[5-i] = map(servos[5-i], 0, 180, servo_range[i][0], servo_range[i][1]);
      servo_obj[i].write(servos[5-i]);
      write_motors(motor_pins[i], motors[5-i]);
//      Serial.print("Servo ");Serial.print(i);Serial.print(" ");Serial.println(servos[5-i]);
//      Serial.print("Motor ");Serial.print(i);Serial.print(" ");Serial.println(motors[5-i]);Serial.println(" ");
    }
  }
}


int readflag = 0;
void loop() {
  while (Serial.available()>0){
    char inByte = Serial.read();
    if (inByte==60){
      readflag = 1;
    }
    else if (inByte==38){
//        Serial.println(x);
        extract_data(x);
        x = "";
    }
    else if (inByte==62){
//        Serial.println("ok");
        drive(servo_values, motor_values);
        readflag = 0;
    }
    else{
      if (readflag){
        x = x + inByte;
      }
    }
  }
}
