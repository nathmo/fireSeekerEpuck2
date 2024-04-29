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
 * TODO : check image size and optimise memory  (Nathann Morand)
 */

#define RED_FACTOR 1.5 // Factor to compare with average of green and blue

static BSEMAPHORE_DECL(sem_capture_image, TRUE);
static BSEMAPHORE_DECL(sem_capture_image_ready, TRUE);
static BSEMAPHORE_DECL(sem_process_image_ready, TRUE);
static BSEMAPHORE_DECL(sem_process_image_FireDetected, FALSE);

static THD_WORKING_AREA(WAcapture_image, 512);
static THD_FUNCTION(capture_image, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;
    while(TRUE){
        //waits that we are ready to capture an image
        sem_capture_image();

        // the following code snippet come from TP 4
        //Takes pixels 0 to IMAGE_BUFFER_SIZE of the lines USED_LINE and USED_LINE + 1 (minimum 2 lines because reasons)
        po8030_advanced_config(FORMAT_RGB565, 0, USED_LINE, IMAGE_BUFFER_SIZE, 2, SUBSAMPLING_X1, SUBSAMPLING_X1);
        dcmi_enable_double_buffering();
        dcmi_set_capture_mode(CAPTURE_ONE_SHOT);
        dcmi_prepare();

        while(1){
            //starts a capture
            dcmi_capture_start();
            //waits for the capture to be done
            wait_image_ready();
            //signals an image has been captured
            chBSemSignal(&sem_capture_image_ready);
        }
    }
}

static THD_WORKING_AREA(WAprocess_image, 4096);
static THD_FUNCTION(process_image, arg) {
    chRegSetThreadName(__FUNCTION__);
    (void)arg;

    // declare the space for storing the image
    uint8_t *img_buff_ptr;
    uint8_t image_red[IMAGE_BUFFER_SIZE] = {0};
    uint8_t image_green[IMAGE_BUFFER_SIZE] = {0};
    uint8_t image_blue[IMAGE_BUFFER_SIZE] = {0};
    uint32_t total_red = 0;
    uint32_t total_green = 0;
    uint32_t total_blue = 0;

    while(TRUE){
        //waits for the image to be ready
        sem_capture_image_ready();

        //copy the red frame to the red buffer :
        for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
            //extracts 5 MSbits of the MSbyte (First byte in big-endian format)
            //takes nothing from the second byte
            image_red[i/2] = (uint8_t)img_buff_ptr[i] & 0xF8;
        }
        //copy the green frame to the red buffer :
        for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
			//extracts 3 LSbits of the first byte and the 3 MSbits of second byte
			image_green[i/2] = (((uint8_t)img_buff_ptr[i] & 0x07) << 5 ) + (((uint8_t)img_buff_ptr[i+1] & 0xE0) >> 3);
		}
        //copy the blue frame to the red buffer :
        for(uint16_t i = 0 ; i < (2 * IMAGE_BUFFER_SIZE) ; i+=2){
            //extracts 5 LSbits of the LSByte (Second byte in big-endian format)
            //and rescale to 8 bits
            //takes nothing from the first byte
            total_blue[i/2] = ((uint8_t)img_buff_ptr[i+1] & 0x1F) << 3;
        }

        // Calculate the total amount of red, green, and blue
        total_red = 0;
        total_green = 0;
        total_blue = 0;
        for (uint16_t i = 0; i < IMAGE_BUFFER_SIZE; i++) {
            total_red += image_red[i];
            total_green += image_green[i];
            total_blue += image_blue[i];
        }

        // Calculate the average of green and blue
        uint32_t average_green_blue = (total_green + total_blue) / 2;

        // Check if the red frame value is at least RED_FACTOR times bigger than the average of green and blue
        if ((double)total_red >= RED_FACTOR * (double)average_green_blue) {
            // Signal that fire is detected
            chBSemSignal(&sem_process_image_FireDetected);
        }
        chBSemSignal(&sem_process_image_ready);
    }
}

void process_camera_start(void){
    chThdCreateStatic(WAcapture_image, sizeof(WAcapture_image), NORMALPRIO, capture_image, NULL);
    chThdCreateStatic(WAprocess_image, sizeof(WAprocess_image), NORMALPRIO, process_image, NULL);
}