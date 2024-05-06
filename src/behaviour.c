/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : behaviour.c
 * 
 * This file contains the high level logic of the state machine of the robot.
 * 
      ┌─────────────────────────────────────────────────────────┐
      │                                                         │
      │                                                         │
      │                                                         │
      │         ┌─────────┐                                     │
      │         │         │                                     │
      ▼         ▼         │                                     │
┌───────────────────┐     │                                     │
│                   │     │                                     │
│ Move forward slow │     │                                     │
│                   │     │                                     │
└────────┬──────────┘     │                                     │
         │                │                                     │
         │                │                                     │
         │                │                                     │
         │                │                                     │
         ▼                │                                     │
┌──┬─────────────┬──┐     │                                     │
│  │ IF obstable │  │     │                                     │
│  │ on the front│  │     │                                     │
│  │ half circle │  │     │                                     │
└──┴─┬───────┬───┴──┘     │                                     │
     │       │            │                                     │
True │       │ False      │                                     │
     │       └────────────┘                                     │
     │                                                          │
     ▼                                                          │
┌───────────────────┐                                           │
│ turn toward the   │                                           │
│                   │                                           │
│ obstacle          │                                           │
└─────────┬─────────┘                                           │
          │                                                     │
          │                                                     │
          │                                                     │
          │                                                     │
          ▼                                                     │
┌───────────────────┐                                           │
│                   │                                           │
│   get an immage   │                                           │
│                   │                                           │
└─────────┬─────────┘                                           │
          │                                                     │
          │                                                     │
          │                                                     │
          │                                                     │
          ▼                                                     │
┌──┬─────────────┬──┐                                           │
│  │ If fire is  │  │                                           │
│  │ Detected on │  │                                           │
│  │ the image   │  │                                           │
└──┴┬──────────┬─┴──┘                                           │
    │          │ True                                           │
    │          └──────────┐                                     │
    │                     │                                     │
    │                     ▼                                     │
    │          ┌───────────────────┐                            │
    │          │ enable light      │                            │
    │          │ and sound effect  │                            │
    │          │ and roll fast fwd │                            │
    │          └──────────┬────────┘                            │
    │                     │                                     │
    │ False               │                                     │
    │          ┌──────────┘                                     │
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
    uint8_t previous_state = -1; // we check if we actually changed state since last time
    uint8_t arrival_direction = 0;
    // State machine loop
    while (true) {
        switch(state) {
            case 0:
                // Move forward slow
                if(get_if_front_collision()) {
                    stop_engines();
                    state = 1;    // if there is a collision, we stop the motor
                } else {
                    if(state != previous_state) // we check if we actually changed state since last time
                    {// this prevent problem with avancer() who dont work if called to often
                        previous_state = state;
                        avancer(SLOWPACE); // if there is no collision, we keep the course
                        state = 0;
                    }
                }
                break;
            case 1:
                previous_state = state; // this prevent problem with avancer() who dont work if called to often
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
                previous_state = state; // this prevent problem with avancer() who dont work if called to often
                setIsFireDetected(false); // reset the flag so ack that we processed it
                chThdSleepMilliseconds(300);
                // use camera to check if its a fire
                if (getIsFireDetected()){ //getIsFireDetected()
                    state = 4; // there is a fire
                    setIsFireDetected(false); // reset the flag so ack that we processed it
                } else {
                    state = 3; // there is no fire
                }
                break;
            case 3:
                previous_state = state; // this prevent problem with avancer() who dont work if called to often
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
                //enable light and rush forward to destry the fire
                set_fire_blink_mode(true);
                previous_state = state; // this prevent problem with avancer() who dont work if called to often
                avancer(FASTPACE);
                chThdSleepMilliseconds(2000); // give enough time to extinguish the fire
                set_fire_blink_mode(false);
                state = 3; // we turn away from the fire
                break;
            default:
                state = 0;  // Go back to the initial state
                break;
        }
        chThdSleepMilliseconds(50);
    }
}

void process_state_machine_start(void){
    chThdCreateStatic(WAstate_machine, sizeof(WAstate_machine), NORMALPRIO, state_machine, NULL);
}