/* ---------------------------------------------
 * NEW FILE: bh1750.h  ->  place in Core/Inc/
 * WHY: Small driver header so main.c only has to call Init and ReadLux,
 *      and both sensors can share the same code (one struct per sensor).
 * --------------------------------------------- */
#ifndef BH1750_H
#define BH1750_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* 7-bit I2C addresses, set by the ADDR pin on the module */
#define BH1750_ADDR_LOW    0x23   /* ADDR pin wired to GND -> sensor 1 */
#define BH1750_ADDR_HIGH   0x5C   /* ADDR pin wired to 3V3 -> sensor 2 */

/* One of these per physical sensor */
typedef struct
{
    I2C_HandleTypeDef *hi2c;   /* I2C bus the sensor is on (hi2c1) */
    uint8_t            addr;   /* 7-bit address (not shifted) */
} BH1750_t;

/* Checks the sensor answers, powers it on and starts continuous
 * high-resolution measuring. Wait at least 180 ms before the first read. */
HAL_StatusTypeDef BH1750_Init(BH1750_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr);

/* Reads the latest measurement and converts it to lux. */
HAL_StatusTypeDef BH1750_ReadLux(BH1750_t *dev, float *lux);

#endif /* BH1750_H */
/* --------------------------------------------- */
