//conecting to the gate started working on it - having a mad brain fade today grrr. Apologies on the garbage/
//port is 1024 from arthur
//password is Please
//Supposedly supposed to be 5 lines of code

int speedleftm = 0;
int speedrightm = 0;
int updateMotorSpeed() {
	return 0;
}
//arbitrary value im assuming this works.
int scan_front = 100;
int maxMotorSpeed = 0;

//beginning of actually testing for the wall need a lot of rearrangement
int lookingwall = 0; //testing the wall itself, could assign values to then, not really sure if I need to test for the wall using the scanner itself meaning

int (int walltest) {
	speedleftm = maxMotorSpeed;
	speedrightm = maxMotorSpeed;
	    updateMotorSpeed();
	sleep1(1, 0); //sleep for
		bool walltest = false; //changes if the gate rising is detected
		if (scan_front < 300) {
			if (scan_front > 400) {
				walltest = true;
			}
	}
	return 0;
}
if (walltest == true){
  UI.printf("JET FUEL CANT MELT STEEL BEAMS"\n", );
  //then proceed to sending message??
  char serverAddr[15]("130.195.6.196") //Lots of assumptions.
char message[24]("P","l","e","a","s","e"); //I need to make sure that this method works not sure it does
int conect_to_server (char serverAddr[15], int 1024;)
int recieve_from_server(char message[24]) )
int send_to_server(char message[24])
if (send_to_server == true){
  printf("This is supposed to work I believe\n", );
}
//needs to test for wall infront before sending message
//continues moving after wall goes up (also after delay)
//changes section number
