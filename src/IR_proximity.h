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

// Déclaration des sémaphores pour la détection de collision par les capteurs IR
extern BSEMAPHORE_DECL(sem_detection_collision_side_frontR, FALSE);
extern BSEMAPHORE_DECL(sem_detection_collision_side_frontL, FALSE);
extern BSEMAPHORE_DECL(sem_detection_collision_side_R, FALSE);
extern BSEMAPHORE_DECL(sem_detection_collision_side_L, FALSE);
extern BSEMAPHORE_DECL(sem_detection_collision_NO, TRUE);

// Fonction pour démarrer le traitement de la proximité IR
void process_IR_proximity_start(void);

#endif /* IR_PROXIMITY_H */
