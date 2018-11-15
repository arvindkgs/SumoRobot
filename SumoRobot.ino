#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "Pitches.h"
#include "SRL2.h"

#define buttonPin 2
#define buzzerPin 3
#define LEDpin 13

/*************************************************/
/** Global Variables and Configuration Settings **/
/*************************************************/
// Competition configuration settings
#define attackDistance 25
#define debug false
#define speed 80
#define angle 90
SRL2 bot(debug);

void setup() {
  bot.jumpStartSetup();
}

void loop() {
  bot.buttonRead();
  if ( bot.isButtonPressed() ) {
    bot.buttonState = 1;
    if ( bot.state == IDLING) {
      // we were in idle mode, so let's begin our
      // competition countdown
      Serial.println("Competition countdown starting...");
      Serial.println("Good luck!");
      //bot.calibrateBlackWhite();
      bot.begin_countdown();
    } else {
      bot.state = IDLING;
      bot.Left(0);
      bot.Right(0);
      Serial.println("Robot entering idle mode.");
    }
  }

  // if the countdown has ended, we're now in competition mode
  if ( bot.state == COMPETING ) {
    compete();
  }
}

// This function is the primary competition control
// function and controls the operational flow and state
// of the robot in competition mode
void compete() {
  bot.readIR();
  if( bot.getIR(rearSensor) < bot.getThreshold(rearSensor) ){
      if(debug){
        Serial.println("Detected border on rearSensor");
      }
      //Drive forward at full speed for 0.1 second
      bot.Forward(speed);
  }
  else if( bot.getIR(leftSensor) < bot.getThreshold(leftSensor) && bot.getIR(rightSensor) < bot.getThreshold(rightSensor) ){
      if(debug){
        Serial.println("Detected border on front sensors");
      }
      //Move Backward
      bot.Backward(speed);
      // Pick a random number of degrees to turn between -90 (right) and 90 (left)
      bot.turnDegrees(0.0 + bot.randomTurn());
  }
  else if( bot.getIR(leftSensor) < bot.getThreshold(leftSensor) ){
      if(debug){
        Serial.println("Detected border on leftSensor");
      }
      // Turn 90 degrees to the right and go forward for 1 second
      bot.Right(angle);
      bot.Forward(speed);
  }
  else if( bot.getIR(rightSensor) < bot.getThreshold(rightSensor) ){
      // Turn 90 degrees to the Left and go forward for 1 second
      if(debug){
        Serial.println("Detected border on rightSensor");
      }
      bot.Left(angle);
      bot.Forward(speed);
  }
  if( bot.objectWithin(attackDistance) ){
      // Attack
      bot.beep();
      bot.Forward(speed);
  }
  else{
    //Wander
    bot.Forward(speed);
  }
}
