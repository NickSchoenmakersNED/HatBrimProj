/*
 * fake main.h  (HOST ONLY - never part of the STM32 build)
 */

// ---------------------------------------------
// WHY: the real main.h pulls in the whole STM32 HAL, which cannot compile on a PC.
// AdcScanner.h includes "main.h", so on the PC this fake gets found first
// (see include order in CMakeLists.txt) and provides just enough for the code to compile.
#ifndef FAKE_MAIN_H_
#define FAKE_MAIN_H_

#include <stdint.h>

// WHY: AdcScanner stores a pointer to this type. an empty struct is enough,
// because the LightSensor test never calls into the HAL.
typedef struct { int dummy; } ADC_HandleTypeDef;

typedef enum { HAL_OK = 0, HAL_ERROR = 1, HAL_BUSY = 2, HAL_TIMEOUT = 3 } HAL_StatusTypeDef;

#define ADC_SINGLE_ENDED 0

// WHY: AdcScanner.cpp must link, so every HAL function it mentions needs a body.
// they do nothing here. 'static inline' keeps them in this header, so no extra .cpp is needed.
static inline void Error_Handler(void) {}
static inline HAL_StatusTypeDef HAL_ADCEx_Calibration_Start(ADC_HandleTypeDef*, uint32_t) { return HAL_OK; }
static inline HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef*) { return HAL_OK; }
static inline HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef*, uint32_t) { return HAL_OK; }
static inline uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef*) { return 0; }
static inline HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef*) { return HAL_OK; }

#endif /* FAKE_MAIN_H_ */
// ---------------------------------------------
