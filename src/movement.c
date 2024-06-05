/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : movement.c
 * 
 * This file contains the function related to the robot movement
 * 
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../e-puck2_main-processor/src/motors.h"
#include <math.h>
#include "movement.h"

void turn_toward_sensor_front_right(void) {
    turn_toward_given_sensor(INDEXFRONTRIGHT);
}

void turn_toward_sensor_front_left(void) {
    turn_toward_given_sensor(INDEXFRONTLEFT);
}

void turn_toward_sensor_side_right(void) {
    turn_toward_given_sensor(INDEXSIDERIGHT);
}

void turn_toward_sensor_side_left(void) {
    turn_toward_given_sensor(INDEXSIDELEFT);
}

void turn_toward_given_sensor(uint8_t sensor_index) {
    const int16_t sensor_angles[8]={15, 50, 90, 150, -150, -90, -50, -15};
    int16_t number_of_step = (sensor_angles[sensor_index]*9)/2; // integer ratio so no float :) -> experimentally determined
    int8_t direction = FORWARD; 
    left_motor_set_pos(0);// reset step counter so we can wait using the while loop
    right_motor_set_pos(0);
    if(number_of_step<0){ //used to deal with negative angle -> invert direction of rotation
        direction=(BACKWARD);
    }
    left_motor_set_speed(direction*(TURNINGPACE));
    right_motor_set_speed(direction*(-TURNINGPACE));
    while(left_motor_get_pos() != number_of_step){
        chThdSleepMilliseconds(1); // dont block other threads while turning
    }
    while(right_motor_get_pos() != -number_of_step){
        chThdSleepMilliseconds(1); // dont block other threads while turning
    }
    left_motor_set_speed(0);
    right_motor_set_speed(0); 
}

void turn_specific_angle(int16_t angle_degrees) {
    int16_t number_of_step = (angle_degrees*9)/2; // integer ratio so no float :) -> experimentally determined
    int8_t direction = FORWARD; 
    left_motor_set_pos(0); // reset step counter so we can wait using the while loop
    right_motor_set_pos(0);
    if(number_of_step<0){//used to deal with negative angle -> invert direction of rotation
        direction=(BACKWARD);
    }
    left_motor_set_speed(direction*(TURNINGPACE));
    right_motor_set_speed(direction*(-TURNINGPACE));
    while(left_motor_get_pos() != number_of_step){
        chThdSleepMilliseconds(2);// dont block other threads while turning
    }
    while(right_motor_get_pos() != -number_of_step){
        chThdSleepMilliseconds(2);// dont block other threads while turning
    }
    left_motor_set_speed(0);
    right_motor_set_speed(0); 
}


void go_forward(int16_t vitesse) {

    left_motor_set_pos(0); // reset the step counter
    right_motor_set_pos(0);

    left_motor_set_speed(vitesse); // set the speed for both motor
    right_motor_set_speed(vitesse);
}

void stop_engines(void) {
    left_motor_set_speed(0); 
    right_motor_set_speed(0);
}
