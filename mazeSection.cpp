#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"

//initing the adc reading values
int scan_front;
int scan_right;
int scan_left;

// input pin numbers
int l = 0;
int f = 1;
int r = 2;

// input pin numbers for motors
int leftMotor = 1;
int rightMotor = 2;

// thresholds for going through wall
int thresholdFront = 550; // always gives high values
int thresholdRight = 60; // most sens
int thresholdLeft = 60;
int mazeCentraliseThresholdRight = 200; // as left and right have different sens
int mazeCentraliseThresholdLeft = 200;
int noWallSenseThresholdLeft = 120;
int noWallSenseThresholdRight = 120;


int mazeTurning = 0; //(-ve(Left),0(Straight),+ve(Right))

int sleepAmount = 10000;
int loopForceTimer = 100;

//////////////////////////////////
//functions						//
//////////////////////////////////

void mazeTurn(int lSpeed, int rSpeed){  //change to main codes turn
  printf("left: %d ... ", lSpeed);
  printf("right: %d\n", rSpeed);
  set_motor(rightMotor,lSpeed);
  set_motor(leftMotor,rSpeed);
}

void mazeForward(){ //goes straight
  mazeTurn(50,50);
}

void turnOnSpot(int speed){
  printf("%d\n", speed);
  set_motor(leftMotor,speed);
  set_motor(rightMotor,(-1)*speed);
}


void scanValueUpdate()
{
// updating values
  scan_front = read_analog(f); // front
  scan_right = read_analog(r); // right
  scan_left = read_analog(l); // left
}

void testTurn(){
  // Tests sensing for wall infront
  if (scan_front > thresholdFront ) //if robot is too close to the wall
  {
    if (scan_right > thresholdRight ) //there seems to be no wall there
    {
      mazeTurning = 1;
    } 
    else if (scan_left > thresholdLeft )
    {
      mazeTurning = -1;
    }
    else
    {
      mazeTurning = 10;
    }
  }
  //reallign if front is too close on one side if nec
  if (scan_left > scan_right + mazeCentraliseThresholdRight && scan_right > noWallSenseThresholdRight && mazeTurning == 0)
  { // realign right
    mazeTurning = 2;
  }
  else if (scan_right > scan_left + mazeCentraliseThresholdLeft && scan_left > noWallSenseThresholdLeft && mazeTurning == 0)
  { // realign left
    mazeTurning = -2;
  }

}

void mazeMove(){
  //Turning/reallign function
  if (mazeTurning == 1) //Turning left
  {
    mazeTurn(0,50);
    if (scan_front < thresholdFront) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    } 
  }
  else if (mazeTurning == -1) //Turning right
  {
    mazeTurn(50,0); 
    if (scan_front < thresholdFront) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == 2) //realign left
  {
    mazeTurn(50,40);
    if (scan_left > thresholdLeft)
    { // if scan of each wall is roughly similar
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == -2) //realign right
  {
    mazeTurn(40,50);
    if (scan_right > thresholdRight)
    {
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == 10)
  { // going backwards but shouldnt be needed to be called
  	mazeTurn(-50,-20);
  	if (scan_front < thresholdFront)
  	{
  	  mazeTurning == 0;
  	}

  }
  printf("front: %d left: %d right: %d turnNum: %d\n",scan_front,scan_left,scan_right,mazeTurning);
  // going straight or updated to go straight
  if (mazeTurning == 0)
  {
    mazeForward();
  }

}


void mazeSection()
{
  scanValueUpdate();
  testTurn();
  mazeMove();
}

////////////////////////
// main
////////////////////////
int main (){
  init();
  int x = -10000;
  while (x<loopForceTimer) {
    mazeSection();

    sleep1(0,sleepAmount);

    x++;
  }
  mazeTurn(50,50); 
return 0;
}
