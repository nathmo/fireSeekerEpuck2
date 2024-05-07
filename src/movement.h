/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : movement.h
 * 
 * This file contains the function related to the robot movement
 * 
 */

#ifndef MOVEMENT_H
#define MOVEMENT_H

#define DEGREES_TO_STEPS(angle_degrees) ((angle_degrees * 340) / 90) // Définition de la constante pour la conversion d'angle en steps
#define TURNINGPACE 300

#define FORWARD 1
#define BACKWARD -1

#define INDEXFRONTRIGHT 0
#define INDEXFRONTLEFT 7
#define INDEXSIDERIGHT 1
#define INDEXSIDELEFT 6

#define TURNINGPACE 300

// set the fonction qui font office de wrapper pour turn_toward_given_sensor avec un nom plus conviviale
void turn_toward_sensor_front_right(void);

void turn_toward_sensor_front_left(void);

void turn_toward_sensor_side_right(void);

void turn_toward_sensor_side_left(void);

// Fonction pour orienter le robot vers un capteur spécifique
void turn_toward_given_sensor(uint8_t sensor_index);

// Fonction pour tourner d'un angle spécifique (en degrés)
void turn_specific_angle(int16_t angle_degrees);

// Fonction pour faire avancer le robot à une vitesse donnée
void go_forward(int16_t vitesse);

// Fonction pour arrêter les moteurs du robot
void stop_engines(void);

#endif
