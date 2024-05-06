#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ch.h>
#include <hal.h>
#include <memory_protection.h>
#include <usbcfg.h>
#include <motors.h>
#include <camera/po8030.h>
#include <camera/dcmi_camera.h>
#include <chprintf.h>
#include <spi_comm.h>
#include <stdbool.h>
#include <msgbus/messagebus.h>
#include <parameter/parameter.h>

// Declare the message bus and parameter namespace globally
extern messagebus_t bus;
extern parameter_namespace_t parameter_root;

void SendUint8ToComputer(uint8_t* data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif
