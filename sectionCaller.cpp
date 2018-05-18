//===============================================================
// library calling
//===============================================================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"
//===============================================================
//  initialising variables
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

// input pin numbers for motors
int leftMotor = 1;
int rightMotor = 2;

int distFromWall = 600;
int distFromSideWall = 100;
int sleepAmount = 10000;
int mazeTurning = 0; //(-ve(Left),0(Straight),+ve(Right))

/**/

//===============================================================
// FUNCTIONS
//===============================================================
//===============================================================
// gate 1 code
//===============================================================

void passwordGate(){
  char serverAddr[15]={'1','3','0','.','1','9','5','.','6','.','1','9','6'};
  char message[24]={'P','l','e','a','s','e'};
  char password[24];
  connect_to_server(serverAddr, 1024);
  send_to_server(message);
  receive_from_server(password);
  printf(password);
  send_to_server(password);
  sleep1(2,0);
  sectionNum = 2;
}

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
      mazeTurning = 1;
    } 
    else if (scan_left > distFromSideWall)
    {
      mazeTurning = -1;
    }
  }
  //reallign if front is too close on one side
  if (scan_left > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && mazeTurning == 0)
  { // realign right
    mazeTurning = -2;
  }
  else if (scan_right > scan_right + mazeCentraliseThreshold && scan_right > noWallSenseThreshold && mazeTurning == 0)
  { // realign left
    mazeTurning = 2;
  }
}

void mazeMove(){
  //turning/reallign function
  if (mazeTurning == 1) //turning left
  {
    turn(10,100);
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    } 
  }
  else if (mazeTurning == -1) //turning right
  {
    turn(100,10); 
    if (scan_front < distFromWall) //if large enough gap in front of robot
    {
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == 2) //realign left
  {
    turn(100,50);
    if (scan_right < distFromSideWall)
    { // if scan of each wall is roughly similar
      mazeTurning == 0;
    }
  }
  else if (mazeTurning == -2) //realign right
  {
    turn(50,100);
    if (scan_left < distFromSideWall)
    {
      mazeTurning == 0;
    }
  }
  // going straight or updated to go straight
  if (mazeTurning = 0)
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
// for another section
//===============================================================

//for another section



//===============================================================
// main section caller method
//===============================================================
int main()
{
  //set up
  init();

  //repeating section
  while(sectionNum <= 5 /*&& timeLimit > 0*/)
  {
    if (sectionNum == 1)
    { //before gate
      passwordGate();
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
      mazeSection();
    }
    else if (sectionNum == 5)
    { //follow maze after gate
      mazeSection();
    }
    sleep1(0,sleepAmount);
    //timeLimit == timeLimit - 1; //use for testing with time to avoid infinite loops when testing
  }
  return 0;
}
