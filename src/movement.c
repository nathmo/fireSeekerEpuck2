/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : movement.c
 * 
 * This file contains the function related to the robot movement behaviour
 * such as the high level patrol and rushFire and the lower level fonction like
 * to rotate of a given angle or to proceed in straight line at a given speed.
 * 
 * TODO : create a function to turn a given number of sensor (angle) (Felipe Ramirez)
 * TODO : create a function that advance fast (Felipe Ramirez)
 * TODO : create a function that adance slowly (Felipe Ramirez)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ch.h>
#include <hal.h>
#include <motors.h>

#define DEGREES_TO_STEPS(angle_degrees) ((angle_degrees * 340) / 90) // Définition de la constante pour la conversion d'angle en steps

const uint16_t sensor_angles[] = {15, 50, 90, 150, 210, 270, 310, 345}; // Tableau des angles correspondants aux capteurs (en degrés)

// Fonction pour orienter le robot vers un capteur spécifique
void orientation_sensor(uint8_t sensor_index) {

    motors_init();

    // Vérification de la validité de l'index du capteur
    if (sensor_index >= sizeof(sensor_angles) / sizeof(sensor_angles[0])) {
        return;  // Sortie de la fonction si l'index est invalide
    }

    // Conversion de l'angle de rotation en steps
    int16_t angle_steps = DEGREES_TO_STEPS(sensor_angles[sensor_index]);

    // Détermination du sens de rotation en fonction de l'index du capteur
    int8_t rotation_direction;
    if (sensor_index < 4) {
        rotation_direction = 1;  // Sens horaire pour les 4 premiers capteurs
    } else {
        rotation_direction = -1;  // Sens anti-horaire pour les 4 derniers capteurs
    }

    left_motor_set_pos(0);
    right_motor_set_pos(0);

    // Réglage de la vitesse des moteurs en fonction du sens de rotation
    int16_t speed_left = rotation_direction * 300;
    int16_t speed_right = rotation_direction * -300;
    left_motor_set_speed(speed_left);
    right_motor_set_speed(speed_right);

    // Attente que le robot atteigne l'angle de rotation désiré
    while (!(left_motor_get_pos() == angle_steps && right_motor_get_pos() == -angle_steps));

    left_motor_set_speed(0);
    right_motor_set_speed(0);
}

