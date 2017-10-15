/*
 * WiiClassic.h
 *
 *  Created on: Oct 14, 2017
 *      Author: Maximilian Gerhardt
 *
 * This library is based on the WiiClassController
 * library [1]. It is in turn based on information
 * from [2] for the I2C specifics.
 *
 * [1] Andrew Mascolo, https://github.com/AndrewMascolo/WiiClassicController
 * [2] frank26080115, http://www.instructables.com/id/USB-Wii-Classic-Controller/
 */

#ifndef WIICLASSIC_H_
#define WIICLASSIC_H_

#include <stdint.h>
#include <stdbool.h>

/* a structure to hold all the state info of the controller */
typedef struct {
	/* Buttons */
	uint8_t buttonA;
	uint8_t buttonB;
	uint8_t buttonY;
	uint8_t buttonX;
	uint8_t buttonSelect;
	uint8_t buttonStart;
	uint8_t buttonHome;
	/* shoulder buttons */
	uint8_t shoulderL;
	uint8_t shoulderZL;
	uint8_t shoulderZR;
	uint8_t shoulderR;
	/* digital pad */
	uint8_t dpadUp;
	uint8_t dpadRight;
	uint8_t dpadDown;
	uint8_t dpadLeft;
	/* analog sticks. values are from 0.0 to 1.0 */
	float analogLeftX;
	float analogLeftY;
	float analogRightX;
	float analogRightY;
} wiiclassic_status_t;

/* defines for I2C specifics */
#define WIICLASSIC_I2C_ADDRESS 0x52
/* use /dev/i2c-0 */
#define ONION_I2C_DEV_NUM 0
/* if set to 1, outputs a hexdump of the received data */
#define DEBUG_WIICLASSIC_DATA 0

/* Initializes the WiiClassic controller */
bool WiiClassic_Init();
/* reads the status of the WiiClass controller into the structure */
bool WiiClassic_ReadStatus(wiiclassic_status_t* status);
/* prints the status to stdout */
void WiiClassic_PrintStatus(wiiclassic_status_t* status);

#endif /* WIICLASSIC_H_ */
