/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * This robot will patrol randomly an area while bouncing of obstable.
 * It use it's camera to look for the color of a fire (red) and will rush toward it.
 * the "Fire" are piece of paper that are red. The robot will roll them over to "extinguish" it
 * Once there is no more "Fire" , it will resume it's patrol mode.
 * 
 * this file is the entry point and just start the main threads.
 * 
 */

#include <camera/po8030.h>
#include <camera/dcmi_camera.h>
#include <msgbus/messagebus.h>
#include <parameter/parameter.h>
#include <spi_comm.h>

#include "../e-puck2_main-processor/src/sensors/proximity.h"
#include "IR_proximity.h"
#include "camera.h"
#include "blink.h"
#include "behaviour.h"
#include "main.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);
parameter_namespace_t parameter_root;

// Main function
int main(void) {
    halInit();
    chSysInit();
    mpu_init();

    messagebus_init(&bus, &bus_lock, &bus_condvar);
    parameter_namespace_declare(&parameter_root, NULL, NULL);

    //starts the camera
    dcmi_start();
	po8030_start();
    //inits proximity sensors
    
    proximity_start();
	//inits the motors
	motors_init();
	//starts RGB LEDS and User button managment
	spi_comm_start();

    // start all the threads
    process_blink_start(); // thread that deal with the led to make the red and blue light
    process_camera_start(); // process that capture and process the image
    process_state_machine_start(); // process that deal with the high level behaviour of the robot
    while(true) { // the thead in behaviour do the job (among others) main is empty
        chThdSleepMilliseconds(5000); // always sleep in main thread to let other thread time to run
	}
}

#define STACK_CHK_GUARD 0xe2dee396
uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void)
{
    chSysHalt("Stack smashing detected");
}
