//initing the adc reading values
int scan_front;
int scan_right1;
int scan_right2;
int scan_left1;
int scan_left2;

// input numbers to change on prop code
int f = 0;
int r1 = 1;
int r2 = 2;
int l1 = 3;
int l2 = 4;

int distFromWall = 600;
int sleepAmount = 10000;


int mazeSection()
{

	scan_front = read_analog(f); 
	scan_right1 = read_analog(r1);
	scan_right2 = read_analog(r2); 
	scan_left1 = read_analog(l1);
	scan_left2 = read_analog(l2); 

// Tests sensing for wall infront
	if (scan_front > 100 ) {
// When finds one -> sensors for walls on either side
// Turns that direction
        int scanDirectionShift = scan_left1 - scan_right1;
        driveWithShift(scanDirectionShift);
	}

// Tests for pink tape 
	if (/*test for pink tape below*/)
		
	{
//    Waits till door is closed
//    Then waits till door is open
//    Wait afew more secs
//    Then continues on its way
	}


	sleep1(0,sleepAmount);
	return 0;
}
