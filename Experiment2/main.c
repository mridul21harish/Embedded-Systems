#define SYSCTL_RCGCGPIO_R (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DIR_R  (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R  (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_PUR_R  (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_CR_R   (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_PUR_R  (*((volatile unsigned long *)0x40025510))

// Different LED Colours
#define RED 0x02U
#define BLUE 0x04U
#define GREEN 0x08U
#define MAGENTA 0x06U   // RED + BLUE 
#define CYAN 0x0CU      // BLUE + GREEN
#define YELLOW 0x0AU    // RED + GREEN
#define WHITE 0x0EU     // RED + BLUE + GREEN
#define LED_OFF 0x00U   // LED is in OFF state

#define BUTTON_PRESSED 0x00U      //Value when SW1 is pressed

int main() 
{  
    SYSCTL_RCGCGPIO_R = 0x20;
    // this enables Port F from the clock gating register
    GPIO_PORTF_DIR_R = 0x1E;
    // this puts the direction of the Port F as output = 1 for 3 LEDs and PF4 for pull up switch in the bit position 1, 2, 3, 4 => 0x1E
    GPIO_PORTF_DEN_R = 0x0E;
    // this enables digital transfer between the Port F and the output
    GPIO_PORTF_CR_R = 0xFF;
    // this removes write protect from the ports
    GPIO_PORTF_PUR_R = 0x10;
    // this enables the pull up 
     
  int LEDsequence[8] = {LED_OFF, RED, BLUE, GREEN, MAGENTA, CYAN, YELLOW, WHITE};
  int itr = 0;
  int index = 0; //this will tell which colour LED to glow
  GPIO_PORTF_DATA_R = LEDsequence[index]; //initially LED is off
  
  while(1)
  {
    //simulating when PF4 switch is pressed
    if(itr%2 == 0)
      *((unsigned int *) 0x40025040) = BUTTON_PRESSED;
    else
      *((unsigned int *) 0x40025040) = ~BUTTON_PRESSED;

    //every 2nd iteration PF4 switch is pressed and LED lights up
    if(*((unsigned int *) 0x40025040) == BUTTON_PRESSED)
    {
      index = (index+1)%8;
       GPIO_PORTF_DATA_R = LEDsequence[index];
    }
       
    itr += 1;
  }
}
