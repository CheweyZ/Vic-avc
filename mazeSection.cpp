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

int distFromWall = 600;
int distFromSideWall = 100;
int sleepAmount = 10000;
int mazeTurning = 0; //(-ve(Left),0(Straight),+ve(Right))

int loopForceTimer = 100000;

/////////////////////////////////
//functions
////////////////////////////////

void mazeTurn(int lSpeed, int rSpeed){  //change to main codes turn
  printf("left: %d ... ", lSpeed);
  printf("right: %d\n", rSpeed);
  //set_motor(rightMotor,lSpeed);
  //set_motor(leftMotor,rSpeed);
}

void mazeForward(){ //goes straight
  mazeTurn(100,100);
}

void turnOnSpot(int speed){
  printf("%d\n", speed);
  //set_motor(leftMotor,speed);
  //set_motor(rightMotor,(-1)*speed);
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
  if (scan_front > distFromWall ) //if robot is too close to the wall
  {
    if (scan_right > distFromSideWall) //there seems to be no wall there
    {
      mazeTurning = 1;
    } 
    else if (scan_left > distFromSideWall)
    {
      mazeTurning = -1;
    }
    else
    {
      mazeTurning = 10;
    }
  }
  //reallign if front(/back) is too close on one side if nec
  if (scan_left > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && mazeTurning == 0)
  { // realign right
    mazeTurning = -2;
  }
  else if (scan_right > scan_left + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && mazeTurning == 0)
  { // realign left
    mazeTurning = 2;
  }
  //old realign code
  /*if (scan_left > distFromWall && mazeTurning == 0) //change to test to adjust when: one side is bigger than the other, no side is giving ~0 (gap in wall) and is not mazeTurning
  { // realign right
    mazeTurning = -2;
  }
  else if (scan_right > distFromWall && mazeTurning == 0)
  { // realign left
    mazeTurning = 2;
  }*/

}

void mazeMove(){
  //Turning/reallign function
  if (mazeTurning == 1) //Turning left
  {
    mazeTurn(10,100);
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    } 
  }
  else if (mazeTurning == -1) //Turning right
  {
    mazeTurn(100,10); 
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == 2) //realign left
  {
    mazeTurn(100,50);
    if (scan_right < distFromSideWall)
    { // if scan of each wall is roughly similar
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == -2) //realign right
  {
    mazeTurn(50,100);
    if (scan_left < distFromSideWall)
    {
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == 10)
  { // going backwards but shouldnt be needed to be called
  	printf("error\n");

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
  //for (int i = 0; i < 10; ++i)
  //{
  //  for (int j = 0; j < 10; ++j)
  //  {
  //  int x=0;
  //mazeSection();
  //  }
  //}
  while (x<loopForceTimer) {
    mazeSection();

    sleep1(0,sleepAmount);

    x++;
  }
return 0;
}
