/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : behaviour.c
 * 
 * This file contains the high level logic of the state machine of the robot.
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

#include <camera.h>
#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <usbcfg.h>
#include <behaviour.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>
#include <stdbool.h>
#include "IR_proximity.h"
#include "movement.h"
#include "blink.h"

static THD_WORKING_AREA(WAstate_machine, 1024); // allocate memory for the tread extinguish_blink_pattern
static THD_FUNCTION(state_machine, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    uint8_t state = 0; // Initial state
    uint8_t arrival_direction = 0;
    uint8_t timeout_extinguish = 0;
    // State machine loop
    while (true) {
        switch(state) {
            case 0:
                // Move forward slow
                if(get_if_front_collision()) {
                    stop_engines();
                    chThdSleepMilliseconds(50);
                    state = 1;    // if there is a collision, we stop the motor
                } else {
                    avancer(100); // if there is no collision, we keep the course
                    chThdSleepMilliseconds(200);
                    state = 0;
                }
                break;
            case 1:
                // turn toward obstacle
                if (get_if_collision_front_right()) {
                    turn_toward_given_sensor(0);
                    arrival_direction = 0;
                } else if (get_if_collision_front_left()){
                    turn_toward_given_sensor(7);
                    arrival_direction = 7;
                } else if (get_if_collision_side_right()){
                    turn_toward_given_sensor(1);
                    arrival_direction = 1;
                } else if (get_if_collision_side_left()){
                    turn_toward_given_sensor(6);
                    arrival_direction = 6;
                }
                state = 2;
                break;
            case 2:
                // use camera to check if its a fire
                if (getIsFireDetected()){ //getIsFireDetected()
                    state = 4; // there is a fire
                    timeout_extinguish = 0;
                    setIsFireDetected(false); // reset the flag so ack that we process it
                } else {
                    state = 3; // there is no fire
                }
                break;
            case 3:
                set_fire_blink_mode(false);
                // turn away from obstacle in a bouncing fashion
                 switch(arrival_direction) {
                    case 0:
                        turn_specific_angle(165);
                        break;
                    case 1:
                        turn_specific_angle(130);
                        break;
                    case 6:
                        turn_specific_angle(-130);
                        break;
                    case 7:
                        turn_specific_angle(-165);
                        break;
                 }
                state = 0; // and return to move foward slow
                break;
            case 4:
                //enable light and rush forward
                set_fire_blink_mode(true);
                if(get_if_front_collision()) { // if the fire is not extinguished
                    avancer(200);
                    chThdSleepMilliseconds(50);
                    state = 5; // if there is still an obstacle -> state to check timeout
                } else {
                    set_fire_blink_mode(false);
                    stop_engines();
                    state = 0; // if there is no collision, we keep go back to state 0
                }
                break;
            case 5:
                timeout_extinguish++;
                //increment timeout counter.
                if(timeout_extinguish<60){
                    state = 4; // if no timeout we go back to extinguishing the fire
                } else {
                    state = 3; // if timeout, we turn away from obstacle
                }
                break;
            case 6:
                state = 0;
                break;
            case 7:
                state = 0;  // Go back to the initial state
                break;
            default:
                state = 0;  // Go back to the initial state
                break;
        }
        chThdSleepMilliseconds(100);
    }
}

void process_state_machine_start(void){
    chThdCreateStatic(WAstate_machine, sizeof(WAstate_machine), NORMALPRIO, state_machine, NULL);
}