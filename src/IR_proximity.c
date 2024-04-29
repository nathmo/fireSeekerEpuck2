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

#include <stdint.h>
#include "sensors/proximity.h"

static BSEMAPHORE_DECL(sem_detection_collision_side_frontR, FALSE);
static BSEMAPHORE_DECL(sem_detection_collision_side_frontL, FALSE);
static BSEMAPHORE_DECL(sem_detection_collision_side_R, FALSE);
static BSEMAPHORE_DECL(sem_detection_collision_side_L, FALSE);
static BSEMAPHORE_DECL(sem_detection_collision_NO, TRUE);

chBSemSignal(&sem_process_image_FireDetected);

static THD_WORKING_AREA(WAdetection_collision_side, 64); // allocate memory for the tread detection_collision_side
static THD_FUNCTION(detection_collision_side, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    const uint8_t NUM_SENSORS = 4;
    const uint16_t MIN_DISTANCE = 800; // Quand on est au plus proche du capteur
    const uint16_t MAX_DISTANCE_THRESHOLD = 50; // Se trouve "loin" de l'obstacle
    const uint8_t sensor_indices[NUM_SENSORS] = {1, 2, 7, 8};

    while(TRUE){
        int8_t closest_sensor_index = -1; // Initialisation avec une valeur indiquant aucune détection
        // Lire les distances des capteurs spécifiés
        for (uint8_t i = 0; i < NUM_SENSORS; i++) {
            uint8_t sensor_index = sensor_indices[i];
            uint16_t current_distance = get_proximity(sensor_index);

            // Mettre à jour le capteur le plus proche si la distance est inférieure ou égale à MIN_DISTANCE
            if (current_distance <= MIN_DISTANCE && current_distance <= MAX_DISTANCE_THRESHOLD) {
                MIN_DISTANCE = current_distance;
                closest_sensor_index = sensor_index;
            }
        }
        // Retourner l'index du capteur qui touche.
        switch(closest_sensor_index) {
            case 1:
                chBSemSignal(&sem_detection_collision_side_frontR) 
                break;
            case 2:
                chBSemSignal(&sem_detection_collision_side_R)
                break;
            case 7:
                chBSemSignal(&sem_detection_collision_side_L)
                break;
            case 8:
                chBSemSignal(&sem_detection_collision_side_frontL) 
                break;
            default:
            chBSemSignal(&sem_detection_collision_NO) // aucune collision est détectée
        }
        chThdSleepMilliseconds(10);
    }
}

void process_IR_proximity_start(void){
    chThdCreateStatic(WAroaming_blink_pattern, sizeof(WAroaming_blink_pattern), HIGHPRIO, roaming_blink_pattern, NULL);
}