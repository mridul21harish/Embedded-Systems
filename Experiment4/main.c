// Required Registers 
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTC_DIR_R        (*((volatile unsigned long *)0x40006400))
#define GPIO_PORTC_DEN_R        (*((volatile unsigned long *)0x4000651C))

#define C4      *((volatile unsigned int *)0x40006040) // C4 pin for PortC
#define C5      *((volatile unsigned int *)0x40006080) // C5 pin for PortC
#define C6      *((volatile unsigned int *)0x40006100) // C6 pin for PortC
#define C7      *((volatile unsigned int *)0x40006200) // C7 pin for PortC

void delay()
{
   for(int i=0; i<1000000; i++);
}

int main()
{
  // Enable clock on GPIO portC
  SYSCTL_RCGCGPIO_R = 0x04;
  // Set direction of pins C4, C5, C6, C7 to output for PortC
  GPIO_PORTC_DIR_R = 0xF0;
  // Set digital enable of pins C4, C5, C6, C7 for PortC
  GPIO_PORTC_DEN_R = 0xF0;
  
  while(1)
  {
    // MSB - C7
    // LSB - C4
    
    // 0001
    C4 = 0x10U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x00U; // Set PC7
      
    delay();
      
    // 0010
    C4 = 0x00U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x00U; // Set PC7
      
    delay();
      
    // 0011
    C4 = 0x10U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x00U; // Set PC7
    
    delay();
      
    // 0100
    C4 = 0x00U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x00U; // Set PC7
    
    delay();
      
    // 0101
    C4 = 0x10U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x00U; // Set PC7
      
    delay();
      
    // 0110
    C4 = 0x00U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x00U; // Set PC7
    
    delay();
      
    // 0111
    C4 = 0x10U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x00U; // Set PC7
    
    delay();
      
    // 1000
    C4 = 0x00U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
      
    // 1001
    C4 = 0x10U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
    
    // 1010
    C4 = 0x00U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
      
    // 1011
    C4 = 0x10U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x00U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
      
    // 1100
    C4 = 0x00U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
      
    // 1101
    C4 = 0x10U; // Set PC4
    C5 = 0x00U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x80U; // Set PC7
      
    delay();
     
    // 1110
    C4 = 0x00U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();
      
    // 1111
    C4 = 0x10U; // Set PC4
    C5 = 0x20U; // Set PC5
    C6 = 0x40U; // Set PC6
    C7 = 0x80U; // Set PC7
    
    delay();

  }
  //return 0;
}
