/*
 * WiiClassic.c
 *
 *  Created on: Oct 14, 2017
 *      Author: max
 */
#include "WiiClassic.h"
#include <onion-i2c.h>
#include "Debug.h"

static bool WiiClassic_ReadData();
static bool WiiClassic_GetButton(uint8_t col, uint8_t D);
static uint8_t WiiClassic_GetStick(uint8_t col, uint8_t mask);
static int WiiClassic_ResetReadPointer();
static bool WiiClassic_IsControllerPresent();

/* i2c read buffer */
static uint8_t wiiData[6];
/* analog stick factors */
static uint8_t RX = 8, RY = 8, LX = 8, LY = 8;

/* init sequence */
#define WIICLASSIC_INIT_BYTES 0x40, 0x00
/* stick value byte to float divider */
#define WIICLASSIC_DIVIDER 256.0f
/* outer deadzone. controller will not go higher than this. */
#define WIICLASSIC_OUTER_DEADZONE 0.968750

/* macro for checking deadzones */
#define WIICONTROLLER_CHECK_DEADZONE(val) \
	if((val) >= WIICLASSIC_OUTER_DEADZONE) \
	(val) = 1.0f;

/* bitmasks for buttons and sticks */
/* BYTE 5*/
#define UD 0x01
#define LD 0x02
#define ZR 0x04
#define BX  0x08
#define BA  0x10
#define BY  0x20
#define BB  0x40
#define ZL 0x80

/* BYTE 4*/
#define RB 0x02
#define START 0x04
#define HOME 0x08
#define SELECT 0x10
#define LB 0x20
#define DD 0x40
#define RD 0x80

/* Initializes the WiiClassic controller */
bool WiiClassic_Init() {

	//initialization routine
	uint8_t initBytes[] = { WIICLASSIC_INIT_BYTES };
	i2c_writeBufferRaw(ONION_I2C_DEV_NUM, WIICLASSIC_I2C_ADDRESS, initBytes, (int)sizeof(initBytes));

	//Do one read to check if it is present
	WiiClassic_ReadData();
	return WiiClassic_IsControllerPresent();
}

static bool WiiClassic_IsControllerPresent() {
	//controller is assumed to be not present when read data is all 0xff
	uint8_t dataIfNotPresent[6];
	memset(dataIfNotPresent, 0xff, sizeof(dataIfNotPresent));
	return memcmp(wiiData, dataIfNotPresent, sizeof(dataIfNotPresent)) != 0;
}

/* reads the status of the WiiClass controller into the structure */
bool WiiClassic_ReadStatus(wiiclassic_status_t* status) {
	//sanity check
	if(!status) return false;

	//read status bytes from i2c
	bool readOkay = WiiClassic_ReadData();
	if(!readOkay)
		return false;

	//above I2C error handling doesn't seem to work because
	//the Omega never reports an error, even if no controller
	//is connected. let's check ourselves.
	if(!WiiClassic_IsControllerPresent())
		return false;

	//parse them into the structure
	status->shoulderL = WiiClassic_GetButton(4, LB);
	status->shoulderR = WiiClassic_GetButton(4, RB);
	status->shoulderZL = WiiClassic_GetButton(5, ZL);
	status->shoulderZR = WiiClassic_GetButton(5, ZR);
	status->dpadLeft = WiiClassic_GetButton(5, LD);
	status->dpadRight = WiiClassic_GetButton(4, RD);
	status->dpadUp = WiiClassic_GetButton(5, UD);
	status->dpadDown = WiiClassic_GetButton(4, DD);

	status->buttonSelect = WiiClassic_GetButton(4, SELECT);
	status->buttonHome = WiiClassic_GetButton(4, HOME);
	status->buttonStart = WiiClassic_GetButton(4, START);

	status->buttonX = WiiClassic_GetButton(5, BX);
	status->buttonY = WiiClassic_GetButton(5, BY);
	status->buttonA = WiiClassic_GetButton(5, BA);
	status->buttonB = WiiClassic_GetButton(5, BB);

	uint8_t al_x_raw = (uint8_t)((WiiClassic_GetStick(0, 0x3F) >> 1) * LX);
	status->analogLeftX = al_x_raw / WIICLASSIC_DIVIDER;
	WIICONTROLLER_CHECK_DEADZONE(status->analogLeftX);

	uint8_t al_y_raw = (uint8_t)((WiiClassic_GetStick(1, 0x3F) >> 1) * LY);
	status->analogLeftY = al_y_raw / WIICLASSIC_DIVIDER;
	WIICONTROLLER_CHECK_DEADZONE(status->analogLeftY);

	uint8_t ar_x_raw = (uint8_t)(((WiiClassic_GetStick(0, 0xC0) >> 3) | (WiiClassic_GetStick(1,0xC0) >> 5) | (WiiClassic_GetStick(2, 0xC0) >> 7)) * RX);
	//printf("ar_x_raw: %d\n", (int)ar_x_raw);
	status->analogRightX = ar_x_raw / WIICLASSIC_DIVIDER;
	WIICONTROLLER_CHECK_DEADZONE(status->analogRightX);

	uint8_t ar_y_raw = (uint8_t)(WiiClassic_GetStick(2, 0x3F) * RY);
	//printf("ar_y_raw: %d\n", (int)ar_y_raw);
	status->analogRightY = ar_y_raw / WIICLASSIC_DIVIDER;
	WIICONTROLLER_CHECK_DEADZONE(status->analogRightY);

	return true;
}

void WiiClassic_PrintStatus(wiiclassic_status_t* status) {
	if(!status) {
		printf("No controller status object given.\n");
		return;
	}
	uint8_t buttons[] = {
			status->buttonA, status->buttonB, status->buttonY,
			status->buttonX, status->buttonSelect, status->buttonStart,
			status->buttonHome, status->shoulderL, status->shoulderZL,
			status->shoulderZR, status->shoulderR, status->dpadUp,
			status->dpadRight, status->dpadDown, status->dpadLeft,
	};
	const char* buttonNames[] = {
			//Front buttons
			"A","B","Y","X","Select","Start","Home",
			//shoulder buttons
			"L","ZL","ZR","R",
			//dpad
			"Up","Right","Down","Left"
	};

	printf("Buttons: ");
	for(int i=0; i < 7; i++)
		if(buttons[i])
			printf("%s ", buttonNames[i]);
	printf("\nShoulder: ");
	for(int i=7; i < 11; i++)
		if(buttons[i])
			printf("%s ", buttonNames[i]);
	printf("\nDPAD: ");
	for(int i=11; i < 15; i++)
		if(buttons[i])
			printf("%s ", buttonNames[i]);
	printf("\nAnalog stick left/right: (%.6f, %.6f), (%.6f, %.6f)\n",
			status->analogLeftX, status->analogLeftY,
			status->analogRightX, status->analogRightY);
}

static int WiiClassic_ResetReadPointer() {
	uint8_t readAddr = 0x00;
	int err = i2c_writeBufferRaw(ONION_I2C_DEV_NUM, WIICLASSIC_I2C_ADDRESS, &readAddr, 1);
	return err;
}

static bool WiiClassic_ReadData() {
	//clear old data
	memset(wiiData, 0xff, sizeof(wiiData));
	//read 6 bytes from the I2C bus
	int err = i2c_readRaw(ONION_I2C_DEV_NUM, WIICLASSIC_I2C_ADDRESS, wiiData, (int)sizeof(wiiData));
	//Error handling
	if(err == EXIT_FAILURE)
		return false;
	//set read pointer to 0 again
	err = WiiClassic_ResetReadPointer();
	//error handling
	if(err == EXIT_FAILURE)
		return false;

	//debug
#if DEBUG_WIICLASSIC_DATA == 1
	hexDump("wii controller data", wiiData, (int)sizeof(wiiData));
#endif
	return true;
}

static bool WiiClassic_GetButton(uint8_t col, uint8_t D) {
	return (~wiiData[col] & D);
}

static uint8_t WiiClassic_GetStick(uint8_t col, uint8_t mask) {
	return (wiiData[col] & mask);
}
