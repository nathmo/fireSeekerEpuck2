/*
 * Author: Nathann Morand + Felipe Ramirez
 * Description: Fire Seeker Robot
 * 
 * File : blink.c
 * 
 * This file contains the function related driving the led and showing
 * a colorful pattern (fire truck red and blue) or the default color when patrolling
 * 
 * TODO : create a function to play the lightshow when extinguishing a fire (Felipe Ramirez)
 * TODO : create a function that play the lightshow when moving around (Felipe Ramirez)
 * 
 */

#include <blink.h>
#include <leds.h> \\fichier donné par le prof

static BSEMAPHORE_DECL(LED_EXTINGUISH, TRUE)
static BSEMAPHORE_DECL(LED_ROAMING, TRUE)


static THD_WORKING_AREA(WAextinguish_blink_pattern, 64); // allocate memory for the tread extinguish_blink_pattern
static THD_FUNCTION(extinguish_blink_pattern, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    chBSemWait(&LED_EXTINGUISH);

    toggle_rgb_led(LED2, BLUE_LED, 100); 
    chThdSleepMilliseconds(100);          
    clear_leds();                         
    toggle_rgb_led(LED4, BLUE_LED, 100);  
    chThdSleepMilliseconds(100);          
    clear_leds();                         
    toggle_rgb_led(LED6, BLUE_LED, 100);  
    chThdSleepMilliseconds(100);          
    clear_leds();                          
    toggle_rgb_led(LED8, BLUE_LED, 100);  
    chThdSleepMilliseconds(200);         
    clear_leds();
}

static THD_WORKING_AREA(WAroaming_blink_pattern, 64); // allocate memory for the tread roaming_blink_pattern
static THD_FUNCTION(roaming_blink_pattern, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    chBSemWait(&LED_ROAMING);

    void pulse_red_led(void) {
    int intensity = 0;
    int direction = 1; // Direction de l'augmentation (1 pour augmenter, -1 pour diminuer)

    while (intensity >= 0 && intensity <= 100) {
        // Allumer les LED rouges avec l'intensité actuelle
        toggle_rgb_led(LED2, RED_LED, 100)
        toggle_rgb_led(LED4, RED_LED, 100)
        toggle_rgb_led(LED6, RED_LED, 100)
        toggle_rgb_led(LED8, RED_LED, 100)

        // Attendre un court laps de temps pour observer le changement progressif
        chThdSleepMilliseconds(20);

        // Mettre à jour l'intensité pour la prochaine itération
        intensity += direction;

        // Inverser la direction si l'intensité atteint les limites
        if (intensity == 100 || intensity == 0) {
            direction = -direction; // Changer de direction pour commencer à diminuer l'intensité
        }
    }

    // Éteindre les LED une fois la séquence terminée
    clear_leds();
}
