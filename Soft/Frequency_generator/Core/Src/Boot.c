#include "Boot.h"
#include "FrequencyRead.h"

extern UART_HandleTypeDef huart2;
extern RTC_HandleTypeDef hrtc;
extern I2C_HandleTypeDef hi2c1;

extern int frequency_ideal;
extern int frequency_notIdeal;
extern uint16_t lux_val;
extern uint8_t temp_C;

uint8_t data[32];

uint8_t str[100];
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;
uint8_t PayloadId = 0x44;


void Boot()
{
pwmStart();
freqInit();
rtc();


	while(1)
	{
frequencySequence();
Get_I2C_Value();
tempSensor();

data[0] = PayloadId;
data[1] = (frequency_ideal >> 16) & 0xFF;
data[2] = (frequency_ideal >> 8) & 0xFF;
data[3] = frequency_ideal & 0xFF;
data[4] = (frequency_notIdeal >> 16) & 0xFF;
data[5] = (frequency_notIdeal >> 8) & 0xFF;
data[6] = frequency_notIdeal & 0xFF;
data[7] = (lux_val >> 8) & 0xFF;
data[8] = lux_val;
data[9] = temp_C;

int width_of_diff = sprintf(str, "%X %X %X %X %X %X %X %X %X %X \r\n",\
		data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9]);
		  HAL_UART_Transmit(&huart2, str, width_of_diff, 100);



	}
}


/****************************************RTC***********************************************/


void rtc ()
{
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	/*sprintf(str, "Time %X%X%X Date %X%X%X \r\n",sTime.Hours , sTime.Minutes, sTime.Seconds,\
			sDate.Date, sDate.Month, sDate.Year);
		   HAL_UART_Transmit(&huart2, str, sizeof(str), 0xFFFF);*/
}








