/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : blink.h
 * 
 * This file contains the function headers related to showing
 * a colorful pattern on the robot
 */

#ifndef BLINK_H
#define BLINK_H

#include <stdbool.h>

// Fonction pour démarrer le motif de clignotement de patrouille
void process_blink_start(void);

void set_fire_blink_mode(bool value);

// Getter function
bool get_fire_blink_mode(void);


#endif
