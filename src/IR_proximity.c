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

// Fonction pour détecter une collision du côté
int8_t detection_collision_side(void) {
    const uint8_t NUM_SENSORS = 4;
    const uint16_t MIN_DISTANCE = 800; // Quand on est au plus proche du capteur
    const uint16_t MAX_DISTANCE_THRESHOLD = 100;

    const uint8_t sensor_indices[NUM_SENSORS] = {1, 2, 7, 8};

    uint16_t sensor_distances[NUM_SENSORS];
    int8_t closest_sensor_index = -1; // Initialisation avec une valeur indiquant aucune détection

    // Lire les distances des capteurs spécifiés
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        uint8_t sensor_index = sensor_indices[i];
        uint16_t current_distance = get_proximity(sensor_index);

        // Mettre à jour le capteur le plus proche si la distance est inférieure ou égale à MIN_DISTANCE
        if (current_distance <= MIN_DISTANCE) {
            MIN_DISTANCE = current_distance;
            closest_sensor_index = sensor_index;
        }
    }

    // Vérifier si la distance la plus proche est inférieure ou égale au seuil maximal
    if (closest_sensor_index != -1 && MIN_DISTANCE <= MAX_DISTANCE_THRESHOLD) {
        return -1; // Retourner -1 si une collision est détectée du côté
    }

    return closest_sensor_index; // Retourner l'indice du capteur le plus proche
}