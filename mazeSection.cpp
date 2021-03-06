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

// var to know if turning/adjusting/going straight while in maze
int mazeTurning = 0; //(-ve(Left),0(Straight),+ve(Right))

// vars for repeat amounts (also used in other section)
int sleepAmount = 10000; //time between each iteration
int loopForceTimer = 600; //for testing

//////////////////////////////////
//functions           //
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

/*
void turnOnSpot(int speed){
  printf("%d\n", speed);
  set_motor(leftMotor,speed);
  set_motor(rightMotor,(-1)*speed);
}
*/


void scanValueUpdate()
{
// updating values
  scan_front = read_analog(f); // front
  scan_right = read_analog(r); // right
  scan_left = read_analog(l); // left
}

void testTurn(){
  // Tests sensing for wall infront
  if (scan_front > 440 ) //if robot is too close to the front
  {
    if (scan_right < 300 ) //there seems to be no wall: r
    {
      mazeTurning = 1;
    } 
    else if (scan_left < 300 ) //there seems to be no wall: l
    {
      mazeTurning = -1;
    }
    else //there is no free wall f, r & l
    {
      mazeTurning = 10;
    }
  }

  if (scan_front > 490) //when robot is way too close to wall
  {
  mazeTurn(-60,-60);
  sleep1(0,350000);
  printf("backing back");
  }

  //reallign if front is too close on one side if nec
  if (scan_left > 400 && mazeTurning%2 == 0)
  { // realign right
    mazeTurning = 2;
  }
  else if (scan_right > 400 && mazeTurning%2 == 0)
  { // realign left
    mazeTurning = -2;
  }

}

void mazeMove(){
  //Turning/reallign function

  if (mazeTurning == 1) //Turning right
  {
    mazeTurn(60,-60);
    if (scan_front < 400) //if large enough gap in front of robot
    {
      mazeTurning = 0;
      if (scan_left < 300 && scan_right < 300)
      {
	      mazeTurn(60,-60);
	      sleep1(0,150000);
  	  }
    } 
  }
  else if (mazeTurning == -1) //Turning left
  {
    mazeTurn(-60,60); 
    if (scan_front < 400) //if large enough gap in front of robot
    {
      mazeTurning = 0;
      if (scan_left < 300 && scan_right < 300)
      {
	      mazeTurn(-60,60);
	      sleep1(0,150000);
      }

    }
  }
  else if (mazeTurning == 2) //realign right
  {
    mazeTurn(60,45);
    if (scan_left < 400)
    { // if scan of each wall is roughly similar
      mazeTurning = 0;
    }
  }
  else if (mazeTurning == -2) //realign left
  {
    mazeTurn(45,60);
    if (scan_right < 400)
    {
      mazeTurning = 0;
    }
  }
  else if (mazeTurning == 10)
  { // going backwards but shouldnt be needed to be called
    mazeTurn(-50,-50);
    if (scan_front < 400)
    {
      mazeTurning = 0;
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
  int x = 0;
  while (x<loopForceTimer) {
    mazeSection();

    sleep1(0,sleepAmount);

    x++;
  }
  mazeTurn(0,0); 
  printf("finished!\n");
return 0;
}















/////////

bool swingLeft = false;


if (scan_right < 250 && scan_left < 250 && mazeTurning%2 == 0)
{
	if (swingLeft)
	{
		mazeTurning = 2;
	}
	else
	{
		mazeTurning = -2;
	}
	swingLeft = !swingLeft;
}

