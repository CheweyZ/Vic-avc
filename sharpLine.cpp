//Need to make int leftShift and int rightShift global variables: move out of camera scanner
//for this to work

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
int f = 1;
int scan_front = 100;
int maxMotorSpeed = 0;
int leftShift = 0;
int whitePixSum=0;
int rightShift = 0;
int sectionNum = 0;
int upperBoundarySum = 3000; //used to check for which ways can be turned
int lowerBoundarySum = 100; //used to check if a line is present at all

//TEST FOR FIRST SECTION CHANGE

void sharpLineTest() {
	int whiteMagnitude = leftShift + rightShift;
	printf("whiteMagnitude = %d\n", whiteMagnitude);
	if (whiteMagnitude > 1500) {
		sectionNum = 3;
	}
}

//CODE BEGINS
//Main method structure= call cameraScanner(), pass result through sharpLine(). Call turn(String direction)

void sharpLine() {
	int closeScan = cameraScanner(70);
	//checks against close line test with the updated shift variables. Then tests the line further away if necessary and checks again,
	if (leftShift >= boundarySum && rightShift >= boundarySum){ //this is a t intersection -> always turn left
		turn("Left");
	}else if (leftShift >= boundarySum){ //left turn is possible
		int farScan = cameraScanner(220);
		if (whitePixSum < lowerBoundarySum) { //there is not a line ahead
			turn("Left");
		} 
	}else if (rightShift==boundarySum){ //right turn is possible
		int farScan = cameraScanner(220);
		if (whitePixSum < lowerBoundarySum) { //there is not a line ahead
			turn("Right");
		}
	}else { //standard line response
		driveWithShift(closeScan);
	}
}
  
