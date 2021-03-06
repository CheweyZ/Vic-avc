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

int red1 = 0;
int redLineSect = 1;
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


/////////////////////////////////
// Main code
/////////////////////////////////

void reverseAndTurn(){
    //runs code that makes the robot
    //  backs out of the maze
    //  and turn 180
    mazeTurn(60,60); //reversing but is facing other direction
    sleep1(0,100000);
    mazeTurn(-60,60); //rotates on spot
    sleep1(0,300000);
}

void lineFollow(){ /**Needs adjustment*/
    //take_picture();
    //printf("needs line follow code here\n");
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
    //printf("red_total = %d \n", red_total);
    if( red_total > 4000) { //if significant enough red has been found
        returnVal = 1;
        printf("red line\n");
    }
    return returnVal;
}

void mazeIntroSection(){
    //printf("mazeIntroSection\n");
    if (red1 == 0)
    {
        red1 = redTest();
    }
    if (red1 == 1)
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
            red1 = 0;
        }

    }
}

void mazeSectionDoorTest(){ //should be run at the same time as the maze code
    //printf("mazeDoorTest\n");
    red1 = redTest();
    if (red1 == 1)
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
    //printf("run redLineCode\n");
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
        take_picture();
        redLineCode();
    }
    if (redLineSect > 1)
    {
        //call mazeSection();
    }

}
    
int main (){
    init();
    red1 = 0;
    redLineSect = 1;

    for (int i = 0; i < 200; i++)
    {
        temp();
        sleep1(0,100000);
        //printf("%f %f\n", red1, redLineSect);
    }
    
    mazeTurn(0,0);
    printf("finished!\n");
return 0;
}
