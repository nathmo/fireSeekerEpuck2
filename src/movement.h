/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : movement.h
 * 
 * This file contains the function header related to the robot movement behaviour
 * such as the high level patrol and rushFire and the lower level fonction like
 * to rotate of a given angle or to proceed in straight line at a given speed.
 */

#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdint.h>

#define DEGREES_TO_STEPS(angle_degrees) ((angle_degrees * 340) / 90) // Définition de la constante pour la conversion d'angle en steps

// Fonction pour orienter le robot vers un capteur spécifique
void turn_toward_given_sensor(uint8_t sensor_index);

// Fonction pour tourner d'un angle spécifique (en degrés)
void turn_specific_angle(int16_t angle_degrees);

// Fonction pour faire avancer le robot à une vitesse donnée
void avancer(int16_t vitesse);

// Fonction pour arrêter les moteurs du robot
void stop_engines(void);

#endif
