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

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

// Main function
int main(void) {
    messagebus_init(&bus, &bus_lock, &bus_condvar);

    // init the peripheral
    halInit();
    chSysInit();
    //starts the camera
    dcmi_start();
	po8030_start();
	//inits the motors
	motors_init();
	//starts RGB LEDS and User button managment
	spi_comm_start();

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

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
