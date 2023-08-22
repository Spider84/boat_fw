/*
 * compas.c
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

#define COMPAS_TASK_SIZE 2*1024

static StackType_t   COMPAS_StackBuffer[COMPAS_TASK_SIZE];
static StaticTask_t  COMPAS_TaskBuffer;

extern I2C_HandleTypeDef hi2c1;

#define CORDIC_MAXITER 16
#define CORDIC_PI 0x10000000

static const uint32_t CORDIC_ZTBL[] = { 0x04000000, 0x025C80A4, 0x013F670B, 0x00A2223B,
		0x005161A8, 0x0028BAFC, 0x00145EC4, 0x000A2F8B, 0x000517CA, 0x00028BE6,
		0x000145F3, 0x0000A2FA, 0x0000517D, 0x000028BE, 0x0000145F, 0x00000A30 };

int32_t fxpt_atan2(int32_t y, int32_t x) {
	int32_t tx, z = 0, fl = 0;
	if (x < 0) {
		fl = ((y > 0) ? +1 : -1);
		x = -x;
		y = -y;
	}
	for (uint_fast8_t k = 0; k < CORDIC_MAXITER; ++k) {
		tx = x;
		if (y <= 0) {
			x -= (y >> k);
			y += (tx >> k);
			z -= CORDIC_ZTBL[k];
		} else {
			x += (y >> k);
			y -= (tx >> k);
			z += CORDIC_ZTBL[k];
		}
	}

	if (fl != 0) {
		z += fl * CORDIC_PI;
	}
	return z;
}

static void Compas_task(void *arg)
{
	while(1)
	{
		fxpt_atan2(1, 1);
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void Compas_Init(void)
{
	xTaskCreateStatic(Compas_task, "Compas", COMPAS_TASK_SIZE, NULL, tskIDLE_PRIORITY+configMAX_PRIORITIES-2, COMPAS_StackBuffer, &COMPAS_TaskBuffer);
}
