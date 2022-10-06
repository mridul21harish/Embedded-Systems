#include "tm4c123gh6pm.h"



// Required Micros

#define RED (1U << 1)

#define BLUE (1U << 2)

#define GREEN (1U << 3)

#define MAGENTA (RED | BLUE)

#define CYAN (BLUE | GREEN)

#define YELLOW (RED | GREEN)

#define WHITE (RED | BLUE | GREEN)

#define LED_OFF ~(RED | BLUE | GREEN) 



int main()

{

  volatile int result;

  /* Register setting as per Datasheet Pg 817, Section 13.4.1 - Module Initiallization */

  

  SYSCTL_RCGCGPIO_R = (1U << 4) | (1U << 5); // Enable Clock Gating Register on GPIO Port E and F

  SYSCTL_RCGCADC_R |= 1; // Enable ADC Clock Gating

  GPIO_PORTF_DIR_R = 0xFFU; // Set all pins of Port F to output

  GPIO_PORTF_DEN_R = 0xFFU; // Enable Digital function for all pins of Port F

  

  GPIO_PORTE_DIR_R &= ~8; // Disable digital function on PE3

  GPIO_PORTE_AMSEL_R |= 8; // Selecting Analog mode on PE3

  

  /* Rest of the Register setting is done as per Datasheet Pg 818, Section 13.4.2 - Sample Sequencer Configuration */

  

  ADC0_ACTSS_R &= ~8; // Disables SS3 during Configuration

  

  ADC0_EMUX_R &= ~0xF000; // Software trigger event. F is continuos sampling mode, given on Pg 833 of Datasheet

  ADC0_SSMUX3_R = 0; // get input from Channel 0, given on Pg 875 of Datasheet

  ADC0_SSCTL3_R |= 6;  // This register contains the configuration information for a sample executed with Sample Sequencer 3, given on Pg 876 of Datasheet 

  

  ADC0_ACTSS_R |= 8; // Enable ADC0 sequencer 3

  

  while(1)

  {

    ADC0_PSSI_R = 8; // start a conversion sequence 3. see pg 845

    if (ADC0_RIS_R & 8 == 0) // Wait for conversion to complete

    {

      result = ADC0_SSFIFO3_R; // Read conversion result

          

      // Store result in Port F Data Register

      if (result > 3584)

        GPIO_PORTF_DATA_R = WHITE;

      else if (result > 3072)

        GPIO_PORTF_DATA_R = CYAN;

      else if (result > 2560)

        GPIO_PORTF_DATA_R = MAGENTA;

      else if (result > 2048)

        GPIO_PORTF_DATA_R = YELLOW;

      else if (result > 1536)

        GPIO_PORTF_DATA_R = GREEN;

      else if (result > 1024)

        GPIO_PORTF_DATA_R = BLUE;

      else if (result > 512)

        GPIO_PORTF_DATA_R = RED;

      else

        GPIO_PORTF_DATA_R = LED_OFF;

    }

    

    ADC0_ISC_R = 8; // Clear completion flag     

  }

  

  return 0;

}

