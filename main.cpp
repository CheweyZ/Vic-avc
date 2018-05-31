#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"
// testing pic3.ppm right drift 
// picq.ppm left drift
// pick.ppm is left turn

#define CAMERA_WIDTH 320 //Control Resolution from Camera
#define CAMERA_HEIGHT 240 //Control Resolution from Camera
// unsigned char pixels_buf[CAMERA_WIDTH*CAMERA_HEIGHT*4];
int midCameraBlind=5;
int maxMotorSpeed=55; //best 60
int reverseSpeed=80;
int lMSpd=maxMotorSpeed;
int rMSpd=maxMotorSpeed;
int readRange=(CAMERA_WIDTH/2)-midCameraBlind;
// Sum 1 to n  https://betterexplained.com/articles/techniques-for-adding-the-numbers-1-to-100/
int readSkip=2;
int boundarySum=((readRange-readSkip)*((readRange+1)-readSkip))/2;
// int rightBoundarySum=((readRange-1)*(readRange))/2;
double effectFactor=1; //the factor that correction effect is 0-100% (0- to 1)
double effectFactorReverse=1.6;
int reverseThreshold=15; //15 prior
int blackWhiteTolerance=30;
int baseWhiteMin=110;

bool ivSeenRed=false;

int tThres=200;//was 200
// bool iJustWentFoward=false;
// int reverseBoostForShift=70;

int reverseSleepTime=200000;

int turnIgnoreTime= 300000;

int loopForceTimer=3500;


int scan_front;
int scan_right;
int scan_left;

// input pin numbers
int l = 0;
int f = 1;
int r = 2;

bool swingLeft = false;
// input pin numbers for motors
int leftMotor = 1;
int rightMotor = 2;

// var to know if turning/adjusting/going straight while in maze
int mazeTurning = 0; //(-ve(Left),0(Straight),+ve(Right))

// returns color component (color==0 -red,color==1-green,color==2-blue
// color == 3 - luminocity
// for pixel located at (row,column)
// unsigned char get_pixel( int row,int col, int color)
// {
//     // calculate address in 1D array of pixels
//     int address = CAMERA_WIDTH*row*3 + col*3;
//     if ((row < 0 ) || (row > CAMERA_HEIGHT) )
//     {
//         printf("row is out of range\n");
//         return -1;
//     }
//     if ( (col< 0) || (col > CAMERA_WIDTH))
//     {
//         printf("column is out of range\n");
//         return -1;
//     }
// 
//     if (color==0)
//     {
//         return (pixels_buf[address]);
//     }
//     if (color==1)
//     {
//         return (pixels_buf[address + 1]);
//     }
//     if (color==2)
//     {
//         return (pixels_buf[address + 2]);
//     }
//     if (color==3)
//     {
//         unsigned char y = (pixels_buf[address] + pixels_buf[address+1] +pixels_buf[address+2])/3;
//         return y;
//     }
//     printf("Color encoding wrong: 0-red, 1-green,2-blue,3 - luminosity\n");
//     return -2; //error
// }

int updateMotorSpeed(){
  int lMSpdL=(lMSpd<0)?abs(lMSpd):-lMSpd;
  int rMSpdL=(rMSpd<0)?abs(rMSpd):-rMSpd;
  // if (rMSpd<0){rMspdL=abs(rMSpd)}else{rMspdL}
  
  set_motor(2,rMSpdL); //1 is left motor but backward so inversed
  set_motor(1,lMSpdL);
  return 0;
}

int processExit(){
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeed();
  return 0;
}

void reverse(int amt){
  rMSpd=-amt;//+50
  lMSpd=-amt;
  updateMotorSpeed();
  // printf("Sleep Start\n");
  sleep1(0,500000);
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeed();
  sleep1(0,500000);
  // printf("Sleep End\n");
}

void mazeTurn(int lSpeed, int rSpeed){  //change to main codes turn
    // printf("left: %d ... ", lSpeed);
    // printf("right: %d\n", rSpeed);
    set_motor(2,lSpeed);
    set_motor(1,rSpeed);
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
  sleep1(0,200000);
  // printf("backing back");
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
  
  if (scan_right < 250 && scan_left < 250 && mazeTurning%2 == 0)
{
	if (swingLeft)
	{
		// mazeTurning = -2;
    mazeTurn(60,-60);
    sleep1(0,150000);
    // mazeTurn(-60,-60);
    // sleep1(0,200000);
	}
	else
	{
		// mazeTurning = 2;
    mazeTurn(-60,60);
    sleep1(0,150000);
	}
	swingLeft = !swingLeft;
  return;
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
  // printf("front: %d left: %d right: %d turnNum: %d\n",scan_front,scan_left,scan_right,mazeTurning);
  // going straight or updated to go straight
  if (mazeTurning == 0)
  {
    mazeForward();
  }

}

void reverseAndTurn(){
    //runs code that makes the robot
    //  backs out of the maze
    //  and turn 180
    mazeTurn(maxMotorSpeed,maxMotorSpeed); //reversing but is facing other direction
    sleep1(1,0);
    mazeTurn(-maxMotorSpeed,maxMotorSpeed); //rotates on spot
    sleep1(1,400000);
    mazeTurn(maxMotorSpeed,maxMotorSpeed); //reversing but is facing other direction
    sleep1(1,0);
    mazeTurn(maxMotorSpeed,-maxMotorSpeed); //rotates on spot
    sleep1(0,300000);
}

void iCanGoFoward(){
  // printf("Can also go foward\n");
  lMSpd=maxMotorSpeed;
  rMSpd=maxMotorSpeed;
  updateMotorSpeed();
  // sleep1(0,turnIgnoreTime);
}

void turnLeft(int speed){
  int rSpd=rMSpd;
  int lSpd=lMSpd;
  rMSpd=speed;
  lMSpd=-speed+10;
  updateMotorSpeed();
  printf("Sleep Start\n");
  sleep1(0,500000);
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeed();
  sleep1(0,20000);
  rMSpd=rSpd;
  lMSpd=lSpd;
  printf("Sleep End\n");
}

int driveWithShift(int directionShift) {
    if (directionShift!=0){
        if (directionShift<0){ //drifted left
            double effectNeeded=((double)abs(directionShift))/boundarySum; // shows total shift and effect needed
            // validate maybe if effect needed is over say 90 or 95 is it maybe a turn that have found
            lMSpd=maxMotorSpeed;
            rMSpd=maxMotorSpeed*(effectFactor*(1-effectNeeded));
            bool didRev=false;
            // if (rMSpd<0){rMSpd=0;}
            // printf("RM Spd%d\n", rMSpd);
            if (rMSpd<reverseThreshold){
              rMSpd=-(maxMotorSpeed*(effectFactorReverse*(effectNeeded)));//-reverseBoostForShift
              didRev=true;
            }
            updateMotorSpeed();
            if (didRev){
              sleep1(0,reverseSleepTime);
              lMSpd=maxMotorSpeed;
              rMSpd=maxMotorSpeed;
              updateMotorSpeed();
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
            updateMotorSpeed();
            if (didRev){
              sleep1(0,reverseSleepTime);
              lMSpd=maxMotorSpeed;
              rMSpd=maxMotorSpeed;
              updateMotorSpeed();
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

int cameraScanner(){
  int max = 0;
  int min =255;
  int scan_row = 230; //max of 239
  int secondScanRow=0;
  int secondMinMaxRow=100;
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
  int red_total = 0;
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
  int red = get_pixel(scan_row,i,0); //check for red
  int green = get_pixel(scan_row,i,1); //check for green
  int blue = get_pixel(scan_row,i,2); //check for blue
  if ((red > 200) && (green < 100) && (blue < 100)){
      red_total = red_total + red; //add the output to red_total
  }
  }
  
   if( red_total > 1000) {
     reverseAndTurn();
     ivSeenRed=true;
     return 0;
   }
  
  int secondaryLeft=0;
  int secondaryRight=0;
  for (int i = 0; i <320;i++){
  // whi[i]= 0 ;
  int pix = get_pixel(secondScanRow,i,3);
  // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  if ( pix > thr){
    if (i<(midLeftPoint+midCameraBlind)){
      secondaryLeft+=(midLeftPoint+midCameraBlind)-i;
    }else if(i>(midRightPoint-midCameraBlind+1)){
      secondaryRight+=i-(midRightPoint-midCameraBlind);
    }
    // whi[i] = 1;
  }
  }
  // 
  bool canGoFoward=false;
  if (secondaryLeft>tThres||secondaryRight>tThres){
    // printf("I can go foward at this T ?\n");
    canGoFoward=true;
  }
  // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  // printf("LShift:%d Rshift:%d\n", secondaryLeft,secondaryRight);
  
  // printf("Min %d Max %d\n",min,max );
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
  
  // if (loopForceTimer%2==0){ //half the logs as drops connection
    // printf("Left:%d Right: %d Sum:%d\n", leftShift,rightShift,boundarySum);
    // Left max = 11325  right max=11175 (right is less due to blind spot)
  // }
  
  int directionShift=leftShift-rightShift;
  // possible check on these is if called read a row a bit above then determine if foward is also option (maybe a function just to return if white strip)
  if (leftShift==boundarySum&& rightShift==boundarySum){ // at a 2 way intersection (should check if can go ahead so look a line up)
    // As C cant return 2 values instead need to copy code (so effecent) with recalulation for a some row up to determin if
    // can go foward or if its only a T
    printf("I can turn both ways\n");
    if (canGoFoward){
      iCanGoFoward();;return 0;};
    // if (!iJustWentFoward){
      turnLeft(70);// at a T we always turn right
    // }else{
    //   lMSpd = maxMotorSpeed;
    //   rMSpd = maxMotorSpeed;
    //   updateMotorSpeed();
    //   sleep1(0,100000);
    // }
    // iJustWentFoward=!iJustWentFoward;
  }else if (leftShift>=boundarySum){
    // left turn option
    // printf("I can turn left\n");
    if (canGoFoward){iCanGoFoward();return 0;};
  }else if (rightShift>=boundarySum){
    // right turn option
    // printf("I can turn right\n");
    if (canGoFoward){iCanGoFoward();return 0;};
  }else if (rightShift==0&& leftShift==0){
    printf("Lets check is there still a path here?\n");
    reverse(reverseSpeed);
  }
  // negative is drifted left +is drifted to right
  driveWithShift(directionShift);
  
//   for (int i = 0; i <320;i++)
// {
//   printf("%d ",whi[i]);
//   }
//   printf("\n");
  return 0;
}

void mazeSection()
{
  scanValueUpdate();
  testTurn();
  mazeMove();
}

void passwordGate(){
	char serverAddr[15]={'1','3','0','.','1','9','5','.','6','.','1','9','6'};
	char message[24]={'P','l','e','a','s','e'};
	char password[24];
	connect_to_server(serverAddr, 1024);
	send_to_server(message);
	receive_from_server(password);
	printf(password);
	send_to_server(password);
}

int main (){
  int x=0;
  init();
  
  // reverseAndTurn();
  passwordGate();
  sleep1(1,0);
  
  
  // set_motor(1,100);
  // sleep1(5,0);
  // set_motor(1,-100);
  // sleep1(5,0);
  
  // while (x<loopForceTimer) {
  while (true) {
    take_picture();
    if (!ivSeenRed){
      cameraScanner();
    }else{
      mazeSection();
    }
    // sleep1(0,50000);
    x++;
  }
  processExit();
  return 0;
}
