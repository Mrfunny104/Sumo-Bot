/********************************
|                  |
|          Pre-Setup      |
|                 |
********************************/
int LPM = 10; //Left Positive Motor
int LNM = 11; //Left Negative Motor
int RPM = 9;  //Right Positive Motor
int RNM = 8;  //Right Negative Motor

int LEN = 5; //Left Enable
int REN = 6; //Right Enable

int LDRF = A0; //Photoresistor on front of bot
int LDRB = A1; //Photoresistor on back of bot

long durationF; //F = Front
int distanceF;
long durationR; //R = Rear
int distanceR;

/********************************
|                 |
|         Functions     |
|                 |
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
|                 |
|         Setup       |
|                 |
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
  
 pinMode(2, INPUT); //echo Front
 pinMode(3, OUTPUT); //trigger Front
 pinMode(12, INPUT); //echo Rear
 pinMode(13, OUTPUT); //trigger Rear
}

/********************************
|                 |
|         Loop        |
|                 |
********************************/
void loop()
{
  //Trigger code front
  digitalWrite(3, LOW);
  delay(0.2); 
  digitalWrite(3, HIGH);
  delay(0.10); 
  digitalWrite(3, LOW);
  
  //Echo code front
  durationF = pulseIn(2, HIGH); 
  distanceF = (durationF*0.034)/2;
  delay(0.2);
   
  //Trigger code rear
  digitalWrite(13, LOW); 
  delay(0.2); 
  digitalWrite(13, HIGH); 
  delay(0.10); 
  digitalWrite(13, LOW); 

  //Echo code rear
  durationR = pulseIn(12, HIGH); 
  distanceR = (durationR*0.034)/2;
  delay(0.2);
  
  /*******************
  | LDR Ring Checker | //Reads LDR values to keep inside the ring
  *******************/
  int x;
  int y;
  y = analogRead(LDRB);
  x = analogRead(LDRF);
  
  if (x < 450){
    Serial.println("Reverse from line");
    motorControl(255, LEN, LPM, LNM);
    motorControl(255, REN, RPM, RNM);
    delay(1000);
   }
  else if (y < 500){
    Serial.println("Forwards from line");
    motorControl(-255, LEN, LPM, LNM);
    motorControl(-255, REN, RPM, RNM);
    delay(1000);
   }
  
  /*******************
  |   Attack Mode 1  | //circle to find enemy, rush when detected
  *******************/
  else {
    Serial.println("Circling"); //Constantly Circles to find enemy
    motorControl(255, LEN, LPM, LNM);
    motorControl(-255, REN, RPM, RNM);
    
    if (distanceF < 100){ 
        Serial.println("Charge"); //When enemy spotted, charges
      motorControl(255, LEN, LPM, LNM);
      motorControl(255, REN, RPM, RNM);
      delay(2000);
    }
    if (distanceR < 100){
        Serial.println("180 Time"); //If enemy detected behind, turns to face it
        motorControl(255, LEN, LPM, LNM); //Using left turn because turning distance will be shorter 
      motorControl(-255, REN, RPM, RNM); //This is because the bot is already turning this direction
      delay(1000);
    }
  }
    
  /********************************
  |                   |
  |           Debug       |
  |                 |
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
