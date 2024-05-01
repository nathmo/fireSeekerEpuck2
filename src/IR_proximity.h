/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : IR_proximity.h
 * 
 * This file contains the function header related to the IR proximity sensor.
 * We detect when we are about to hit an obstacle and the angle of the collision
 */

#ifndef IR_PROXIMITY_H
#define IR_PROXIMITY_H

#include <ch.h>
#include <hal.h>
#include <sensors/proximity.h>

void setFrontRight(bool value);
void setFrontLeft(bool value);
void setSideRight(bool value);
void setSideLeft(bool value);
void setNoObstacleDetected(bool value);
bool getFrontRight(void);
bool getFrontLeft(void);
bool getSideRight(void);
bool getSideLeft(void);
bool getNoObstacleDetected(void);
// Fonction pour démarrer le traitement de la proximité IR
void process_IR_proximity_start(void);

#endif
