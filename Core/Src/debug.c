/*
 * debug.c
 *
 *  Created on: Aug 16, 2023
 *      Author: spider
 */

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <SEGGER_RTT.h>
#include <inttypes.h>

static SemaphoreHandle_t sRTTBusy = NULL;
static StaticSemaphore_t sRTTBusyData = {0};

void printf_debug( const char* format, ... )
{
    va_list arglist;
    BaseType_t inISR = xPortIsInsideInterrupt();
    TickType_t now = (!inISR)?xTaskGetTickCount():xTaskGetTickCountFromISR();

    if (!sRTTBusy) {
    	sRTTBusy = xSemaphoreCreateBinaryStatic(&sRTTBusyData);
    	xSemaphoreGive(sRTTBusy);
    }

    if ((!inISR && xSemaphoreTake(sRTTBusy, portMAX_DELAY)) || (inISR && xSemaphoreTakeFromISR(sRTTBusy, NULL))) {
		SEGGER_RTT_printf( 0, RTT_CTRL_TEXT_BRIGHT_BLACK"["RTT_CTRL_TEXT_WHITE"%"PRIu32""RTT_CTRL_TEXT_BRIGHT_BLACK"]"RTT_CTRL_RESET" ", now);
		va_start( arglist, format );
		SEGGER_RTT_vprintf( 0, format, &arglist );
		va_end( arglist );
		SEGGER_RTT_printf( 0, "\r\n"RTT_CTRL_RESET);
		if (!inISR)
			xSemaphoreGive(sRTTBusy);
		else
			xSemaphoreGiveFromISR(sRTTBusy, NULL);
    }
}
