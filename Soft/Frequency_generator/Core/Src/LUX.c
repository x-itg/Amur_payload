#include "LUX.h"
#include "Boot.h"
uint8_t bufLux[1];
uint8_t receive[10];
uint16_t lux_val;
uint8_t lux_address = ADDR_LUX;
uint8_t Lux = TAKE_MEASUREMENT;
uint32_t i = 0;

extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

void Get_I2C_Value(void)
{

bufLux[0] = Lux;
	   HAL_I2C_Master_Transmit(&hi2c1,lux_address,bufLux,1,HAL_MAX_DELAY);
	   HAL_Delay(200);

	   HAL_I2C_Master_Receive_IT(&hi2c1,lux_address,receive,2);
	   HAL_Delay(1);
}

void 	HAL_I2C_MasterRxCpltCallback (I2C_HandleTypeDef *hi2c){
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
	lux_val = ((uint16_t)receive[0]<<8 | receive[1]);
	/*sprintf((char*)receive,"lux = %u \r\n",lux_val);
	HAL_UART_Transmit(&huart2, receive, strlen((char*)receive), HAL_MAX_DELAY);*/

}
