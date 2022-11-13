/********************************
|                               |
|           Pre-Setup           |
|                               |
********************************/
int LPM = 10; //Left Positive Motor
int LNM = 11; //Left Negative Motor
int RPM = 9;  //Right Positive Motor
int RNM = 8;  //Right Negative Motor

int LEN = 5; //Left Enable
int REN = 6; //Right Enable

int LDRF = A0; //Photoresistor on front of bot
int LDRB = A1; //Photoresistor on back of bot

long duration; 
int distance;

/********************************
|                               |
|           Functions           |
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
|             Setup             |
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
 pinMode(LDRB,INPUT);
  
 pinMode(2, INPUT);  //echo 
 pinMode(3, OUTPUT); //trigger 

 //Initial charge to close distance
 Serial.println("Initial Charge");
 motorControl(255, LEN, LPM, LNM);
 motorControl(255, REN, RPM, RNM);
 delay(1000);
 
}

/********************************
|                               |
|             Loop              |
|                               |
********************************/
void loop()
{
  //Trigger code 
  digitalWrite(3, LOW);
  delay(0.2); 
  digitalWrite(3, HIGH);
  delay(0.10); 
  digitalWrite(3, LOW);
  
  //Echo code 
  duration = pulseIn(2, HIGH); 
  distance = (duration*0.034)/2;
  delay(0.2);
  
  /*******************
  | LDR Ring Checker | //Reads LDR values to keep inside the ring
  *******************/
  int x;
  int y;
  y = analogRead(LDRB);
  x = analogRead(LDRF);
  
  if (x > 450){ // < for black as line colour, > for white as line colour
    Serial.println("Reverse from line");
    motorControl(-255, LEN, LPM, LNM);
    motorControl(-255, REN, RPM, RNM);
    delay(1000);
   }
  if (y > 600){ //this value is calibrated for real prototype
    Serial.println("Forwards from line");
    motorControl(255, LEN, LPM, LNM);
    motorControl(255, REN, RPM, RNM);
    delay(1000);
   }
   
  /*******************
  |   Attack Mode 1  | //circle to find enemy, rush when detected
  *******************/
  else {
    Serial.println(" ");
    Serial.println("Circling"); 
    motorControl(-255, LEN, LPM, LNM);
    motorControl(255, REN, RPM, RNM);
    
    if (distance < 25){ 
      Serial.println("Charge"); 
      motorControl(255, LEN, LPM, LNM);
      motorControl(255, REN, RPM, RNM);
      delay(1500);
      
      if (distance < 10){
        Serial.println("Follow up charge"); 
        motorControl(255, LEN, LPM, LNM);  
        motorControl(255, REN, RPM, RNM);
        delay(750);
      }
    }
  }

  /********************************
  |                               |
  |              Debug            |
  |                               |
  ********************************/
  /*int x;
  x = analogRead(LDRF);*/
  /*Serial.print("LDR FRONT = ");
  Serial.println(x);*/
  
  /*int y;
  y = analogRead(LDRB);*/
  /*Serial.print("LDR BACK = ");
  Serial.println(y);*/
  
  /*
  Serial.print("Distance F: ");
  Serial.println(distanceF);
  Serial.print("Distance R: ");
  Serial.println(distanceR);
  */
}
