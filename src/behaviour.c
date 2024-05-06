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

#include <ch.h>
#include <hal.h>
#include <stdbool.h>
#include "camera.h"
#include "IR_proximity.h"
#include "movement.h"
#include "blink.h"
#include "behaviour.h"

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
                    turn_toward_sensor_front_right();
                    arrival_direction = FRONTRIGHT;
                } else if (get_if_collision_front_left()){
                    turn_toward_sensor_front_left();
                    arrival_direction = FRONTLEFT;
                } else if (get_if_collision_side_right()){
                    turn_toward_sensor_side_right();
                    arrival_direction = SIDERIGHT;
                } else if (get_if_collision_side_left()){
                    turn_toward_sensor_side_left();
                    arrival_direction = SIDELEFT;
                }
                state = 2;
                break;
            case 2:
                previous_state = state; // this prevent problem with avancer() who dont work if called too often
                setIsFireDetected(false); // reset the flag so we dont get detection that happened before
                chThdSleepMilliseconds(300); // time to take and process the image
                // use camera to check if its a fire
                if (getIsFireDetected()){
                    state = 4; // there is a fire -> ram into it to extinguish
                    setIsFireDetected(false); // reset the flag so ack that we processed it
                } else {
                    state = 3; // there is no fire -> bounce off 
                }
                break;
            case 3:
                previous_state = state; // this prevent problem with avancer() who dont work if called too often
                // turn away from obstacle in a bouncing fashion
                switch(arrival_direction) {
                    case FRONTRIGHT:
                        turn_specific_angle(165); // 180°-15° (angle of IR sensor relative to front)
                        break;
                    case SIDERIGHT:
                        turn_specific_angle(130); // 180°-50° (angle of IR sensor relative to front)
                        break;
                    case SIDELEFT:
                        turn_specific_angle(-130); // -(180°-50°) (angle of IR sensor relative to front)
                        break;
                    case FRONTLEFT:
                        turn_specific_angle(-165); // -(180°-15°) (angle of IR sensor relative to front)
                        break;
                }
                state = 0; // and return to move foward slow
                break;
            case 4:
                //enable light and rush forward to destroy the fire
                set_fire_blink_mode(true);
                previous_state = state; // this prevent problem with avancer() who dont work if called too often
                avancer(FASTPACE);
                chThdSleepMilliseconds(2000); // give enough time to extinguish the fire
                set_fire_blink_mode(false);
                state = 3; // once done -> we turn away from the fire
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