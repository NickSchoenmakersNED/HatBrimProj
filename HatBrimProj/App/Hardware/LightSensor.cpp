/*
 * LightSensor.cpp
 */

#include "Hardware/LightSensor.h"

// source: https://gist.github.com/bmccormack/d12f4bf0c96423d03f82
// I hate variable names like this. But this is the way you are supposed to do it.
// i dont exacly know how the sum works. but i got it from the code from the github and it works well.
static int movingAvg(int *ptrArrNumbers, long *ptrSum, int pos, int len, int nextNum)
{
	*ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
	ptrArrNumbers[pos] = nextNum;
	return *ptrSum / len;
}

LightSensor::LightSensor(AdcScanner& adc, uint32_t slot, int* buffer, int length)
	: adc(adc), slot(slot), buffer(buffer), length(length), pos(0), sum(0), average(0)
{
}

int LightSensor::read()
{
	average = movingAvg(buffer, &sum, pos, length, (int)adc.getValue(slot));
	pos++;
	if (pos >= length)
	{
		pos = 0;
	}
	return average;
}
