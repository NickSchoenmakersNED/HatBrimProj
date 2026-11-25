/*
 * AdcScanner.h
 */

#ifndef INC_ADCSCANNER_H_
#define INC_ADCSCANNER_H_

#include "main.h"
#include <stdint.h>

class AdcScanner {
public:
	AdcScanner(ADC_HandleTypeDef* hadc, volatile uint32_t* values, uint32_t numSensors);

	void init();                              // calibrates the ADC
	void scan();                              // converts all sensor readings into into values[]
	uint32_t getValue(uint32_t slot) const;   // result of the last read for one sensor

private:
	ADC_HandleTypeDef* hadc;
	volatile uint32_t* values;
	uint32_t numSensors;
};

#endif /* INC_ADCSCANNER_H_ */
