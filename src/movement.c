/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : movement.c
 * 
 * This file contains the function related to the robot movement behaviour
 * such as the high level patrol and rushFire and the lower level fonction like
 * to rotate of a given angle or to proceed in straight line at a given speed.
 * 
 * TODO : create a function to turn a given number of sensor (angle) (Felipe Ramirez)
 * TODO : create a function that advance fast (Felipe Ramirez)
 * TODO : create a function that adance slowly (Felipe Ramirez)
 */

#include <ch.h>
#include <hal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../e-puck2_main-processor/src/motors.h"
#include "../e-puck2_main-processor/src/leds.h"
#include <math.h>
#include "movement.h"
#include "blink.h"

void turn_toward_given_sensor(uint8_t sensor_index) {
    const int16_t sensor_angles[8]={15, 50, 90, 150, -150, -90, -50, -15};
    int16_t number_of_step = (sensor_angles[sensor_index]*7)/2;
    motors_init();
    int8_t direction = 1; 
    left_motor_set_pos(0);
    right_motor_set_pos(0);
    if(number_of_step<0){
        direction=(-1);
    }
    left_motor_set_speed(direction*(300));
    right_motor_set_speed(direction*(-300));
    while(left_motor_get_pos() != number_of_step);
    while(right_motor_get_pos() != -number_of_step);
    left_motor_set_speed(0);
    right_motor_set_speed(0); 
}

void turn_specific_angle(int16_t angle_degrees) {
    int16_t number_of_step = (angle_degrees*9)/2;
    motors_init();
    int8_t direction = 1; 
    left_motor_set_pos(0);
    right_motor_set_pos(0);
    if(number_of_step<0){
        direction=(-1);
    }
    left_motor_set_speed(direction*(300));
    right_motor_set_speed(direction*(-300));
    while(left_motor_get_pos() != number_of_step);
    while(right_motor_get_pos() != -number_of_step);
    left_motor_set_speed(0);
    right_motor_set_speed(0); 
}


void avancer(int16_t vitesse) {
    motors_init();

    left_motor_set_pos(0);
    right_motor_set_pos(0);

    left_motor_set_speed(vitesse);
    right_motor_set_speed(vitesse);
}

void stop_engines(void) {
    left_motor_set_speed(0);
    right_motor_set_speed(0);
}