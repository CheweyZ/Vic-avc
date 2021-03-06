//Still does not allow for a rest period, so that it does not detect the same piece of red tape to infact be multiple
//Also needs color tests to include lack of green/blue otherwise could trigger on white

// DELETE ALL BELOW DECLARATIONS, ONLY FOR COMPILATION
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "E101.h"
int lMSpd = 0;
int rMSpd = 0;
int updateMotorSpeed() {
	return 0;
}
int frontSensor = 1;
int scan_front = 100;
int maxMotorSpeed = 0;
bool gateUp = false;

//CODE BEGINS

int stage = 0; // 0 = never passed red tape, 1 = seen it once, 2 = seen it twice

int redStage(int stage) { //responds to red tape events
	lMSpd = maxMotorSpeed;
	rMSpd = maxMotorSpeed;
	updateMotorSpeed(); //Move robot forward past detecting the red tape
	sleep1(1, 0); //sleep for one second so that the same red tape is not detected again
	if(stage == 2) {
		lMSpd = 0;
		lMSpd = 0;
		updateMotorSpeed();
		bool gateUP = false; //changes if the gate rising is detected
		if (scan_front < 300) { //if the gate is already up
			sleep1(20, 0); //wait for a full rotation -> gate is guaranteed to be down
		}
		while(!gateUP) { //while the gate has not gone up after arriving
			int scan_front = read_analog(frontSensor); 
			if (scan_front > 400) {
				gateUP = true;
			}
		} //gate is now up, continue with standard code and drive forward
	}
	return 0;
}

int redTest(int scaning_row) {
     int red_total = 0;
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
         stage = stage + 1; //increment redTape
         redStage(stage); //call red

     }
     return stage;
 }

int main() {
	init();
	int x = 0;
	while (x < 100) {
		take_picture();
		redTest(100); //changed var to avoid overlapping vars when combining code
		x= x +1;
	}
}






/////////////////////////

bool redTest(int scaning_row) {
    int red_total = 0;
   for (int i = 0; i < 320;i++) { //check pixels along an entire line
        int red = get_pixel(scaning_row,i,0); //check for red
        int green = get_pixel(scaning_row,i,1); //check for green
        int blue = get_pixel(scaning_row,i,2); //check for blue
        if ((red > 200) && (green < 100) && (blue < 100)){
            red_total = red_total + red; //add the output to red_total
        }
    }
     if( red_total > 1000) { //if significant enough red has been found
        return true;
    } else {
    	return false;     	
    }
}
