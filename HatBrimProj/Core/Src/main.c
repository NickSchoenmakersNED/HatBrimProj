/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* --------------------------------------------- */
/* WHY: gives main.c access to the BH1750 driver (bh1750.h in Core/Inc). */
#include "bh1750.h"
#include <stdio.h>
#include <stdbool.h>
/* --------------------------------------------- */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t fb_period = 0;
volatile uint32_t fb_high = 0;
volatile uint32_t ctrl_pulse = 0;
/* --------------------------------------------- */
/* WHY: One struct per physical sensor so each keeps its own I2C address. */
//BH1750_t lightSensor1;   /* ADDR -> GND, address 0x23 */
//BH1750_t lightSensor2;   /* ADDR -> 3V3, address 0x5C */
//volatile float    lux1 = 0.0f;
//volatile float    lux2 = 0.0f;
//volatile uint32_t sensor1_status = 0;
//volatile uint32_t sensor2_status = 0;
// this way i should be able to call right or left instead of just 0 and 1
// for moving avarage
enum LDRIndex {
	LDR_LEFT,   // 0
	LDR_RIGHT,  // 1
};

volatile uint32_t LDRValues[2];

void read_sensors(void) {
	uint32_t left = LDRValues[LDR_LEFT];
	uint32_t right = LDRValues[LDR_RIGHT];
}

// so i can read them out in the live expressions
volatile int32_t avgLeft = 0;
volatile int32_t avgRight = 0;

const int32_t DIFF_ON  = 300;  // Minimum lead required to turn an LED ON
const int32_t DIFF_OFF = 150;

volatile bool turnLeft  = false;
volatile bool turnRight = false;

// once more for live expression
volatile int32_t difference = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM2_Init();
	MX_I2C1_Init();
	MX_ADC1_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
	// __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1680); move motor. delete later. its only here so i dont forget the syntax
	if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) != HAL_OK) {
		Error_Handler();
	}
	// move this somewhere better later
	// source: https://gist.github.com/bmccormack/d12f4bf0c96423d03f82
	int movingAvg(int *ptrArrNumbers, long *ptrSum, int pos, int len, int nextNum)
	{
		//Subtract the oldest number from the prev sum, add the new number
		*ptrSum = *ptrSum - ptrArrNumbers[pos] + nextNum;
		//Assign the nextNum to the position in the array
		ptrArrNumbers[pos] = nextNum;
		//return the average
		return *ptrSum / len;
	}
	int arrayLeft[10];
	int arrNumbersLeft[5] = { 0 };
	int posLeft = 0;
	long sumLeft = 0;
	int lenLeft = sizeof(arrNumbersLeft) / sizeof(int);
	int countLeft = sizeof(arrNumbersLeft) / sizeof(int);

	int arrayRight[10];
	int arrNumbersRight[10] = { 0 };
	int posRight = 0;
	long sumRight = 0;
	int lenRight = sizeof(arrNumbersRight) / sizeof(int);
	int countRight = sizeof(arrNumbersRight) / sizeof(int);

//  sensor1_status = BH1750_Init(&lightSensor1, &hi2c1, BH1750_ADDR_LOW);
//  sensor2_status = BH1750_Init(&lightSensor2, &hi2c1, BH1750_ADDR_HIGH);

	/* first measurement needs up to 180 ms (datasheet max) before it is valid. */
	HAL_Delay(180);
	/* USER CODE END 2 */

	/* Initialize leds */
	BSP_LED_Init(LED_GREEN);

	/* USER CODE BEGIN BSP */

	/* -- Sample board code to switch on leds ---- */
	BSP_LED_On(LED_GREEN);

	/* USER CODE END BSP */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		/* -- Sample board code to toggle leds -- */
		BSP_LED_Toggle(LED_GREEN);

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		printf("going to enter\n");

		// TOREAD:DMA (Direct Memory Access)

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

//		volatile bool turnLeft  = false;
//		volatile bool turnRight = false;
		if (turnLeft == false) {
		    if (difference >= DIFF_ON) {
		    	turnLeft = true;
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1280);
		    } else {
		    	turnLeft = false;
		    }
		}else {
		    if (difference <= DIFF_OFF) {
		    	turnLeft = false;
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 1480);
		    } else {
		    	turnLeft = true;
		    }
		}

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
//    fb_period  = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_2);
//    fb_high    = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
//    ctrl_pulse = __HAL_TIM_GET_COMPARE(&htim1, TIM_CHANNEL_1);
//    {
//      float tmp1 = 0.0f;
//      float tmp2 = 0.0f;
//      sensor1_status = BH1750_ReadLux(&lightSensor1, &tmp1);
//      sensor2_status = BH1750_ReadLux(&lightSensor2, &tmp2);
//      if (sensor1_status == HAL_OK)
//      {
//    	  	  lux1 = tmp1;
//      }
//      if (sensor2_status == HAL_OK)
//      {
//    	  	  lux2 = tmp2;
//      }
//    }

		HAL_Delay(100);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1)
			!= HAL_OK) {
		Error_Handler();
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
	RCC_OscInitStruct.PLL.PLLM = 1;
	RCC_OscInitStruct.PLL.PLLN = 16;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
