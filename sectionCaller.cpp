//===============================================================
// lib calling
//===============================================================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"
//===============================================================
//	var initing
//===============================================================

/* from section caller section */
int sectionNum = 1;
int sleepAmount = 1000;
// int timeLimit = 10000;

/* from maze section */
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

/**/



//===============================================================
// main section caller method
//===============================================================
int main()
{
	//set up
	init();

	//repeating section
	while(sectionNum <= 5 /*&& timeLimit < 0*/)
	{
		if (sectionNum == 1)
		{ //before gate
			/*call door*/
			/*call follow line*/
		}
		else if (sectionNum == 2)
		{ //follow line
			/*call follow line*/
			//sharpLineTest();
		}
		else if (sectionNum == 3)
		{ //follow sharp line
			//sharpLine();
			/*call test for red line test*/
		}
		else if (sectionNum == 4)
		{ //follow maze before gate
			// call redTest(int scanning_row);
			//mazeSection();
		}
		else if (sectionNum == 5)
		{ //follow maze after gate
			//mazeSection();
		}
		sleep1(0,sleepAmount);
		//timeLimit == timeLimit - 1;
	}
	return 0;
}
//===============================================================
// 
//===============================================================


//===============================================================
// maze section code
//===============================================================

/* needs adjusting to rest of code
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
}*/


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
  if (scan_left > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && turning == 0)
  { // realign right
    turning = -2;
  }
  else if (scan_right > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && turning == 0)
  { // realign left
    turning = 2;
  }
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

//===============================================================
// 
//===============================================================
