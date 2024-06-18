#include <FreeRTOS.h>
#include <task.h>

#include "init.h"

int main() {
	init();
	vTaskStartScheduler();

	while(1) {
		// we should have never gotten here
		printf("Why are we here?!\n");
	}

	return 0;
}

