/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : camera.h
 * 
 * This file contains the function header related to the camera and the image processing associated.
 * there is one thread for image capture and another to detect if "Fire" (red) is present
 * in the image.
 */

#ifndef CAMERA_FIRE_H
#define CAMERA_FIRE_H

#include <ch.h>
#include <hal.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>
#include <stdbool.h>

// Définition de la constante pour le facteur de comparaison avec la moyenne de vert et bleu
#define RED_FACTOR 6 // Factor to compare with average of green and blue
#define IMAGE_BUFFER_SIZE 40 // number of pixel we store

// Fonction pour démarrer le traitement de la caméra
void process_camera_start(void);

// Setter function for setting fire detection flag
void setIsFireDetected(bool value);

// Getter function for getting fire detection flag
bool getIsFireDetected(void);

#endif
