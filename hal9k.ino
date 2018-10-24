
/******     
This code belongs to HAL9000 TEAM
********************************************************************/

// Pins numbers mapped with readable strings
#define pingPin 10
#define echoPin A0
#define leftSensor A1
#define rightSensor A2
#define rearSensor A3
#define buttonPin 2
#define buzzerPin 3
#define IREmitter 4
#define leftMotorSpeed 5
#define rightMotorSpeed 6
#define leftDirection 7
#define rightDirection 8
#define LED 13

bool leftIsFlipped = true; //If the left motor goes backwards when it is supposed to go forwards set to true
bool rightIsFlipped = false; 
int lBlack = 800; int rBlack = 800; //default black and white values
int lWhite = 100; int rWhite = 100;

// Global to store if the robot is currently fighting.
bool isHalOn = false;

void setup() {
  // This code runs once in the lifecycle of sumo bot
  Serial.begin(9600);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(leftDirection, OUTPUT);
  pinMode(rightDirection, OUTPUT);
  pinMode(leftMotorSpeed, OUTPUT);
  pinMode(rightMotorSpeed, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(rearSensor, INPUT);
  pinMode(IREmitter, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly
  // The loop function gets called repeatedly, thousands of times a second
  // In this code, the only state is saved in the isHalOn global

  // If the button is being pushed
  if(digitalRead(buttonPin)==0) {
    Serial.print("Button pushed: ");
    Serial.println(isHalOn);
    if(isHalOn==false) {
      // If the robot isn't currently on, wait 5 seconds before starting to battle.  This is the Sumobot default
      delay(5000);
      // Once isHalOn is true, the drive functionality is enabled
      isHalOn=true;
    }
    else {
      // If the robot is on, then we need to turn it off.  If you don't put a delay in, the robot will get turned off then immediately
      // get turned back on
      delay(1000);
      isHalOn=false;
    }
  }
  // If I'm in a sumo battle, start
  if(isHalOn) {
    // Attack
    // If an object is within 10 cm of our ultrasonic sensor, drive at full strength for 100 miliseconds, then turn the drive off
    //[Babloo] Increased the range to 15 cm
    if (objectWithin(15)) {
      // Attack
	  //TODO: If you find enemy in range then hit and push
      beep();
      drive(100);
      delay(1000);
      drive(0);    
    } 
  
    //Sense edges
    digitalWrite(IREmitter, HIGH);
    int rightInput = analogRead(rightSensor);
    int leftInput = analogRead(leftSensor);
    int rearInput = analogRead(rearSensor);
  
    /*
    Serial.print("Left/");
    Serial.print(leftInput);
    Serial.print(" - Right/");
    Serial.println(rightInput);
    Serial.print(" - Rear/");
    Serial.println(rearInput);
    */

    // These are cases to take care if parts of the robot are off the mat
    // If the rear seconsor is off the mat, but both the front sensors are on the matt
    if(rearInput < lBlack && leftInput > lBlack && rightInput > lBlack)
    {
      // Pick a random number of degrees to turn between -45 (right) and 45 (left) to tell us which way we should turn
      int randomTurn = random(-45, 45);
      Serial.print("Turning: ");
      Serial.println(randomTurn);
      // Turn, then drive at full speed for 1 second
      turnDegrees(0.0 + randomTurn);
      drive(100);
      delay(1000);
      drive(0);
    }
    // If the rear input is on the mat, but the front left and right aren't
    if(rearInput > lBlack && leftInput < lBlack && rightInput < lBlack)
    {
      // Go backwards at full speed for 1 second
      drive(-100);
      delay(1000);
      drive(0);
      // Pick a random number of degrees to turn between -90 (right) and 90 (left)
      int randomTurn = random(-90, 90);
      Serial.print("Turning: ");
      Serial.println(randomTurn);
      turnDegrees(0.0 + randomTurn);
    }
    // If the front right is off the mat and the front left isn't
    else if (leftInput > lBlack && rightInput < lBlack)
    {
      // Turn 90 degrees to the left and go forward for 1 second
      turnDegrees(90.0);
      drive(100);
      delay(1000);
      drive(0);
    }
    // If the front left is off the mat and the front right isn't
    else if (leftInput < lBlack && rightInput > lBlack)
    {
      // Turn 90 degrees to the right and go forward for 1 second
      turnDegrees(-90.0);
      drive(100);
      delay(1000);
      drive(0);
    }
    else {
      // All sensors are on the mat.  Drive forward for .1 seconds
      // Wander
      drive(100);
      delay(100);
      drive(0);        
    }
  }
}


void beep() {
  tone(buzzerPin, 1397, 250); // Freq. 1397 = F6
}

// below here are the functions to help you get up and running fast.  Feel free to look around but making changes could break your code.

void drive(int x) {
  // map converts [-100,100] to [-255,255]
  int spd = map(x, -100, 100, -255, 255);
  setSpeeed(spd, 0);
  setSpeeed(spd, 1);
}

void setSpeeed(int sped, int motor) {
  int speedPin, directionPin;
  bool flippedPin;
  if ( motor == 0 ) {
    speedPin = leftMotorSpeed;
    directionPin = leftDirection;
    flippedPin = leftIsFlipped;
  } else if ( motor == 1 ) {
    speedPin = rightMotorSpeed;
    directionPin = rightDirection;
    flippedPin = rightIsFlipped;
  }

  if ( sped == 0 ) {
    analogWrite(speedPin, 0);
    digitalWrite(directionPin, LOW);
  } else if ( (sped > 0 && !flippedPin) || (sped < 0 && flippedPin) ) {
    digitalWrite(directionPin, HIGH);
    analogWrite(speedPin, abs(sped));
  } else if ( (sped < 0 && !flippedPin) || (sped > 0 && flippedPin) ) {
    digitalWrite(directionPin, LOW);
    analogWrite(speedPin, abs(sped));
  }
}

void turnDegrees(double y) {
  bool lWheel = 0;
  bool rWheel = 0;

  if (y > 0) {
    lWheel = 0;
    rWheel = 1;
    if (leftIsFlipped) {
      lWheel = 1;
    }
    if (rightIsFlipped) {
      rWheel = 0;
    }
    digitalWrite(leftDirection, lWheel); analogWrite(leftMotorSpeed, 255);
    digitalWrite(rightDirection, rWheel); analogWrite(rightMotorSpeed, 255);
    delay(y * 6.21111);
    analogWrite(leftMotorSpeed, 0);
    analogWrite(rightMotorSpeed, 0);
  } else if (y < 0) {
    lWheel = 1;
    rWheel = 0;
    if (leftIsFlipped) {
      lWheel = 0;
    }
    if (rightIsFlipped) {
      rWheel = 1;
    }
    digitalWrite(leftDirection, lWheel); analogWrite(leftMotorSpeed, 255);
    digitalWrite(rightDirection, rWheel); analogWrite(rightMotorSpeed, 255);
    delay(abs(y * 6.21111));
    analogWrite(leftMotorSpeed, 0);
    analogWrite(rightMotorSpeed, 0);
  } else Serial.println("0 degree turn");
}

boolean objectWithin(int cm) {
  return getDistance() < cm;
}

int getDistance() {
  long duration;
  int distance;
  digitalWrite(pingPin, LOW);// Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (int) ( (duration / 2) / 29.1 );
  return distance;
}