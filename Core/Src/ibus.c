/*
 * ibus.c
 *
 *  Created on: Aug 16, 2023
 *      Author: spider
 */

#include "main.h"
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "debug.h"
#include <stdio.h>
#include <sys/param.h>

#define IBUS_TASK_SIZE 2*1024

static StackType_t   IBUS_StackBuffer[IBUS_TASK_SIZE];
static StaticTask_t  IBUS_TaskBuffer;

static SemaphoreHandle_t sWaitRx = NULL;
static StaticSemaphore_t sWaitRxData = {0};

static uint8_t iBusData[2][16];
static uint8_t cBuffer = 0;
static uint16_t lBuffer = 0;

extern UART_HandleTypeDef huart2;

static void HAL_UART_RxCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	cBuffer = (cBuffer+1) & 1;
	lBuffer = Size;
	HAL_UART_Receive_DMA(&huart2, iBusData[cBuffer], sizeof(iBusData[cBuffer]));
	xSemaphoreGiveFromISR(sWaitRx, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

static void iBus_task(void *arg)
{
	char buffer[75];

	HAL_UART_RegisterRxEventCallback(&huart2, HAL_UART_RxCallback);
	sWaitRx = xSemaphoreCreateBinaryStatic(&sWaitRxData);
	if (!sWaitRx) {
		vTaskSuspend(NULL);
	}
	xSemaphoreTake(sWaitRx, 0);

	HAL_UART_Receive_DMA(&huart2, iBusData[cBuffer], sizeof(iBusData[cBuffer]));

	while(1) {
		if (xSemaphoreTake(sWaitRx, portMAX_DELAY)) {
			uint16_t len = lBuffer;
			uint16_t addr = 0;
			uint8_t cBuf = (cBuffer-1)&1;
			char *p = (char *)iBusData[cBuf];
			printf_debug("RX[%u]: %"PRIu16":", cBuf, len);

			while (len>0) {
				uint16_t offset = snprintf(buffer, sizeof(buffer), "%04"PRIx16":", addr);
				uint_fast8_t i;
				char *line = p;
				for (i=0; i<16; ++i) {
					if (i<MIN(len, 16))
						offset+=snprintf(buffer+offset, sizeof(buffer)-offset, " %02"PRIx8, *line++);
					else
						offset+=snprintf(buffer+offset, sizeof(buffer)-offset, "   ");
				}
				offset+=snprintf(buffer+offset, sizeof(buffer)-offset, " | ");
				for (i=0; i<MIN(len, 16); ++i) {
					char c[3] = { *p++, 0, 0};
					if (c[0] < 32) c[0] = '.';
					if (c[0] == '%') c[1] = c[0];
					offset+=snprintf(buffer+offset, sizeof(buffer)-offset, "%s", c);
				}
				addr += i;
			}
		}
	}
}

void iBus_Init(void)
{
	xTaskCreateStatic(iBus_task, "iBUS", IBUS_TASK_SIZE, NULL, tskIDLE_PRIORITY+configMAX_PRIORITIES-1, IBUS_StackBuffer, &IBUS_TaskBuffer);
}
