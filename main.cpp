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
int midCameraBlind=10;
int maxMotorSpeed=255;
int lMSpd=maxMotorSpeed;
int rMSpd=maxMotorSpeed;
int readRange=(CAMERA_WIDTH/2)-midCameraBlind;
// Sum 1 to n  https://betterexplained.com/articles/techniques-for-adding-the-numbers-1-to-100/
int boundarySum=(readRange*(readRange+1))/2;
double effectFactor=1; //the factor that correction effect is 0-100% (0- to 1)


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
  set_motor(1,lMSpd);
  set_motor(2,rMSpd);
  return 0;
}

int processExit(){
  rMSpd=0;
  lMSpd=0;
  updateMotorSpeed();
  return 0;
}

int driveWithShift(int directionShift) {
    if (directionShift!=0){
        if (directionShift<0){ //drifted left
            double effectNeeded=((double)abs(directionShift))/boundarySum; // shows total shift and effect needed
            // validate maybe if effect needed is over say 90 or 95 is it maybe a turn that have found
            lMSpd=maxMotorSpeed;
            rMSpd=maxMotorSpeed*(effectFactor*(1-effectNeeded));
            updateMotorSpeed();
            printf("Left drift Lm:%d Rm:%d Ef:%f",lMSpd,rMSpd,effectNeeded);
        }else if (directionShift>0){ //drifted right
            double effectNeeded=((double)directionShift)/boundarySum; // shows total shift and effect needed
            lMSpd=maxMotorSpeed*(effectFactor*(1-effectNeeded));
            rMSpd=maxMotorSpeed;
            updateMotorSpeed();
            printf("Right drift Lm:%d Rm:%d Ef:%f",lMSpd,rMSpd,effectNeeded);
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
  int thr = (max+min)/2;
  printf("min=%d max=%d threshold=%d\n", min, max,thr);
  
  // int whi[320];  // white pixels
  int midLeftPoint=(CAMERA_WIDTH/2)-midCameraBlind;
  int midRightPoint=(CAMERA_WIDTH/2)+midCameraBlind;
  printf("MidLeftPoint:%d MidRightPoint:%d\n", midLeftPoint,midRightPoint);
  int leftShift=0;
  int rightShift=0;
  for (int i = 0; i <320;i++){
  // whi[i]= 0 ;
  int pix = get_pixel(scan_row,i,3);
  if ( pix > thr){
    if (i<midLeftPoint){
      leftShift+=midLeftPoint-i;
      // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
    }else if(i>midRightPoint){
      rightShift+=i-midRightPoint;
    }
    // whi[i] = 1;
  }
  }
  printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
  // possible check on these is if called read a row a bit above then determine if foward is also option (maybe a function just to return if white strip)
  if (leftShift==boundarySum&& rightShift==boundarySum){ // at a 2 way intersection (should check if can go ahead so look a line up)
    // As C cant return 2 values instead need to copy code (so effecent) with recalulation for a some row up to determin if
    // can go foward or if its only a T
    printf("I can turn both ways\n");
  }else if (leftShift==boundarySum){
    // left turn option
    printf("I can turn left\n");
  }else if (rightShift==boundarySum){
    // right turn option
    printf("I can turn right\n");
  }else if (rightShift==0&& leftShift==0){
    printf("Lets check is there still a path here?\n");
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
  while (x<20) {
    take_picture();
    cameraScanner();
    x++;
  }
  processExit();
  return 0;
}
