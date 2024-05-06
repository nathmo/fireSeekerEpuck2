/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : IR_proximity.c
 * 
 * This file contains the function related to the IR proximity sensor.
 * We detect when we are about to hit an obstacle and the angle of the collision
 * 
 */

#include <ch.h>
#include <hal.h>
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdint.h>
#include "../e-puck2_main-processor/src/sensors/proximity.h"
#include "IR_proximity.h"


bool get_if_collision_front_right(void) {
    return get_if_given_sensor_collision(0);
}

bool get_if_collision_front_left(void) {
    return get_if_given_sensor_collision(7);
}

bool get_if_collision_side_right(void) {
    return get_if_given_sensor_collision(1);
}

bool get_if_collision_side_left(void) {
    return get_if_given_sensor_collision(6);
}

bool get_if_given_sensor_collision(uint8_t index) {
    uint16_t raw_distance = get_prox(index);
    if (raw_distance > COLLISION_DISTANCE) {
        return true;
    } else {
        return false;
    }
}

bool get_if_front_collision(void) {
    if (get_if_collision_front_right() || 
        get_if_collision_front_left() || 
        get_if_collision_side_right() || 
        get_if_collision_side_left() )
    {
        return true;
    } else {
        return false;
    }
}