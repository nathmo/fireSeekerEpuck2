/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : IR_proximity.c
 * 
 * This file contains the function related to the IR proximity sensor.
 * We detect when we are about to hit an obstacle and the angle of the collision
 * 
 * TODO : write a function that check if any of the fron half circle sensors are triggered and return which one
 * (also be careful with false positive, ensure a certain level of confidence is reached between returning a value) 
 * (Felipe Ramirez)
 */

#include <ch.h>
#include <hal.h>
#include <math.h>
#include <usbcfg.h>
#include <chprintf.h>
#include <stdint.h>
#include "sensors/proximity.h"
#include "IR_proximity.h"

// Define the struct with 5 boolean flags
struct DetectionFlags {
    bool frontRight : 1;
    bool frontLeft : 1;
    bool sideRight : 1;
    bool sideLeft : 1;
    bool noObstacleDetected : 1;
};

// Global variable of type struct DetectionFlags
struct DetectionFlags detectionFlags;

// Setter functions for each flag
void setFrontRight(bool value) {
    detectionFlags.frontRight = value;
}

void setFrontLeft(bool value) {
    detectionFlags.frontLeft = value;
}

void setSideRight(bool value) {
    detectionFlags.sideRight = value;
}

void setSideLeft(bool value) {
    detectionFlags.sideLeft = value;
}

void setNoObstacleDetected(bool value) {
    detectionFlags.noObstacleDetected = value;
}

// Getter functions for each flag
bool getFrontRight(void) {
    return detectionFlags.frontRight;
}

bool getFrontLeft(void) {
    return detectionFlags.frontLeft;
}

bool getSideRight(void) {
    return detectionFlags.sideRight;
}

bool getSideLeft(void) {
    return detectionFlags.sideLeft;
}

bool getNoObstacleDetected(void) {
    return detectionFlags.noObstacleDetected;
}

static THD_WORKING_AREA(WAdetection_collision_side, 128); // allocate memory for the tread detection_collision_side
static THD_FUNCTION(detection_collision_side, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    const uint16_t MAX_DISTANCE_THRESHOLD = 50; // Se trouve "loin" de l'obstacle
    const uint8_t sensor_indices[4] = {1, 2, 7, 8};

    while(true){
        int8_t closest_sensor_index = -1; // Initialisation avec une valeur indiquant aucune détection
        // Lire les distances des capteurs spécifiés
        for (uint8_t i = 0; i < sizeof(sensor_indices) / sizeof(sensor_indices[0]); i++) {
            uint8_t sensor_index = sensor_indices[i];
            uint16_t current_distance = get_prox(sensor_index);
            uint16_t MIN_DISTANCE = 800; // Quand on est au plus proche du capteur
            // Mettre à jour le capteur le plus proche si la distance est inférieure ou égale à MIN_DISTANCE
            if (current_distance >= MIN_DISTANCE && current_distance >= MAX_DISTANCE_THRESHOLD) {
                MIN_DISTANCE = current_distance;
                closest_sensor_index = sensor_index;
            }
        }
        // Retourner l'index du capteur qui touche.
        switch(closest_sensor_index) {
            case 1:
                setFrontRight(true);
                setFrontLeft(false);
                setSideRight(false);
                setSideLeft(false);
                setNoObstacleDetected(false);
                break;
            case 2:
                setFrontRight(false);
                setFrontLeft(false);
                setSideRight(true);
                setSideLeft(false);
                setNoObstacleDetected(false);
                break;
            case 7:
                setFrontRight(false);
                setFrontLeft(false);
                setSideRight(false);
                setSideLeft(true);
                setNoObstacleDetected(false);
                break;
            case 8:
                setFrontRight(false);
                setFrontLeft(true);
                setSideRight(false);
                setSideLeft(false);
                setNoObstacleDetected(false);
                break;
            default:
                setFrontRight(false);
                setFrontLeft(false);
                setSideRight(false);
                setSideLeft(false);
                setNoObstacleDetected(true); // aucune collision est détectée
        }
        chThdSleepMilliseconds(10);
    }
}

void process_IR_proximity_start(void){
    chThdCreateStatic(WAdetection_collision_side, sizeof(WAdetection_collision_side), NORMALPRIO, detection_collision_side, NULL);
}