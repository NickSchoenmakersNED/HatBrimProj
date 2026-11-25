/*
 * LightSensing.h
 */

#ifndef INC_LIGHTSENSING_H_
#define INC_LIGHTSENSING_H_

#include "Interfaces/ILightSensor.h"
#include "Hardware/AdcScanner.h"

#include <stdint.h>

class LightSensing {
public:
	LightSensing(AdcScanner& adc, ILightSensor& left, ILightSensor& right);

	void update();
	int32_t getDifference() const;   // positive = left is brighter

private:
	AdcScanner& adc;
	ILightSensor& left;
	ILightSensor& right;
	// for Live Expressions
	volatile int32_t avgLeft;
	volatile int32_t avgRight;
	volatile int32_t difference;
};

#endif /* INC_LIGHTSENSING_H_ */
