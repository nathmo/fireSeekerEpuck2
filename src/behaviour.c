/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : behaviour.c
 * 
 * This file contains the high level logic of the state machine of the robot.
 * 
 * TODO : implement a thread to modele that state machine and pass the state as semaphore to other thread.  (Nathann Morand)
 * 
      ┌─────────────────────────┬───────────────────────────────┐
      │                         │                               │
      │                         │                               │
      │                         │                               │
      │         ┌─────────┐     │                               │
      │         │         │     │                               │
      ▼         ▼         │     │                               │
┌───────────────────┐     │     │       ┌──────────┐            │
│                   │     │     │       │          │            │
│ Move forward slow │     │     │       │          │            │
│                   │     │     │false  │ true     │            │
└────────┬──────────┘     │     │       │          │            │
         │                │     │       │          │            │
         │                │ ┌──┬┴───────┴────┬──┐  │            │
         │                │ │  │ IF obstable │  │  │            │
         │                │ │  │ on the front│  │  │            │
         ▼                │ │  │ half circle │  │  │            │
┌──┬─────────────┬──┐     │ └──┴─────────────┴──┘  │            │
│  │ IF obstable │  │     │           ▲            │            │
│  │ on the front│  │     │           │            │            │
│  │ half circle │  │     │           │            │            │
└──┴─┬───────┬───┴──┘     │           │            │            │
     │       │            │           │            ▼            │
True │       │ False      │           │  ┌──┬─────────────┬──┐  │
     │       └────────────┘           │  │  │If time out  │  │  │
     │                                │  │  │             │  │  │
     ▼                                │  │  │occured      │  │  │
┌───────────────────┐                 │  └──┴─┬───────┬───┴──┘  │
│ turn toward the   │                 │       │       │ True    │
│                   │                 │       │       │         │
│ obstacle          │                 │       │       │         │
└─────────┬─────────┘                 │       │       │         │
          │                           │       │ False │         │
          │                           │       │       │         │
          │                           │       └────┐  │         │
          │                           │            │  │         │
          ▼                           │            │  │         │
┌───────────────────┐       ┌─────────┴─────────┐  │  │         │
│                   │       │ enable light      │  │  │         │
│   get an image    │       │ and sound effect  │◄─┘  │         │
│                   │       │ and roll fast fwd │     │         │
└─────────┬─────────┘       └───────────────────┘     │         │
          │                           ▲               │         │
          │                           │               │         │
          │                           │               │         │
          │                           │               │         │
          ▼                           │               │         │
┌──┬─────────────┬──┐                 │               │         │
│  │ If fire is  │  │                 │               │         │
│  │ Detected on │  │                 │               │         │
│  │ the image   │  │                 │               │         │
└──┴┬──────────┬─┴──┘                 │               │         │
    │          │ True                 │               │         │
    │          └──────────────────────┘               │         │
    │ False                                           │         │
    │          ┌──────────────────────────────────────┘         │
    ▼          ▼                                                │
┌───────────────────┐                                           │
│ turn away from    │                                           │
│ the obstacle      ├───────────────────────────────────────────┘
│                   │                                            
└───────────────────┘                                            
*/

#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <usbcfg.h>
#include <camera.h>
#include <behaviour.h>

static THD_WORKING_AREA(WAstate_machine, 64); // allocate memory for the tread extinguish_blink_pattern
static THD_FUNCTION(state_machine, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
            // Initial state
    int state = 0;

    // State machine loop
    while (TRUE) {
        // Switch-case statement to handle states
        switch(state) {
            case 0:
                // Move forward slow
                if(getNoObstacleDetected()) {
                    // TODO call start motor
                    state = 0; // if there is no collision, we keep the course motor
                } else {
                    // TODO call stop motor
                    state = 1; // if there is a collision, we stop the motor
                }
                break;
            case 1:
                // turn toward obstacle
                if (getFrontRight()) {
                    orientation_sensor(1);
                } else if (getFrontLeft()){
                    orientation_sensor(8);
                } else if (getSideRight()){
                    orientation_sensor(2);
                } else if (getSideLeft()){
                    orientation_sensor(7);
                }
                state = 2;
                break;
            case 2:
                // use camera to check if its a fire
                chBSemSignal(&sem_capture_image); // start the image processing workflow
                chBSemWait(&sem_process_image_ready); // wait that the workflow is done
                if (getIsFireDetected()){
                    state = 4; // there is a fire
                } else {
                    state = 3; // there is no fire
                }
                break;
            case 3:
                // turn away from obstacle
                state = 0; // and return to move foward slow
                break;
            case 4:
                //enable light and sound and rush forward
                // also start the timeout process
                state = 0; // if the fire was extinguished (no more obstacle)
                state = 5; // if there is still an obstacle -> state to check timeout
                break;
            case 5:
                //increment timeout counter.
                state = 4; // if no timeout we go back to extinguishing the fire
                state = 3; // if timeout, we turn away from obstacle
                break;
            case 6:
                
                state = 7;
                break;
            case 7:
                
                state = 0;  // Go back to the initial state
                break;
            default:
                state = 0;  // Go back to the initial state
                break;
        }
        chThdSleepMilliseconds(2);
    }
}

void process_state_machine_start(void){
    chThdCreateStatic(WAstate_machine, sizeof(WAstate_machine), NORMALPRIO, state_machine, NULL);
}