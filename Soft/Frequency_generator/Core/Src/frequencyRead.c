#include "Boot.h"
#include "FrequencyRead.h"

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

uint8_t str1[100];
uint8_t str2[100];
uint8_t str_final[100];

int riseCaptured = 0;
int fallCaptured = 0;

uint32_t riseData[numval];
uint32_t fallData[numval];

uint32_t frequency_ideal = 0;
uint32_t frequency_notIdeal = 0;
uint32_t freq_differ;

float width = 0;

int isMeasured = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
	// If the Interrupt is triggered by 1st Channel
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		riseCaptured = 1;
	}

	// If the Interrupt is triggered by 2nd Channel
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
		fallCaptured = 1;
	}


	/* Rest of the calculations will be done,
	 * once both the DMAs have finished capturing enough data */
	if ((riseCaptured) && (fallCaptured))
	{

		// calculate the reference clock
		float refClock = TIMCLOCK/(PSCALAR+1);

		int indxr = 0;
		int indxf = 0;

		int countr = 0;
		int countrf = 0;

		float riseavg = 0;
		float rfavg = 0;

		/* In case of high Frequencies, the DMA sometimes captures 0's in the beginning.
		 * increment the index until some useful data shows up
		 */
		while (riseData[indxr] == 0) indxr++;

		/* Again at very high frequencies, sometimes the values don't change
		 * So we will wait for the update among the values
		 */
		while ((MIN((riseData[indxr+1]-riseData[indxr]), (riseData[indxr+2]-riseData[indxr+1]))) == 0) indxr++;

		/* riseavg is the difference in the 2 consecutive rise Time */

		/* Assign a start value to riseavg */
		riseavg += MIN((riseData[indxr+1]-riseData[indxr]), (riseData[indxr+2]-riseData[indxr+1]));
		indxr++;
		countr++;

		/* start adding the values to the riseavg */
		while (indxr < (numval))
		{
			riseavg += MIN((riseData[indxr+1]-riseData[indxr]), riseavg/countr);
			countr++;
			indxr++;
		}

		/* Find the average riseavg, the average time between 2 RISE */
		riseavg = riseavg/countr;

		indxr = 0;

		/* The calculation for the Falling pulse on second channel */

		/* If the fall time is lower than rise time,
		 * Then there must be some error and we will increment
		 * both, until the error is gone
		 */
		if (fallData[indxf] < riseData[indxr])
		{
			indxf+=2; indxr+=2;
			while (fallData[indxf] < riseData[indxr]) indxf++;
		}

		else if (fallData[indxf] > riseData[indxr])
		{
			indxf+=2; indxr+=2;
			while (fallData[indxf] > riseData[indxr+1]) indxr++;
		}


		/* The method used for the calculation below is as follows:
		 * If Fall time < Rise Time, increment Fall counter
		 * If Fall time - Rise Time is in between 0 and (difference between 2 Rise times), then its a success
		 * If fall time > Rise time, but is also > (difference between 2 Rise times), then increment Rise Counter
		 */
		while ((indxf < (numval)) && (indxr < (numval)))
		{
			/* If the Fall time is lower than rise time, increment the fall indx */
			while ((int16_t)(fallData[indxf]-riseData[indxr]) < 0)
			{
				indxf++;
			}

			/* If the Difference in fall time and rise time is >0 and less than rise average,
			 * Then we will register it as a success and increment the countrf (the number of successes)
			 */
			if (((int16_t)(fallData[indxf]-riseData[indxr]) >= 0) && (((int16_t)(fallData[indxf]-riseData[indxr]) <= riseavg)))
			{
				rfavg += MIN((fallData[indxf]-riseData[indxr]), (fallData[indxf+1]-riseData[indxr+1]));
				indxf++;
				indxr++;
				countrf++;
			}

			else
			{
				indxr++;
			}
		}

		/* Calculate the Average time between 2 Rise */
		rfavg = rfavg/countrf;

		/* Calculate Frequency
		 * Freq = Clock/(time taken between 2 Rise)
		 */
		frequency_ideal = (refClock/(float)riseavg);

		//int width1 = sprintf(str1, " TIM2    Ideal freq = %lu \r\n", frequency_ideal);
		//	HAL_UART_Transmit(&huart2, str1, width1, 100);
		/* Width of the pulse
		 *  = (Time between Rise and fall) / clock
		 */
		//width = ((rfavg)/((float)(refClock/1000000)))*1000;   // width in ns

		riseCaptured = 0;
		fallCaptured = 0;

		isMeasured = 1;

	}
	}

	else if(htim->Instance == TIM5)
		{
		// If the Interrupt is triggered by 1st Channel
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
		{
			riseCaptured = 1;
		}

		// If the Interrupt is triggered by 2nd Channel
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
		{
			fallCaptured = 1;
		}


		/* Rest of the calculations will be done,
		 * once both the DMAs have finished capturing enough data */
		if ((riseCaptured) && (fallCaptured))
		{

			// calculate the reference clock
			float refClock = TIMCLOCK/(PSCALAR+1);

			int indxr = 0;
			int indxf = 0;

			int countr = 0;
			int countrf = 0;

			float riseavg = 0;
			float rfavg = 0;

			/* In case of high Frequencies, the DMA sometimes captures 0's in the beginning.
			 * increment the index until some useful data shows up
			 */
			while (riseData[indxr] == 0) indxr++;

			/* Again at very high frequencies, sometimes the values don't change
			 * So we will wait for the update among the values
			 */
			while ((MIN((riseData[indxr+1]-riseData[indxr]), (riseData[indxr+2]-riseData[indxr+1]))) == 0) indxr++;

			/* riseavg is the difference in the 2 consecutive rise Time */

			/* Assign a start value to riseavg */
			riseavg += MIN((riseData[indxr+1]-riseData[indxr]), (riseData[indxr+2]-riseData[indxr+1]));
			indxr++;
			countr++;

			/* start adding the values to the riseavg */
			while (indxr < (numval))
			{
				riseavg += MIN((riseData[indxr+1]-riseData[indxr]), riseavg/countr);
				countr++;
				indxr++;
			}

			/* Find the average riseavg, the average time between 2 RISE */
			riseavg = riseavg/countr;

			indxr = 0;

			/* The calculation for the Falling pulse on second channel */

			/* If the fall time is lower than rise time,
			 * Then there must be some error and we will increment
			 * both, until the error is gone
			 */
			if (fallData[indxf] < riseData[indxr])
			{
				indxf+=2; indxr+=2;
				while (fallData[indxf] < riseData[indxr]) indxf++;
			}

			else if (fallData[indxf] > riseData[indxr])
			{
				indxf+=2; indxr+=2;
				while (fallData[indxf] > riseData[indxr+1]) indxr++;
			}


			/* The method used for the calculation below is as follows:
			 * If Fall time < Rise Time, increment Fall counter
			 * If Fall time - Rise Time is in between 0 and (difference between 2 Rise times), then its a success
			 * If fall time > Rise time, but is also > (difference between 2 Rise times), then increment Rise Counter
			 */
			while ((indxf < (numval)) && (indxr < (numval)))
			{
				/* If the Fall time is lower than rise time, increment the fall indx */
				while ((int16_t)(fallData[indxf]-riseData[indxr]) < 0)
				{
					indxf++;
				}

				/* If the Difference in fall time and rise time is >0 and less than rise average,
				 * Then we will register it as a success and increment the countrf (the number of successes)
				 */
				if (((int16_t)(fallData[indxf]-riseData[indxr]) >= 0) && (((int16_t)(fallData[indxf]-riseData[indxr]) <= riseavg)))
				{
					rfavg += MIN((fallData[indxf]-riseData[indxr]), (fallData[indxf+1]-riseData[indxr+1]));
					indxf++;
					indxr++;
					countrf++;
				}

				else
				{
					indxr++;
				}
			}

			/* Calculate the Average time between 2 Rise */
			rfavg = rfavg/countrf;

			/* Calculate Frequency
			 * Freq = Clock/(time taken between 2 Rise)
			 */
			frequency_notIdeal = (refClock/(float)riseavg);

			//int width1 = sprintf(str2, " TIM5 NotIdeal freq = %lu \r\n", frequency_notIdeal);
			//	HAL_UART_Transmit(&huart2, str2, width1, 100);
			/* Width of the pulse
			 *  = (Time between Rise and fall) / clock
			 */
			//width = ((rfavg)/((float)(refClock/1000000)))*1000;   // width in ns

			riseCaptured = 0;
			fallCaptured = 0;

			isMeasured = 1;

		}
		}
}





void frequencySequence()
{



	  if (isMeasured)
		  {
			  TIM2->CNT = 0;

			  HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_1, riseData, numval);
			  HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_2, fallData, numval);

			  isMeasured = 0;

		  }

HAL_Delay(100);/////////////////////////////////////////////////////////////////////////////////////


	  if (isMeasured)
	 		  {

	 			  TIM5->CNT = 0;

	 			  HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_1, riseData, numval);
	 			  HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_2, fallData, numval);

	 			  isMeasured = 0;

	 		  }



}



void pwmStart()
{
    TIM1->CCR1 = 5; //capture compare value
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
}

void freqInit()
{
	HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_1, riseData, numval);
	HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_2, fallData, numval);
    HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_1, riseData, numval);
    HAL_TIM_IC_Start_DMA(&htim5, TIM_CHANNEL_2, fallData, numval);
}
