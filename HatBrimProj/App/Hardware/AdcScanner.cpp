/*
 * AdcScanner.cpp
 */

#include "Hardware/AdcScanner.h"

AdcScanner::AdcScanner(ADC_HandleTypeDef* hadc, volatile uint32_t* values, uint32_t numSensors)
	: hadc(hadc), values(values), numSensors(numSensors)
{
	// no HAL calls here: global objects are constructed before main()
}

void AdcScanner::init()
{
	if (HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED) != HAL_OK)
	{
		Error_Handler();
	}
}

void AdcScanner::scan()
{
	HAL_ADC_Start(hadc);

	for (uint32_t i = 0; i < numSensors; i++)
	{
		if (HAL_ADC_PollForConversion(hadc, 100) == HAL_OK)
		{
			values[i] = HAL_ADC_GetValue(hadc);
		}
	}

	HAL_ADC_Stop(hadc);
}

uint32_t AdcScanner::getValue(uint32_t slot) const
{
	return values[slot];
}
