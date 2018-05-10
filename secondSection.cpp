//initing the adc reading values
int scan_front;
int scan_right1; //front of the right scans
int scan_right2;
int scan_left1; //front of the left scans
int scan_left2;

// input pin numbers to change on prop code
int f = 0;
int r1 = 1;
int r2 = 2;
int l1 = 3;
int l2 = 4;
// input pin numbers for motors to change on prop code
int leftMotor = 5;
int rightMotor = 6;

int distFromWall = 600;
int distFromSideWall = 100;
int sleepAmount = 10000;

int turn(int speed){
	//set_motor(leftMotor,speed);
	//set_motor(rightMotor,(-1)*speed);
	return 0;
}

int mazeSection()
{
	scan_front = read_analog(f); 
	scan_right1 = read_analog(r1);
	scan_right2 = read_analog(r2);
	scan_left1 = read_analog(l1);
	scan_left2 = read_analog(l2);

// Tests sensing for wall infront
	if (scan_front > distFromWall )
	{
		if (scan_right1 < distFromSideWall) //there seems to be no wall there
		{
			while (/*while dist from wall is particular amount*/)
			{
				turn(100);
			}
			turn(0);
		} else if (scan_left1 < distFromSideWall)
		{
			while (/*while dist from wall is particular amount*/)
			{
				turn(100);
			}
			turn(0);
		}


	}


//reallign if front/back is too close on one side if nec
	if (/* condition */)
	{
		/* code */
	}




// Tests for pink tape 
	if (/*test for pink tape below*/)
	{
//    Waits till door is closed
//    Then waits till door is open
//    Wait afew more secs
//    Then continues on its way


//checks for the second
	}
	sleep1(0,sleepAmount);
	return 0;
}
