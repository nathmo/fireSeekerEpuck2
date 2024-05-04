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


#include <IR_proximity.h>
#include <camera.h>
#include <blink.h>
#include <behaviour.h>
#include "main.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);

// Main function
int main(void) {
    halInit();
    chSysInit();
    mpu_init();
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
    //process_camera_start();
    //process_state_machine_start();
    
    // do nothing. the thread will do the work (check behaviour file to understand)
    while(true) {
        chThdSleepMilliseconds(1000); // always sleep in main thread to let other thread time to run
        set_fire_blink_mode(false);
        /*
        if (getFrontRight()) {
            set_fire_blink_mode(true);
            turn_toward_given_sensor(0);
            chThdSleepMilliseconds(1000);
            turn_toward_given_sensor(7);
        } else if (getFrontLeft()){
            set_fire_blink_mode(true);
            turn_toward_given_sensor(7);
            chThdSleepMilliseconds(1000);
            turn_toward_given_sensor(0);
        } else if (getSideRight()){
            set_fire_blink_mode(true);
            turn_toward_given_sensor(1);
            chThdSleepMilliseconds(1000);
            turn_toward_given_sensor(6);
        } else if (getSideLeft()){
            set_fire_blink_mode(true);
            turn_toward_given_sensor(6);
            chThdSleepMilliseconds(1000);
            turn_toward_given_sensor(1);
        } else if (getNoObstacleDetected()){
            set_fire_blink_mode(false);
            chThdSleepMilliseconds(1000);

        }
        */

        // use camera to check if its a fire
/*
        chBSemSignal(&sem_capture_image); // start the image processing workflow
        chBSemWait(&sem_process_image_ready); // wait that the workflow is done
        if (getIsFireDetected()){
            set_fire_blink_mode(true);
        } else {
            set_fire_blink_mode(false); // there is no fire
        }
        chThdSleepMilliseconds(1000);
*/
	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
