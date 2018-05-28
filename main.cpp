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
  printf("Sleep Start\n");
  sleep1(0,500000);
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeed();
  sleep1(0,500000);
  printf("Sleep End\n");
}

void turnLeft(int speed){
  int rSpd=rMSpd;
  int lSpd=lMSpd;
  rMSpd=speed;
  lMSpd=-speed+20;
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
  
  // int secondaryLeft=0;
  // int secondaryRight=0;
  // for (int i = 0; i <320;i++){
  // // whi[i]= 0 ;
  // int pix = get_pixel(secondScanRow,i,3);
  // // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  // if ( pix > thr){
  //   if (i<midLeftPoint+midCameraBlind){
  //     secondaryLeft+=(midLeftPoint+midCameraBlind)-i;
  //   }else if(i>midRightPoint+midCameraBlind){
  //     secondaryRight+=i-(midRightPoint+midCameraBlind);
  //   }
  //   // whi[i] = 1;
  // }
  // }
  // 
  // // bool canGoFoward
  // if (secondaryLeft>tThres&&secondaryRight>tThres){
  //   printf("I can go foward at this T ?\n");
  // }
  // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  
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
  
  // possible check on these is if called read a row a bit above then determine if foward is also option (maybe a function just to return if white strip)
  if (leftShift==boundarySum&& rightShift==boundarySum){ // at a 2 way intersection (should check if can go ahead so look a line up)
    // As C cant return 2 values instead need to copy code (so effecent) with recalulation for a some row up to determin if
    // can go foward or if its only a T
    printf("I can turn both ways\n");
    turnLeft(70);// at a T we always turn right
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
  driveWithShift(directionShift);
  
//   for (int i = 0; i <320;i++)
// {
//   printf("%d ",whi[i]);
//   }
//   printf("\n");
  return 0;
}

int main (){
  int x=0;
  init();
  // set_motor(1,100);
  // sleep1(5,0);
  // set_motor(1,-100);
  // sleep1(5,0);
  
  while (x<loopForceTimer) {
    take_picture();
    cameraScanner();
    x++;
  }
  processExit();
  return 0;
}
