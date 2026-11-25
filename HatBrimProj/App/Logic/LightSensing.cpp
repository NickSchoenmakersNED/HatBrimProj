/*
 * LightSensing.cpp
 */

#include "Logic/LightSensing.h"

LightSensing::LightSensing(AdcScanner& adc, ILightSensor& left, ILightSensor& right)
	: adc(adc), left(left), right(right), avgLeft(0), avgRight(0), difference(0)
{
}

void LightSensing::update()
{
	adc.scan();
	avgLeft = left.read();
	avgRight = right.read();
	difference = avgLeft - avgRight;
}

int32_t LightSensing::getDifference() const
{
	return difference;
}
