#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <ch.h>
#include <hal.h>
#include <chprintf.h>
#include <usbcfg.h>
#include <camera.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>

#define FASTPACE 600
#define SLOWPACE 300
void process_state_machine_start(void);

#endif