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

uint8_t collision_detection_side(void) {
    const uint8_t NUM_SENSORS = 4;
    const uint16_t MAX_DISTANCE = 800;

    // Tableau contenant les indices des capteurs à utiliser (1, 2, 7, 8)
    const uint8_t sensor_indices[NUM_SENSORS] = {1, 2, 7, 8};

    uint16_t sensor_distances[NUM_SENSORS];
    uint8_t closest_sensor_index = 0; // Initialisation avec le premier capteur

    // Lire les distances des capteurs spécifiés
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        uint8_t sensor_index = sensor_indices[i];
        uint16_t current_distance = get_proximity(sensor_index);

        // Vérifier si la distance est plus proche que la distance maximale actuelle
        if (current_distance < MAX_DISTANCE) {
            MAX_DISTANCE = current_distance;
            closest_sensor_index = sensor_index;
        }
    }

    return closest_sensor_index;
}