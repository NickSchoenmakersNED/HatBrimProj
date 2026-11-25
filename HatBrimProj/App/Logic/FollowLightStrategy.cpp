/*
 * FollowLightStrategy.cpp
 */

#include "Logic/FollowLightStrategy.h"

FollowLightStrategy::FollowLightStrategy(LightSensing& lightSensing, int32_t startBand, int32_t stopBand)
	: lightSensing(lightSensing), startBand(startBand), stopBand(stopBand), direction(Direction::STOP)
{
}

float FollowLightStrategy::turnSpeed()
{
	int32_t difference = lightSensing.getDifference();

	// Turning -> Holding: stop once the difference falls inside the small range
	// left
	if (direction == Direction::LEFT && difference <= stopBand)
	{
		direction = Direction::STOP;
	}
	// right
	if (direction == Direction::RIGHT && difference >= -stopBand)
	{
		direction = Direction::STOP;
	}

	// Holding -> Turning: only start again above the large band
	if (direction == Direction::STOP)
	{
		if (difference >= startBand)
		{
			direction = Direction::LEFT;
		}
		else if (difference <= -startBand)
		{
			direction = Direction::RIGHT;
		}
	}

	switch (direction)
	{
		case Direction::LEFT:  return -1.0f;
		case Direction::RIGHT: return 1.0f;
		default:               return 0.0f;
	}
}
/* --------------------------------------------- */
