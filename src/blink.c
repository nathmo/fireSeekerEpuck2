/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : blink.c
 * 
 * This file contains the function related driving the led and showing
 * a colorful pattern (fire truck red and blue) or the default color when patrolling
 * 
 * 
 */
#include <ch.h>
#include <hal.h>
#include <leds.h>
#include <stdbool.h>
#include "blink.h"

// Define the struct with a single bit flag
struct Blink_flag {
    bool fire_blink_mode : 1;
};

// Variable of type struct MyStruct
struct Blink_flag blink_flag;

// Setter function
void set_fire_blink_mode(bool value) {
    blink_flag.fire_blink_mode = value;
}

// Getter function
bool get_fire_blink_mode(void) {
    return blink_flag.fire_blink_mode;
}


static THD_WORKING_AREA(WAblink_pattern, 256);
static THD_FUNCTION(blink_pattern, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    volatile int intensity = 0;
    volatile int direction = 1; // Direction de l'augmentation (1 pour augmenter, -1 pour diminuer)
    while(true){
        if(get_fire_blink_mode()){// fait tourner une led rouge autour du robot
            clear_leds();
            toggle_rgb_led(LED2, RED_LED, LEDINTENSITY); 
            chThdSleepMilliseconds(DELAYLEDTURNAROUND);          
            clear_leds();                         
            toggle_rgb_led(LED4, RED_LED, LEDINTENSITY);  
            chThdSleepMilliseconds(DELAYLEDTURNAROUND);          
            clear_leds();                         
            toggle_rgb_led(LED6, RED_LED, LEDINTENSITY);  
            chThdSleepMilliseconds(DELAYLEDTURNAROUND);          
            clear_leds();                          
            toggle_rgb_led(LED8, RED_LED, LEDINTENSITY);  
            chThdSleepMilliseconds(DELAYLEDTURNAROUND);       
            clear_leds();  
        } else { // roaming blink pattern
            while (intensity >= 0 && intensity <= 50) { // fait un "glowing" effect sur 4 led 
                clear_leds();
                // Allumer les LED bleu avec l'intensité actuelle
                toggle_rgb_led(LED2, BLUE_LED, intensity);
                toggle_rgb_led(LED4, BLUE_LED, intensity);
                toggle_rgb_led(LED6, BLUE_LED, intensity);
                toggle_rgb_led(LED8, BLUE_LED, intensity);

                // Attendre un court laps de temps pour observer le changement progressif
                chThdSleepMilliseconds(DELAYLEDGLOW);

                // Mettre à jour l'intensité pour la prochaine itération
                intensity += direction;
            }
            direction = (-1)*direction; // Changer de direction pour commencer à diminuer l'intensité
            intensity += direction;
        }
        chThdSleepMilliseconds(50);
    }
}

void process_blink_start(void){
    set_fire_blink_mode(false);
    chThdCreateStatic(WAblink_pattern, sizeof(WAblink_pattern), LOWPRIO, blink_pattern, NULL);
}