#ifndef __PWM_H
#define __PWM_H

#include "main.h"
#include "driver/ledc.h"
#include "esp_err.h"

#define PWM_GPIO_NUM    5
#define PWM_FREQ    8000000

void PWM_Init(void);

#endif