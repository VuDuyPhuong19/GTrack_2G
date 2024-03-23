#include "LowPowerMode.h"

void Entry_StopMode(void)
{
		HAL_UART_Transmit(&huart1, "STOP MODE is ON\n\n", sizeof ("STOP MODE is ON\n\n"), HAL_MAX_DELAY); 

		// Sleep mode test
		HAL_SuspendTick();
						
		// Sleep mode 				
    //HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
						
		// Stop mode				
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);		

}
void Exit_StopMode(void)
{
		// For stop mode
		SystemClock_Config();
		HAL_ResumeTick();
}

void Entry_StandbyMode(void)
{
  /** Now enter the standby mode **/
   /* Clear the WU FLAG */
  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);


   /* Display the string */
		HAL_UART_Transmit(&huart1, "About to enter the STANDBY MODE\n\n", sizeof ("About to enter the STANDBY MODE\n\n"),HAL_MAX_DELAY); 

   /* Blink the LED */
  for (int i=0; i<5; i++)
  {
	  HAL_GPIO_TogglePin(GPIOA, pa12_Pin);
	  HAL_Delay(300);
  }
			HAL_UART_Transmit(&huart1, "STANDBY MODE is ON\n\n", sizeof ("STANDBY MODE is ON\n\n"), HAL_MAX_DELAY); 
   /* Enable the WAKEUP PIN */
	
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
    HAL_Delay(500);
  
   /* Finally enter the standby mode */
  HAL_PWR_EnterSTANDBYMode();
}

void Exit_StandbyMode(void)
{
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
	  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);  // clear the flag

	  /** display  the string **/
		HAL_UART_Transmit(&huart1, "Wakeup from the STANDBY MODE\n\n", sizeof ("Wakeup from the STANDBY MODE\n\n"), HAL_MAX_DELAY); 
	  /** Blink the LED **/
	  for (int i=0; i<3; i++)
	  {
		  HAL_GPIO_TogglePin(GPIOA, pa11_Pin);
		  HAL_Delay(200);
	  }

	  /** Disable the WAKEUP PIN **/
	  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);  // disable PA0
		}

}
