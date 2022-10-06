#include <stdint.h>
#include "tm4c123gh6pm.h"

// Different LED Colours
#define RED 0x02U
#define BLUE 0x04U
#define GREEN 0x08U
#define MAGENTA 0x06U   // RED + BLUE 
#define CYAN 0x0CU      // BLUE + GREEN
#define YELLOW 0x0AU    // RED + GREEN
#define WHITE 0x0EU     // RED + BLUE + GREEN

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
  int x = 15999;
  
  // Enable Peripheral Clocks
  SYSCTL_RCGCPWM_R |= 2; // Enable Clock to PWM1
  SYSCTL_RCGCGPIO_R |= 0x20; // Enable Clock to Port F
  SYSCTL_RCC_R &= ~0x00100000; // No Pre-divide for PWM Clock
  
  // Enable port PF2 for PWM1 M1PWM7 
  GPIO_PORTF_AFSEL_R = 0x0E; // PF2 uses Alternate Function
  GPIO_PORTF_PCTL_R |= 0x000005550;
  GPIO_PORTF_DEN_R = 0x02; // Pin Digital
  
  PWM1_2_CTL_R = 0; // Stop Counter
  PWM1_2_GENB_R = 0x0000008C; // M1PWM6 Output Set when Reload
  
  PWM1_3_CTL_R = 0;// Stop Counter
  PWM1_3_GENA_R = 0x0000008C; // M1PWM6 Output Set when Reload
  PWM1_3_GENB_R = 0x0000008C; // M1PWM6 Output Set when Reload
  
  // Clear when Match PWMCMPA
  PWM1_2_LOAD_R = 16000; // Set Load Value for 1KHz (16MHz / 16000)
  PWM1_2_CMPA_R = 15999; // Set Duty Cycle to min
  PWM1_2_CTL_R = 1; // Start Timer
  
  PWM1_3_LOAD_R = 16000; // Set Load Value for 1KHz (16MHz / 16000)
  PWM1_3_CMPA_R = 15999; // Set Duty Cycle to min
  PWM1_3_CTL_R = 1; // Start Timer
  
  PWM1_ENABLE_R = 0xE0; // start PWM1 ch7
  
  int LEDsequence[7] = {RED, BLUE, GREEN, MAGENTA, CYAN, YELLOW, WHITE};
  int index = 0;
  
  while(1)
  {
    GPIO_PORTF_DEN_R = LEDsequence[index];
    index = (index + 1) % 7;
    
    // Increasing the contrast of LED
    while(x > 0)
    {
      x = x - 50;
      PWM1_2_CMPA_R = x;
      PWM1_3_CMPA_R = x;
      delayMs(5);
    }
    
    // Decreasing the contrast of LED
    while(x < 15999)
    {
      x = x + 50;
      PWM1_2_CMPA_R = x;
      PWM1_3_CMPA_R = x;
      delayMs(5);
    }
  }
  
  //return 0;
}

