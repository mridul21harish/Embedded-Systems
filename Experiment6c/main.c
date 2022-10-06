
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define SW1 *((unsigned int *) 0x40025040) //PF4 GPIO Pin (Switch 1)
#define SW2 *((unsigned int *) 0x40025004) //PF0 GPIO Pin (Switch 2)

#define BUTTON_PRESSED 0x00U      //Value when SW1, SW2 is pressed

// delay n milliseconds (16MHz CPU Clock)
void delayMs(int n)
{
  for(int i=0; i<n; i++)
  {
    for(int j=0; j<3180; j++); // do nothing for 1 ms
  }
}

int main()
{
  SYSCTL_RCGCGPIO_R = 0x20; // Enable clock on GPIO portF
  GPIO_PORTF_DIR_R = 0x0E; // Set pins 1,2,3 to output in portF
  GPIO_PORTF_DEN_R = 0x1F; // Digital function register on PF0, PF1, PF2, PF3 and PF4 for pull up switch in bit position 0,1,2,3,4
  GPIO_PORTF_CR_R = 0xFF; // remove write protect from the ports
  GPIO_PORTF_PUR_R = 0x11; // enable the pull up resistor PF0, PF4
  GPIO_PORTF_LOCK_R = 0x4C4F434B;  // Unlock GPIOCR Register
  
  int duty_cycle[4] = {0, 30, 60, 100};
  int counter = 1;
  
  int buttonPressed_Switch = 0;
  int buttonPressed_Select = 0;
  int clockwise = 1;

  SYSCTL_RCGCPWM_R |= 0x02; // Enable Clock to PWM1
  SYSCTL_RCGCGPIO_R |= 0x20; // enable clock to GPIOF
  SYSCTL_RCGCGPIO_R |= 0x02; // enable clock to GPIOB

  SYSCTL_RCC_R &= ~0x00100000; // Use System Clock for PWM
  PWM1_INVERT_R |= 0x80; // Positive Pulse
  PWM1_3_CTL_R = 0; // Disable PWM1_3 during Configuration
  PWM1_3_GENB_R = 0x0000080C; // Output High when Load and Low when Match
  PWM1_3_LOAD_R = 3999; // 4 KHz
  PWM1_3_CTL_R = 1; // Enable PWM1_3
  PWM1_ENABLE_R |= 0x80; // Enable PWM1

  GPIO_PORTF_DIR_R |= 0x08; // Set PF3 Pins as Output (LED) pin
  GPIO_PORTF_DEN_R |= 0x08; // Set PF3 Pins as Digital Pins
  GPIO_PORTF_AFSEL_R |= 0x08; // Enable Alternate Function
  GPIO_PORTF_PCTL_R &= ~0x0000F000; // Clear PF3 Alternate Function
  GPIO_PORTF_PCTL_R |= 0x00005000; // Set PF3 Alternate Funtion to PWM

  GPIO_PORTB_DEN_R |= 0x0C; // PB3 as Digital Pins
  GPIO_PORTB_DIR_R |= 0x0C; // Set PB3 as Output
  GPIO_PORTB_DATA_R |= 0x08; // Enable PB3

  while(1)
  {
    if((buttonPressed_Select) == 0 && (SW1 == BUTTON_PRESSED))
    {
      buttonPressed_Select = 1;
      PWM1_3_CMPB_R = 75 * duty_cycle[counter];
      counter = (counter + 1) % 4;
    }
    
    else if((buttonPressed_Select == 1) && (SW1 != BUTTON_PRESSED))
    {
      buttonPressed_Select = 0;
    }
	
    if((buttonPressed_Switch == 0) && (SW2 == BUTTON_PRESSED))
    {
      buttonPressed_Switch = 1;
      
      if(clockwise == 1)
      {
	GPIO_PORTB_DATA_R &= ~0x04;
	GPIO_PORTB_DATA_R |= 0x08;
      }
      
      else 
      {
	GPIO_PORTB_DATA_R &= ~0x08;
	GPIO_PORTB_DATA_R |= 0x04;
      } 
      
      clockwise = ~clockwise;
    }
    
    else if((buttonPressed_Switch == 1) && (SW2 != BUTTON_PRESSED))
    {
      buttonPressed_Switch = 0;
    }
  }
 // return 0;
}
