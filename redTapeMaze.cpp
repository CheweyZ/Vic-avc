/////////////////////////////////
// Libs
/////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"

/////////////////////////////////
// New initing of vars
/////////////////////////////////

int red = 0;
int redLineSect = 0;
int redSensFirstTime = 1;
/////////////////////////////////
// maze seciton vars that also is used here
/////////////////////////////////

int scan_front;
int scan_right;
int scan_left;

int leftMotor = 1;
int rightMotor = 2;

/////////////////////////////////
// Code used in (maze) other sections
/////////////////////////////////

void mazeTurn(int lSpeed, int rSpeed){  //change to main codes turn
    printf("left: %d ... ", lSpeed);
    printf("right: %d\n", rSpeed);
    set_motor(rightMotor,lSpeed);
    set_motor(leftMotor,rSpeed);
}

////////////////////////////////////////
// Line follow code (from old branch) //
////////////////////////////////////////

#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
// unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];
int midCameraBlind=5;
int maxMotorSpeed=60;
int reverseSpeed=60;
int lMSpd=maxMotorSpeed;
int rMSpd=maxMotorSpeed;
int readRange=(CAMERA_WIDTH/2)-midCameraBlind;
// Sum 1 to n  https://betterexplained.com/articles/techniques-for-adding-the-numbers-1-to-100/
int boundarySum=(readRange*(readRange+1))/2;
// int rightBoundarySum=((readRange-1)*(readRange))/2;
double effectFactor=1; //the factor that correction effect is 0-100% (0- to 1)
double effectFactorReverse=1.6;
int reverseThreshold=15; //20 prior
int blackWhiteTolerance=30;
int baseWhiteMin=110;

int tThres=700;
// int reverseBoostForShift=70;

int reverseSleepTime=200000;

int loopForceTimer=100;

int updateMotorSpeedBM(){
  int lMSpdL=(lMSpd<0)?abs(lMSpd):-lMSpd;
  int rMSpdL=(rMSpd<0)?abs(rMSpd):-rMSpd;
  
  set_motor(2,rMSpdL); //1 is left motor but backward so inversed
  set_motor(1,lMSpdL);
  return 0;
}

int processExitBM(){
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeedBM();
  return 0;
}

void reverse(int amt){
  rMSpd=-amt;//+50
  lMSpd=-amt;
  updateMotorSpeedBM();
  printf("Sleep Start\n");
  sleep1(0,500000);
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeedBM();
  sleep1(0,500000);
  printf("Sleep End\n");
}

void turnLeftBM(int speed){
  int rSpd=rMSpd;
  int lSpd=lMSpd;
  rMSpd=speed;
  lMSpd=-speed+20;
  updateMotorSpeedBM();
  printf("Sleep Start\n");
  sleep1(0,500000);
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeedBM();
  sleep1(0,20000);
  rMSpd=rSpd;
  lMSpd=lSpd;
  printf("Sleep End\n");
}

int driveWithShiftBM(int directionShift) {
    if (directionShift!=0){
        if (directionShift<0){ //drifted left
            double effectNeeded=((double)abs(directionShift))/boundarySum; // shows total shift and effect needed
            // validate maybe if effect needed is over say 90 or 95 is it maybe a turn that have found
            lMSpd=maxMotorSpeed;
            rMSpd=maxMotorSpeed*(effectFactor*(1-effectNeeded));
            bool didRev=false;
            if (rMSpd<reverseThreshold){
              rMSpd=-(maxMotorSpeed*(effectFactorReverse*(effectNeeded)));//-reverseBoostForShift
              didRev=true;
            }
            updateMotorSpeedBM();
            if (didRev){
              sleep1(0,reverseSleepTime);
              lMSpd=maxMotorSpeed;
              rMSpd=maxMotorSpeed;
              updateMotorSpeedBM();
              sleep1(0,reverseSleepTime);
            }
            // printf("Left drift Lm:%d Rm:%d Ef:%f dS:%d Bound:%d\n",lMSpd,rMSpd,effectNeeded,directionShift,boundarySum);
        }else if (directionShift>0){ //drifted right
            double effectNeeded=((double)directionShift)/boundarySum; // shows total shift and effect needed
            lMSpd=maxMotorSpeed*(effectFactor*(1-effectNeeded));
            bool didRev=false;
            // if (lMSpd<0){lMSpd=0;}
            if (lMSpd<reverseThreshold){
              lMSpd=-(maxMotorSpeed*(effectFactorReverse*(effectNeeded)));//-reverseBoostForShift
              didRev=true;
            }
            rMSpd=maxMotorSpeed;
            updateMotorSpeedBM();
            if (didRev){
              sleep1(0,reverseSleepTime);
              lMSpd=maxMotorSpeed;
              rMSpd=maxMotorSpeed;
              updateMotorSpeedBM();
              sleep1(0,reverseSleepTime);
            }
            // printf("Right drift Lm:%d Rm:%d Ef:%f dS:%d\n",lMSpd,rMSpd,effectNeeded,directionShift);
        }
    } else { // if no shift
        lMSpd = maxMotorSpeed;
        rMSpd = maxMotorSpeed;
    }
    return 0;
}

int cameraScannerBM(){
  int max = 0;
  int min =255;
  int scan_row = 120;
  int secondScanRow=240;
  int secondMinMaxRow=160;
  for (int i = 0; i <320;i++)
{
  int pix = get_pixel(scan_row,i,3);
      if ( pix > max) 
      {
    max = pix;
  }
  if (pix < min)
  {
    min =pix;
  }
  }
  
  for (int i = 0; i <320;i++)
{
  int pix = get_pixel(secondMinMaxRow,i,3);
      if ( pix > max) 
      {
    max = pix;
  }
  if (pix < min)
  {
    min =pix;
  }
  }
  
  int thr = (max+min)/2;
  // printf("min=%d max=%d threshold=%d\n", min, max,thr);
  
  // int whi[320];  // white pixels
  int midLeftPoint=(CAMERA_WIDTH/2)-midCameraBlind;
  int midRightPoint=(CAMERA_WIDTH/2)+midCameraBlind-1;
  // printf("MidLeftPoint:%d MidRightPoint:%d\n", midLeftPoint,midRightPoint);
  int leftShift=0;
  int rightShift=0;
  for (int i = 0; i <320;i++){
  // whi[i]= 0 ;
  int pix = get_pixel(scan_row,i,3);
  // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  if ( pix > thr){
    if (i<midLeftPoint){
      leftShift+=midLeftPoint-i;
    }else if(i>midRightPoint){
      rightShift+=i-midRightPoint;
    }
    // whi[i] = 1;
  }
  }

  if (abs(min-max)<blackWhiteTolerance){
    printf("Black and White range not big enough\n");
    if (min<baseWhiteMin){
      printf("The world is darkness\n");
      reverse(reverseSpeed);
      return 0;
    }else{
      printf("Wow its bright here\n");
    }
  }
    
  // possible check on these is if called read a row a bit above then determine if foward is also option (maybe a function just to return if white strip)
  if (leftShift==boundarySum&& rightShift==boundarySum){ // at a 2 way intersection (should check if can go ahead so look a line up)
    // As C cant return 2 values instead need to copy code (so effecent) with recalulation for a some row up to determin if
    // can go foward or if its only a T
    printf("I can turn both ways\n");
    turnLeftBM(70);// at a T we always turn right
  }else if (leftShift==boundarySum){
    // left turn option
    printf("I can turn left\n");
  }else if (rightShift==boundarySum){
    // right turn option
    printf("I can turn right\n");
  }else if (rightShift==0&& leftShift==0){
    printf("Lets check is there still a path here?\n");
    reverse(reverseSpeed);
  }
  int directionShift=leftShift-rightShift;
  // negative is drifted left +is drifted to right
  driveWithShiftBM(directionShift);
  return 0;
}

/////////////////////////////////
// Main code
/////////////////////////////////

void reverseAndTurn(){
    //runs code that makes the robot
    //  backs out of the maze
    //  and turn 180
    mazeTurn(60,60); //reversing but is facing other direction
    sleep1(5,0);
    mazeTurn(-80,80); //rotates on spot
    sleep1(3,0);
}

void lineFollow(){ /**Needs adjustment*/
    take_picture();
    cameraScannerBM();
}


int redTest() {
    int scaning_row = 100; //temp idk value
    int red_total = 0;
    int returnVal = 0;
    for (int i = 0; i < 320;i++) { //check pixels along an entire line
        int red = get_pixel(scaning_row,i,0); //check for red
        int green = get_pixel(scaning_row,i,1); //check for green
        int blue = get_pixel(scaning_row,i,2); //check for blue
        if ((red > 200) && (green < 100) && (blue < 100)){
            red_total = red_total + red; //add the output to red_total
        }
    }
    printf("red_total = %d \n", red_total);
    if( red_total > 4000) { //if significant enough red has been found
        returnVal = 1;
    }
    return returnVal;
}

void mazeIntroSection(){
    if (red == 0)
    {
        red = redTest();
    }
    if (red == 1)
    {
        if (redSensFirstTime == 1)
        {
	        reverseAndTurn(); //gets out of maze and then turns 180
	        redSensFirstTime = 0;
        }
        lineFollow(); //does line test to get towards maze again but reverse
        if (scan_left > 300 && scan_right > 300)
        {
            redLineSect = 1;
            mazeTurn(50,50); //goes forward to pass the red line
            sleep1(0,500000);
            mazeTurn(0,0);
            red = 0;
        }

    }
}

void mazeSectionDoorTest(){ //should be run at the same time as the maze code
    red = redTest();
    if (red == 1)
    {
        mazeTurn(0,0);
        if (scan_front > 300)
        {
            sleep1(20,0);
            printf("wall was up so waiting\n");        
        }
        while (scan_front < 300)
        {
            sleep1(0,500000);
            printf("wall is down\n");
        } 
        printf("wall is up\n");
        redLineSect = 2;
    }
}

void redLineCode(){
    if (redLineSect == 1)
    {
        mazeIntroSection();    
    }
    else if (redLineSect == 2)
    {
        mazeSectionDoorTest();
    }
}

void temp(){
    if (redLineSect < 2)
    {
        redLineCode();
    }
    if (redLineSect > 0)
    {
        //call mazeSection();
    }

}
    
int main (){
    init();

    for (int i = 0; i < 1000; ++i)
    {
        temp();
        sleep1(0,500000);
    }
    
    mazeTurn(0,0);
    printf("finished!\n");
return 0;
}
