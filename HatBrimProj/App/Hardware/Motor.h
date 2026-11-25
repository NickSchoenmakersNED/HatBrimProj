/*
 * Motor.h
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"
#include "Interfaces/IMotor.h"

#include <stdint.h>

class Motor : public IMotor {
public:
	// pulse = stopPulse + speed * pulseRange   (speed -1 -> 1260, 0 -> 1480, +1 -> 1700)
	// this calculation was made by AI. while transferring it to this method from the old, i thought that it would be better to maybe not make these magic numbers
	// i asked what the conventions are, and the AI answered that this was a common one. and its a clear 100% — 0% — -100%. so i used it.
	Motor(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t stopPulse, uint32_t pulseRange);

	void init();                    // starts the PWM. call once
	void drive(float speed) override;
	void stop() override;

private:
	TIM_HandleTypeDef* htim;
	uint32_t channel;
	uint32_t stopPulse;
	uint32_t pulseRange;
};

#endif /* INC_MOTOR_H_ */
