#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "WiiClassic.h"

/* controller status object */
static wiiclassic_status_t controllerStatus;

int main(int argc, char* argv[]) {
	printf("Testing WiiClassic controller now \n");

	bool initOkay = WiiClassic_Init();
	while(!initOkay) {
		printf("No WiiClassic controller detected via I2C. Rechecking in 1 second.\n");
		sleep(1);
		initOkay = WiiClassic_Init();
	}

	//Read data in a loop for 10 seconds
	bool readOkay = false;
	for(int i=0; i < 100; i++) {
		readOkay = WiiClassic_ReadStatus(&controllerStatus);
		if(readOkay)
			WiiClassic_PrintStatus(&controllerStatus);
		else
			printf("Reading failed.\n");
		usleep(100 * 1000);
	}

	return 0;
}
