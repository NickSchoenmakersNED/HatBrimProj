/*
 * HatController.cpp
 */

#include "Logic/HatController.h"

HatController::HatController(LightSensing& lightSensing, FollowLightStrategy& followStrategy, IMotor& motor)
	: lightSensing(lightSensing), followStrategy(followStrategy), motor(motor), state(State::INIT)
{
}

void HatController::handleEvent(Event ev)
{
	switch (state)
	{
	case State::INIT:
		if (ev == Event::INIT_COMPLETE)
		{
			state = State::FOLLOW;
		}
		break;
	case State::FOLLOW:
		break;
	}
}

void HatController::update()
{
	if (state != State::FOLLOW)
	{
		return;
	}

	lightSensing.update();
	float speed = followStrategy.turnSpeed();

	if (speed == 0.0f)
	{
		motor.stop();
	}
	else
	{
		motor.drive(speed);
	}
}
/* --------------------------------------------- */
