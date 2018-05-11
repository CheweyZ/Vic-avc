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
int leftMotor = 1;
int rightMotor = 2;

int distFromWall = 600;
int distFromSideWall = 100;
int sleepAmount = 10000;
int turning = 0 //(-ve(Left),0(Straight),+ve(Right))

int turn(int lSpeed, rSpeed){	//change to main codes turn
	printf("%d\n", lSpeed);
	printf("%d\n", rSpeed);
		//set_motor(rightMotor,lSpeed);
		//set_motor(leftMotor,rSpeed);
	return 0;
}
int turnOnSpot(int speed){
	printf("%d\n", speed);
	//set_motor(leftMotor,speed);
	//set_motor(rightMotor,(-1)*speed);
	return 0;
}

int mazeSection()
{
	scan_front = read_analog(f); 
	scan_right1 = read_analog(r1); //front right
	scan_right2 = read_analog(r2);
	scan_left1 = read_analog(l1); //front left
	scan_left2 = read_analog(l2);

// Tests sensing for wall infront
	if (scan_front > distFromWall )
	{
		if (scan_right1 < distFromSideWall) //there seems to be no wall there
		{
			turning = 1;
		} 
		else if (scan_left1 < distFromSideWall)
		{
			turning = -1;
		}


	}


//reallign if front(/back) is too close on one side if nec
//needs turning test
	if (scan_left1 < distFromWall && turning == 0)
	{
		/* turn right */
		turn(100,10);
	}
	else if (scan_right1 < distFromWall && turning == 0)
	{
		// turn left
		turn(10,100);

	}

	if (turning == 1)
	{
		if (scan_front < distFromWall)
		{
			turning == 0;
		}	
	}
	else if (turning == -1)
	{
		if (scan_front < distFromWall)
		{
			turning == 0;
		}
	}else 	if (turning = 0)
	{
		forward();
	}
	sleep1(0,sleepAmount);
	return 0;
}
