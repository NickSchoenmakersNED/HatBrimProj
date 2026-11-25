/*
 * Motor.cpp
 */

#include "Hardware/Motor.h"

Motor::Motor(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t stopPulse, uint32_t pulseRange)
	: htim(htim), channel(channel), stopPulse(stopPulse), pulseRange(pulseRange)
{
	// no HAL calls here: global objects are constructed before main() has set up the hardware
}

void Motor::init()
{
	HAL_TIM_PWM_Start(htim, channel);
}

void Motor::drive(float speed)
{
	// max speeds are -1 and 1. with 0 being stop
	// here we ensure the program doesnt go above or (sort of)below it.
	if (speed > 1.0f)
	{
		speed = 1.0f;
	} else if (speed < -1.0f)
	{
		speed = -1.0f;
	}

	// pulse = stopPulse + speed * pulseRange   (speed -1 -> 1260, 0 -> 1480, +1 -> 1700)
	int32_t turn = (int32_t)stopPulse + (int32_t)(speed * (float)pulseRange);
	__HAL_TIM_SET_COMPARE(htim, channel, (uint32_t)turn);
}

void Motor::stop()
{
	__HAL_TIM_SET_COMPARE(htim, channel, stopPulse);
}
