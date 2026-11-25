/*
 * FollowLightStrategy.h
 */

#ifndef INC_FOLLOWLIGHTSTRATEGY_H_
#define INC_FOLLOWLIGHTSTRATEGY_H_

#include "Logic/LightSensing.h"
#include "Common/Direction.h"

#include <stdint.h>

class FollowLightStrategy {
public:
	FollowLightStrategy(LightSensing& lightSensing, int32_t startBand, int32_t stopBand);

	// made it a float so we can use PID later
	float turnSpeed();  // -1.0 = turn left, 0 = hold, 1.0 = turn right

private:
	LightSensing& lightSensing;
	int32_t startBand;
	int32_t stopBand;
	volatile Direction direction;
};

#endif /* INC_FOLLOWLIGHTSTRATEGY_H_ */
