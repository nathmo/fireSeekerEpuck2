/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : camera.c
 * 
 * This file contains the function related to the camera and the image processing associated.
 * there is one thread for image capture and another to detect if "Fire" (red) is present
 * in the image.
 * 
 * TODO : write a thread to capture the image  (Nathann Morand)
 * TODO : write a thread to process the image and find if we have a fire in front or something else
 * (depending on time, just look for RED or use fancy template matching algorithm or even a perceptron ) (Nathann Morand)
 */