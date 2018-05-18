#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"

//initing the adc reading values
int scan_front;
int scan_right;
int scan_left;

// input pin numbers to change on prop code
int f = 0;
int r = 1;
int l = 2;

// input pin numbers for motors to change on prop code
int leftMotor = 1;
int rightMotor = 2;

int distFromWall = 600;
int distFromSideWall = 100;
int sleepAmount = 10000;
int turning = 0; //(-ve(Left),0(Straight),+ve(Right))

int loopForceTimer = 100000;

/////////////////////////////////
//functions
////////////////////////////////

void turn(int lSpeed, int rSpeed){  //change to main codes turn
  printf("left: %d ... ", lSpeed);
  printf("right: %d\n", rSpeed);
  //set_motor(rightMotor,lSpeed);
  //set_motor(leftMotor,rSpeed);
}

void forward(){ //goes straight
  turn(100,100);
}

void turnOnSpot(int speed){
  printf("%d\n", speed);
  //set_motor(leftMotor,speed);
  //set_motor(rightMotor,(-1)*speed);
}


void scanValueUpdate()
{
// updating values
  //scan_front = read_analog(f); // front
  //scan_right = read_analog(r); // right
  //scan_left = read_analog(l); // left
}

void testTurn(){
  // Tests sensing for wall infront
  if (scan_front > distFromWall ) //if robot is too close to the wall
  {
    if (scan_right > distFromSideWall) //there seems to be no wall there
    {
      turning = 1;
    } 
    else if (scan_left > distFromSideWall)
    {
      turning = -1;
    }
  }
  //reallign if front(/back) is too close on one side if nec
  //change code to mayb?
  if (scan_left > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && turning == 0)
  { // realign right
    turning = -2;
  }
  else if (scan_right > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && turning == 0)
  { // realign left
    turning = 2;
  }
  /*if (scan_left > distFromWall && turning == 0) //change to test to adjust when: one side is bigger than the other, no side is giving ~0 (gap in wall) and is not turning
  { // realign right
    turning = -2;
  }
  else if (scan_right > distFromWall && turning == 0)
  { // realign left
    turning = 2;
  }*/

}

void mazeMove(){
  //turning/reallign function
  if (turning == 1) //turning left
  {
    turn(10,100);
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      turning == 0;
    } 
  }
  else if (turning == -1) //turning right
  {
    turn(100,10); 
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      turning == 0;
    }
  }
  else if (turning == 2) //realign left
  {
    turn(100,50);
    if (scan_right < distFromSideWall)
    { // if scan of each wall is roughly similar
      turning == 0;
    }
  }
  else if (turning == -2) //realign right
  {
    turn(50,100);
    if (scan_left < distFromSideWall)
    {
      turning == 0;
    }
  }
  // going straight or updated to go straight
  if (turning = 0)
  {
    forward();
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
  //  scan_front = i*50+300; //numbers are smaller further away 
  //  scan_right = j*50; //front right
  //  scan_left = 100; //front left
  //  int x=0;
  mazeSection();
  //  printf("front: %d left: %d right: %d turnNum: %d\n",scan_front,scan_left,scan_right,turning);

  //  }
  //}
  /*while (x<loopForceTimer) {
    mazeSection();

    sleep1(0,sleepAmount);
    x++;
  }*/
return 0;
}
