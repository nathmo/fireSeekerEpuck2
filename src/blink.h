/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : blink.h
 * 
 * This file contains the function headers related to showing
 * a colorful pattern (fire truck red and blue) or the default color when patrolling
 */

#ifndef BLINK_H
#define BLINK_H

#include <ch.h>
#include <hal.h>
#include <leds.h> // Fichier donné par le professeur

static BSEMAPHORE_DECL(sem_LED_EXTINGUISH, true);
static BSEMAPHORE_DECL(sem_LED_ROAMING, false);

// Fonction pour démarrer le motif de clignotement de patrouille
void process_blink_start(void);

#endif
