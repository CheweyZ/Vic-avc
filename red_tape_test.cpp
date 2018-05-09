//Still does not allow for a rest period, so that it does not detect the same piece of red tape to infact be multiple

int redTape = 0; // 0 = never passed red tape, 1 = seen it once, 2 = seen it twice

int redTest(int scaning_row) {
     int red_total = 0;
     for (int i = 0; i <320;i++) { //check pixels along an entire line
        int pix = get_pixel(scaning_row,i,0); //check for red pixels
        int red_total = red_total + pix; //add the output to red_total
     }
     if( red_total > 1000) { //if significant enough red has been found
         redTape++; //increment redTape
     }
     return redTape;
 }
