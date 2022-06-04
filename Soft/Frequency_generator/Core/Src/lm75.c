#include "lm75.h"

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

uint8_t tempSens = ADDR;
uint8_t tempReg = TEMP_REG;
uint16_t val;
uint8_t temp_C;
uint8_t bufTemp[12];


void tempSensor()
{
	bufTemp[0] = TEMP_REG;
	 HAL_I2C_Master_Transmit(&hi2c1, tempSens, bufTemp, 1, HAL_MAX_DELAY);
		HAL_I2C_Master_Receive(&hi2c1, tempSens, bufTemp, 2, HAL_MAX_DELAY);
		val = ((int16_t)bufTemp[0] << 3) | (bufTemp[1] >> 5);
			temp_C = val * 0.125;
			/*sprintf((char*)bufTemp, "%d C \r\n",temp_C);

	HAL_UART_Transmit(&huart2, bufTemp, strlen((char*)bufTemp), HAL_MAX_DELAY);
	*/
		  HAL_Delay(300);
}
