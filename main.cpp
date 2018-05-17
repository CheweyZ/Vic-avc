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
int leftShift=0;
int rightShift=0;
int whitePixSum=0; //used to confirm if tape was found
double effectFactor=1; //the factor that correction effect is 0-100% (0- to 1)

//FOR TESTING WITH PPM IMAGES: DELETE IN FINAL

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

int cameraScanner(int scanRow){
  int max = 0;
  int min =255;
  for (int i = 0; i <320;i++)
{
  int pix = get_pixel(scanRow,i,3);
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
  for (int i = 0; i <320;i++){
  // whi[i]= 0 ;
  int pix = get_pixel(scanRow,i,3);
  if ( pix > thr){
	whitePixSum++;
    if (i<midLeftPoint){
      leftShift+=midLeftPoint-i;
      // printf("LShift:%d Rshift:%d\n", leftShift,rightShift);
    }else if(i>midRightPoint){
      rightShift+=i-midRightPoint;
    }
    // whi[i] = 1;
  }
  int directionShift=leftShift-rightShift;
  // negative is drifted left +is drifted to right
  
//   for (int i = 0; i <320;i++)
// {
//   printf("%d ",whi[i]);
//   }
//   printf("\n");
  driveWithShift(directionShift);
}

int main (){
  int x=0;
  init();
  while (x<1000) {
    take_picture();
    cameraScanner();
    x++;
  }
  return 0;
}
