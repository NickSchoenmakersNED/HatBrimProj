#include "app.h"
#include "main.h"
#include "adc.h"
#include "tim.h"
#include <stdio.h>
extern "C" {

}

volatile uint32_t fb_period = 0;
volatile uint32_t fb_high = 0;
volatile uint32_t ctrl_pulse = 0;

enum LDRIndex {
	LDR_RIGHT,  // 0
	LDR_LEFT,   // 1
};

volatile uint32_t LDRValues[2];

volatile int32_t avgLeft = 0;
volatile int32_t avgRight = 0;

const int32_t DIFF_ON  = 300;
const int32_t DIFF_OFF = 150;

volatile bool turnLeft  = false;
volatile bool turnRight = false;

volatile int32_t difference = 0;

// source: https://gist.github.com/bmccormack/d12f4bf0c96423d03f82
static int movingAvg(int *ptrArrNumbers, long *ptrSum, int pos, int len, int nextNum)
{
	*ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
	ptrArrNumbers[pos] = nextNum;
	return *ptrSum / len;
}

static int arrNumbersLeft[5] = { 0 };
static int posLeft = 0;
static long sumLeft = 0;
static int lenLeft = sizeof(arrNumbersLeft) / sizeof(int);
static int arrNumbersRight[10] = { 0 };
static int posRight = 0;
static long sumRight = 0;
static int lenRight = sizeof(arrNumbersRight) / sizeof(int);

void app_init(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
		Error_Handler();
	}

	/* first measurement needs up to 180 ms (datasheet max) before it is valid. */
	HAL_Delay(180);
}

void app_loop(void)
{
	printf("going to enter\n");

	HAL_ADC_Start(&hadc1);

	for (int i = 0; i <= 1; i++)
	{
		if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
			LDRValues[i] = HAL_ADC_GetValue(&hadc1);
		}
	}

	HAL_ADC_Stop(&hadc1);

	avgLeft = movingAvg(arrNumbersLeft, &sumLeft, posLeft, lenLeft, (int)LDRValues[LDR_LEFT]);
	posLeft++;
	if (posLeft >= lenLeft) {
		posLeft = 0;
	}

	avgRight = movingAvg(arrNumbersRight, &sumRight, posRight, lenRight, (int)LDRValues[LDR_RIGHT]);
	posRight++;
	if (posRight >= lenRight) {
		posRight = 0;
	}

	difference = avgLeft - avgRight;

	// check if the motor should turn left.
	if (turnLeft == false) {
		if (difference >= DIFF_ON) {
			turnLeft = true;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1280);
		} else {
			turnLeft = false;
		}
	} else {
		if (difference <= DIFF_OFF) {
			turnLeft = false;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1480);
		} else {
			turnLeft = true;
		}
	}

	// check if the motor should turn right
	if (turnRight == false) {
		if (difference <= -DIFF_ON) {
			turnRight = true;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1720);
		} else {
			turnRight = false;
		}
	} else {
		if (difference >= -DIFF_OFF) {
			turnRight = false;
			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1480);
		} else {
			turnRight = true;
		}
	}

	HAL_Delay(100);
}

//https://claude.ai/share/4dc9b8e9-1304-4f1a-9f0f-5956f63ff31f
