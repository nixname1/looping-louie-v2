#include "ll_system.h"
#include "ll_motor.h"

/**
 * @brief initializes the motor module
 * sets up a timer on the specified LL_MOTOR_PIN on LL_MOTOR_PORT
 */
void ll_motor_init()
{
  /** enable GPIO Port */
  // enable clock on GPIO B
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  // Set Pin 2 as output
  GPIOB->MODER |= GPIO_MODER_MODER2_0;
  // set to low speed (2MHz)
  GPIOB->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR2);


}

/**
 * @brief de-initializes the motor module
 */
void ll_motor_deinit()
{

}

/**
 * @brief
 * @param val	expects the speed of the motor in precent
 * @param dir	the direction of the motor
 * 				0=forward 1=backward
 * @retval		0 on success, -1 on failure
 */
int ll_motor_set_speed(int dir, int val)
{
//  int i;
//  switch(dir)
//  {
//	// move forward
//	case 0:
//	  // 20ms per signal * 15 = 300ms
//	  for(i = 0 ; i < 15; i++)
//	  {
//		GPIOB->ODR |= GPIO_ODR_ODR_2;
//		delay_us(500);
//		GPIOB->ODR &= ~(GPIO_ODR_ODR_2);
//		delay_us(20000-500);
//	  }
//	  for(i = 0; i < 15; i++)
//	  {
//		GPIOB->ODR |= GPIO_ODR_ODR_2;
//		//delay_us();
//		GPIOB->ODR &= ~(GPIO_ODR_ODR_2);
//		delay_us(20000-500);
//	  }
//	  break;
//
//	// move backward
//	case 1:
//	  // 20ms per signal * 15 = 300ms
//	  for(i = 0 ; i < 15; i++)
//	  	  {
//	  		GPIOB->ODR |= GPIO_ODR_ODR_2;
//	  		delay_us(1500);
//	  		GPIOB->ODR &= ~(GPIO_ODR_ODR_2);
//	  		delay_us(20000-1500);
//	  	  }
//	break;
//
//	default:
//	  return -1;
//  }
  return 0;
}

/**
 * @brief stops the motor
 */
void ll_motor_stop()
{
  ll_motor_set_speed(0, 0);
}
