/*
 * LightSensor.h
 */

#ifndef INC_LIGHTSENSOR_H_
#define INC_LIGHTSENSOR_H_

#include "Interfaces/ILightSensor.h"
#include "Hardware/AdcScanner.h"

#include <stdint.h>

class LightSensor : public ILightSensor {
public:
	LightSensor(AdcScanner& adc, uint32_t slot, int* buffer, int length);

	int read() override;  // adds the newest ADC value to the average and returns the average

private:
	AdcScanner& adc;
	uint32_t slot;        // which ADC channel (LDR_LEFT / LDR_RIGHT)
	int* buffer;
	int length;
	int pos;
	long sum;
	volatile int average; // last result, for Live Expressions
};
/* --------------------------------------------- */

#endif /* INC_LIGHTSENSOR_H_ */
