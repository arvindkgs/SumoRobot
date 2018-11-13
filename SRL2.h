
/************************************************************
 *  SRL2.h - A Library for Controlling the MRK-2 Sumo Robot League robot
 *  Author - William J. Ashby, PhD <washby@sumorobotleague.com> This library is released into the public domain.
 *  ************************************************************
 */

#ifndef SRL2_h

#define SRL2_h
#include "Arduino.h"

#define IDLING 1
#define COMPETING 2
#define pingPin 10
#define echoPin A0
#define leftSensor A1
#define rightSensor A2
#define rearSensor A3
#define buttonPin 2
#define buzzerPin 3
#define IREmitter 4
#define leftMotorSpeedPin 5
#define rightMotorSpeedPin 6
#define leftDirection 7
#define rightDirection 8
#define LED 13

class SRL2 {
  public:
    SRL2(bool debug);
    // State variables
    int buttonState = 1; // 1 = up, 0 = pressed
    int state       = 1; // 1 = idle, 2 = competing
    void buttonRead();
    bool isButtonPressed();
    void begin_countdown();
    void playNote(int, int, int);
	  bool debug2;
    bool leftIsFlipped2;
    bool rightIsFlipped2;
	  void debug(int val);	
    void beep();
    void Forward(int amt); 
    void Backward(int amt);
    void Right(int deg);
    void Left(int deg);
    void PD();
    void PU();
    void drive(int percentPower);
    void setSpeedForMotor(int spd, int motor);
    void flipMotorDirection(int mtr);
    void turnDegrees(int y);
    bool objectWithin(int cm);
    int getDistance();
    void readIR();
	  int getIR(int side);
	  int getThreshold(int side);
	  void setThreshold(int side, int val);
    bool isWhite();
    void calibrateBlackWhite();
	  void tuneForward(double x);
	  void tuneBackward(double x);
	  void tuneTurn(double x);
    void jumpStartSetup();
    int randomTurn();

  private:
    int lThresh2;
    int rThresh2;
	  int rearThresh2;
	  double tuningTurn2;
	  double tuningFwd2;
	  double tuningBwd2;
    int leftIR;
    int rightIR;
    int rearIR;
};

#endif
