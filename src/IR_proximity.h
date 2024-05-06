/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : IR_proximity.h
 * 
 * This file contains the function header related to the IR proximity sensor.
 * it's mostly a wrapper around the proximity.h library provided.
 */

#ifndef IR_PROXIMITY_H
#define IR_PROXIMITY_H

#include <stdbool.h>

#define COLLISION_DISTANCE 500

bool get_if_collision_front_right(void);
bool get_if_collision_front_left(void);
bool get_if_collision_side_right(void);
bool get_if_collision_side_left(void);
bool get_if_given_sensor_collision(uint8_t index);
bool get_if_front_collision(void);

#endif
