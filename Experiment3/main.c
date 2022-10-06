// Required Registers 
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_LOCK_R       (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))

#define SW1 *((unsigned int *) 0x40025040) //PF4 GPIO Pin (Switch 1)
#define SW2 *((unsigned int *) 0x40025004) //PF0 GPIO Pin (Switch 2)

// Different LED Colours
#define RED 0x02U
#define BLUE 0x04U
#define GREEN 0x08U
#define MAGENTA 0x06U   // RED + BLUE 
#define CYAN 0x0CU      // BLUE + GREEN
#define YELLOW 0x0AU    // RED + GREEN
#define WHITE 0x0EU     // RED + BLUE + GREEN
#define LED_OFF 0x00U   // LED is in OFF state

#define BUTTON_PRESSED 0x00U      //Value when SW1, Sw2 is pressed

//simulating SW1, SW2 button pressing
int simulate_switch(int itr)
{  
  int flag;
  
    if(itr%4 == 0) // LED in off state
    {
      SW1 = ~BUTTON_PRESSED;  // off state
      SW2 = ~BUTTON_PRESSED;  // off state
      flag = 0;
    }
    else if(itr%4 == 1) //Primary Colours are shown
    {
      SW1 = ~BUTTON_PRESSED;  // off state
      SW2 = BUTTON_PRESSED;   // on state
      flag = 1;
    }
    else if(itr%4 == 2) //Secondary Colours are shown
    {
      SW1 = BUTTON_PRESSED;   // on state
      SW2 = ~BUTTON_PRESSED;  // off state
      flag = 2;
    }
    else //White LED is glown
    {
      SW1 = BUTTON_PRESSED;   // on state
      SW2 = BUTTON_PRESSED;   // on state
      flag = 3;
    }
    
    return flag;
}

int main()
{
  // Enable clock on GPIO portF
  SYSCTL_RCGCGPIO_R = 0x20;
  // Set pins 1,2,3 to output in portF
  GPIO_PORTF_DIR_R = 0x0E;
  // Digital function register on PF0, PF1, PF2, PF3 and PF4 for pull up switch in bit position 0,1,2,3,4
  GPIO_PORTF_DEN_R = 0x1F;
  // Unlock GPIOCR Register
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  // remove write protect from the ports
  GPIO_PORTF_CR_R = 0xFF;
  // enable the pull up resistor PF0, PF4
  GPIO_PORTF_PUR_R = 0x11;
  
  int Primary[3] = {RED, BLUE, GREEN};
  int Secondary[3] = {MAGENTA, CYAN, YELLOW};
  int itr = 0;
  int index = 0; //this will tell which colour LED to glow
  
  while(1)
  {
    int flag = simulate_switch(itr);
    
    if (flag == 0) // off state
      GPIO_PORTF_DATA_R = LED_OFF;
    
    else if(flag == 1) //Primary Colour LED
    {
      static int index = 0;
      GPIO_PORTF_DATA_R = Primary[index];
      index = (index + 1) % 3;
    }
    
    else if(flag == 2) //Secondary Colour LED
    {
      static int index = 0;
      GPIO_PORTF_DATA_R = Secondary[index];
      index = (index + 1) % 3;
    }
    
    else //White LED
      GPIO_PORTF_DATA_R = WHITE;
       
    itr += 1;
  }
  
  return 0;
}
