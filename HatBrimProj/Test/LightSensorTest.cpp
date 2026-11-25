/*
 * LightSensorTest.cpp  (HOST ONLY)
 */

// ---------------------------------------------
// WHY: gtest provides TEST() and the EXPECT_* checks.
#include <gtest/gtest.h>
#include "Hardware/LightSensor.h"

// WHY: a fixture builds the same setup before every test, so each test starts clean
// and does not depend on the others.
//
// the trick: LightSensor reads through AdcScanner::getValue(), which only returns
// values[slot]. so we hand AdcScanner a plain array and write "ADC readings" into it
// ourselves. no HAL, no mock needed.
class LightSensorTest : public ::testing::Test {
protected:
	static const int LENGTH = 4;             // small window so the maths is easy to follow by hand

	volatile uint32_t adcValues[2] = { 0, 0 };
	int buffer[LENGTH] = { 0 };              // same as arrNumbersLeft in app.cpp: starts at zero
	AdcScanner adc{ nullptr, adcValues, 2 }; // nullptr handle: the HAL is never touched
	LightSensor sensor{ adc, 0, buffer, LENGTH };  // watches slot 0

	// WHY: helper so a test reads like "the ADC now says 100, read once".
	int readWith(uint32_t adcReading)
	{
		adcValues[0] = adcReading;
		return sensor.read();
	}
};

// the buffer starts filled with zeros, so one reading of 100 gives (100+0+0+0)/4.
TEST_F(LightSensorTest, FirstReadIsAveragedWithEmptyBuffer)
{
	EXPECT_EQ(25, readWith(100));
}

// once the window is full of the same value, the average must equal that value.
TEST_F(LightSensorTest, ConstantInputConvergesToThatValue)
{
	int result = 0;
	for (int i = 0; i < LENGTH; i++)
	{
		result = readWith(100);
	}
	EXPECT_EQ(100, result);
}

// after one more read the OLDEST value must be dropped (pos wraps back to 0).
// window 100,100,100,100 -> read 200 replaces the first 100 -> (200+100+100+100)/4 = 125
TEST_F(LightSensorTest, WrapAroundReplacesOldestValue)
{
	for (int i = 0; i < LENGTH; i++)
	{
		readWith(100);
	}
	EXPECT_EQ(125, readWith(200));
}
// ---------------------------------------------
