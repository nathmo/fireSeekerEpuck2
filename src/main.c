/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * This robot will patrol randomly an area while bouncing of obstable.
 * It use it's camera to look for the color of a fire (red) and will rush toward it.
 * the "Fire" are piece of paper that are red. The robot will roll them over to "extinguish" it
 * Once no more "Fire" is detected or that the timeout occure, it will resume it's patrol mode.
 * 
 * this file is the entry point and just start the main thread.
 * 
 * TODO : start main threads  (Nathann Morand)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Main function
int main(void) {
    // init the peripheral
    
    // start all the threads
    process_IR_proximity_start();
    process_blink_start();
    process_camera_start();
    process_state_machine_start();
    
    // do nothing. the thread will do the work (check behaviour file to understand)
    while(1) {
		chThdSleepMilliseconds(1000);
	}
}
