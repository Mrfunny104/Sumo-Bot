/********************************
|                                |
|             Pre-Setup          |
|                                |
********************************/
int LPM = 10; //Left Positive Motor
int LNM = 11; //Left Negative Motor
int RPM = 9;  //Right Positive Motor
int RNM = 8;  //Right Negative Motor

int LEN = 5; //Left Enable
int REN = 6; //Right Enable

int LDRF = A0; //Photoresistor on front of bot

/********************************
|                               |
|            Functions          |
|                               |
********************************/
void motorControl(float Speed, int PWM, int D1, int D2) {
  if (Speed != 0) {
  if (Speed > 0) {
      analogWrite(PWM, Speed);
      digitalWrite(D1, HIGH);
      digitalWrite(D2, LOW);
  } else if (Speed < 0) {
      Speed = abs(Speed);
      analogWrite(PWM, Speed);
      digitalWrite(D1, LOW);
      digitalWrite(D2, HIGH);
  }
  } else {
  digitalWrite(PWM, LOW);
  }
}

/********************************
|                               |
|              Setup            |
|                               |
********************************/
void setup()
{
 Serial.begin(9600);
  
 pinMode (LPM,OUTPUT);
 pinMode (LNM,OUTPUT);
 pinMode (RPM,OUTPUT);
 pinMode (RNM,OUTPUT);
 
 pinMode(LEN,OUTPUT);
 pinMode(REN,OUTPUT);
  
 pinMode(LDRF,INPUT);
  
 motorControl(255, LEN, LPM, LNM);
 motorControl(255, REN, RPM, RNM);
}

/********************************
|                               |
|              Loop             |
|                               |
********************************/
void loop()
{
  int x; 
  x = analogRead(LDRF); 
  
  if (x < 450){
    Serial.println("Backwards");
    motorControl(-255, LEN, LPM, LNM);
    motorControl(-255, REN, RPM, RNM);
    delay(1000);
    Serial.println("Turn");
    motorControl(255, LEN, LPM, LNM);
    motorControl(-255, REN, RPM, RNM);
    delay(500);
    Serial.println("Forwards");
    motorControl(255, LEN, LPM, LNM);
    motorControl(255, REN, RPM, RNM);
    delay(1000);
   } 
  
  /********************************
  |                               |
  |              Debug            |
  |                               |
  ********************************/
  /*int x;
  x = analogRead(LDRF);*/
  /*Serial.print("FRONT = ");
  Serial.println(x);*/
}
