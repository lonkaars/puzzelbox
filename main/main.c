#include <FreeRTOS.h>
#include <task.h>

#include "init.h"

int main() {
	init();
	vTaskStartScheduler();
}

