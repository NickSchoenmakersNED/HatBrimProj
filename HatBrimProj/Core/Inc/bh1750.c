/* ---------------------------------------------
 * NEW FILE: bh1750.c  ->  place in Core/Src/
 * WHY: Contains the actual I2C talk with the BH1750, kept out of main.c
 *      so CubeMX regenerating main.c can never overwrite it.
 * --------------------------------------------- */
#include "bh1750.h"

/* BH1750 command bytes (from the BH1750FVI datasheet) */
#define BH1750_CMD_POWER_ON     0x01   /* wake up, wait for a measure command */
#define BH1750_CMD_RESET        0x07   /* clear the data register (only works when powered on) */
#define BH1750_CMD_CONT_HRES    0x10   /* continuous, 1 lux resolution, ~120 ms per measurement */

#define BH1750_TIMEOUT_MS       100    /* max time to wait for one I2C transfer */

/* Sends one command byte to the sensor.
 * HAL wants the address shifted left by 1 (8-bit form), so we shift here. */
static HAL_StatusTypeDef BH1750_SendCommand(BH1750_t *dev, uint8_t cmd)
{
    return HAL_I2C_Master_Transmit(dev->hi2c, (uint16_t)(dev->addr << 1),
                                   &cmd, 1, BH1750_TIMEOUT_MS);
}

HAL_StatusTypeDef BH1750_Init(BH1750_t *dev, I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    dev->hi2c = hi2c;
    dev->addr = addr;

    /* Does anything answer on this address? (3 tries) */
    if (HAL_I2C_IsDeviceReady(dev->hi2c, (uint16_t)(dev->addr << 1), 3, BH1750_TIMEOUT_MS) != HAL_OK)
    {
        return HAL_ERROR;   /* not found: check wiring and the ADDR pin */
    }

    if (BH1750_SendCommand(dev, BH1750_CMD_POWER_ON) != HAL_OK)  return HAL_ERROR;
    if (BH1750_SendCommand(dev, BH1750_CMD_RESET) != HAL_OK)     return HAL_ERROR;
    if (BH1750_SendCommand(dev, BH1750_CMD_CONT_HRES) != HAL_OK) return HAL_ERROR;

    return HAL_OK;
}

HAL_StatusTypeDef BH1750_ReadLux(BH1750_t *dev, float *lux)
{
    uint8_t buf[2];

    /* The sensor sends 2 bytes: high byte first, then low byte */
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(dev->hi2c, (uint16_t)(dev->addr << 1),
                                                      buf, 2, BH1750_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return status;
    }

    uint16_t raw = ((uint16_t)buf[0] << 8) | buf[1];

    /* Datasheet: lux = raw / 1.2 in high-resolution mode with default settings */
    *lux = (float)raw / 1.2f;

    return HAL_OK;
}
/* --------------------------------------------- */
