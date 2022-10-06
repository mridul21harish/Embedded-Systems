#include "tm4c123gh6pm.h"



// Different LED Colours

#define RED 0x02U

#define BLUE 0x04U

#define GREEN 0x08U

#define MAGENTA 0x06U   // RED + BLUE 

#define CYAN 0x0CU      // BLUE + GREEN

#define YELLOW 0x0AU    // RED + GREEN

#define WHITE 0x0EU     // RED + BLUE + GREEN



void configureSPIMaster(void);

void configureSPISlave(void);

void configureUART(void);



int decode(char message);



void sendChar(char data);

char recvChar(void);

void printChar(char c);

void printString(char *string);



void delayMs(int n);



int main()

{

  configureSPIMaster(); // Configure the SPI Master Module - Module 0

  configureSPISlave();  // Configure SPI Slave Module - Module 2

  configureUART();  // Configure UART Communication to read



  SYSCTL_RCGCGPIO_R |= 0x20U;  // enable clock on PortF

  GPIO_PORTF_DIR_R = 0x0EU;   // Set pins 1,2,3 to output in portF

  GPIO_PORTF_DEN_R = 0x0EU;  // Enable digital function on LED pins (PF1, PF2, PF3)

  

  // Define control variables

  char c;

  char message[7] = {'R', 'B', 'G', 'M', 'C', 'Y', 'W'};

  int index = 0; // this will tell which colour LED to glow

  

  while(1)

  {

    /* Send the Control Message from SSI0 to SSI2 */

    c = message[index];

    sendChar(c);

    

    /* Sent message */

    printString("Message sent from Master Module(SPI0 Module): ");

    printChar(c);

    printString("\n\r");

    

    delayMs(100);



    c = recvChar();  // Read the message at SSI2

   

    GPIO_PORTF_DATA_R = decode(c);  // Light the LED with the color code corresponding to the received message

    

    delayMs(100); 



    /* Print the message received through UART on the terminal */

    printString("Message Received in Slave Module(SPI2 Module): ");

    printChar(c);

    printString("\n\n\r");



    delayMs(2000);



    index = (index + 1) % 7; // Go onto the next control message index

  }

  

  return 0;

}



/*

Master SPI Module - Module 0

----------------------------

PA2 - SSI0Clk

PA3 - SSI0Fss

PA4 - SSI0Rx

PA5 - SSI0Tx

*/



// Define function to configure the SPI Master Module - SSI0

void configureSPIMaster(void)

{

  SYSCTL_RCGCSSI_R |= (1 << 0);  // Enable clock on appr SSI Module

  SYSCTL_RCGCGPIO_R |= (1 << 0);  // Enable GPIO Clock on PORT A

  GPIO_PORTA_AFSEL_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);  // Select Alternate functions for pins PA2-PA5

  GPIO_PORTA_PCTL_R = 0x00222200;  // Set control register for pins PA2-PA5

  GPIO_PORTA_DEN_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);  // Enable digital on pins PA2-PA5

  GPIO_PORTA_PUR_R |= (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5);  // Selecting pull up registers

  SSI0_CR1_R = 0;  // Disable SSI Module while configuring

  SSI0_CC_R = 0;  // Use the main system clock

  SSI0_CPSR_R = 64;  // Selecting divisor 64 for SSI Clock

  SSI0_CR0_R = 0x07;  // Set configuration of SSI - freescale mode, 8 bit data, steady low clock

  SSI0_CR1_R |= (1 << 1);  // Enable SPI

}



/*

Slave SPI Module - Module 2

---------------------------

PB4 - SSI2Clk

PB5 - SSI2Fss

PB6 - SSI2Rx

PB7 - SSI2Tx

*/



// Define function to configure the SPI Slave Module - SSI2

void configureSPISlave(void)

{

  SYSCTL_RCGCSSI_R |= (1 << 2);  // Enable clock on appr SSI Module

  SYSCTL_RCGCGPIO_R |= (1 << 1);  // Enable GPIO Clock on PORT B

  GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);  // Select Alternate functions for pins PB4-PB7

  GPIO_PORTB_PCTL_R = 0x22220000;  // Set control register for pins PB4-PB7

  GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);  // Enable digital on pins PB4-PB7

  GPIO_PORTB_PUR_R |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);  // Selecting pull up registers

  SSI2_CR1_R = 0;  // Disable SSI Module while configuring

  SSI2_CR0_R = 0x07;  // Set configuration of SSI - freescale mode, 8 bit data, steady low clock

  SSI2_CR1_R |= (1 << 1) | (1 << 2);  // Enable SPI and set slave

}



// Configure the UART Module 0

void configureUART(void)

{

  SYSCTL_RCGCUART_R |= (1 << 0);  // Enable clock to UART0

  GPIO_PORTA_AFSEL_R |= (1 << 0) | (1 << 1);  // Enable Alternate functions in PA0 and PA1

  GPIO_PORTA_PCTL_R |= 0x00000001 | 0x00000010;  // Set CTL for pins PA0 and PA1 as UART

  GPIO_PORTA_DEN_R |= (1 << 0) | (1 << 1);  // Enable DEN in PA0 and PA1

  UART0_CTL_R &= ~(1 << 0);  // Disable UART by clearing UARTEN in CTL

  UART0_IBRD_R = 104;  // Set BRD - integer portion

  UART0_FBRD_R = 11;  // Set BRD - fractional portion

  UART0_LCRH_R = (0x3 << 5) | (1 << 4);  // Set line control settings - 8 bit, no parity, 1-stop bit

  UART0_CTL_R = (1 << 0) | (1 << 8) | (1 << 9);  // Enable UART by setting UARTEN bit

}



int decode(char message)

{

  int data = 0x00;



  switch (message)

  {

    case 'R': 

        data = RED;

        break;

    case 'B': 

        data = BLUE;

        break;

    case 'G':

        data = GREEN;

        break;

    case 'M':

        data = MAGENTA;

        break;

    case 'C':

        data = CYAN;

        break;

    case 'Y':

        data = YELLOW;

        break;

    case 'W':

        data = WHITE;

        break;

  }

  

  return data;

}



// Define function to send data

void sendChar(char data)

{

  // wait for transmission to complete

   while((SSI0_SR_R & (1<<0) == 0));

   SSI0_DR_R = data;

}



// Define function to recv a character from slave module

char recvChar(void)

{

  char c;

  while((SSI2_SR_R & (1<<2) == 0));

  c = SSI2_DR_R;

  

  return c;

}



// Char stored in TX FIFO buffer and transfer it to computer

void printChar(char c)

{

  while((UART0_FR_R & (1<<5)) != 0);

  UART0_DR_R = c;

}



// Print string function send char by char to FIFO txbuffert o comp

void printString(char* string)

{

  while(*string)

    printChar(*(string++));

}



// delay n milliseconds (16MHz CPU Clock)

void delayMs(int n)

{

  for(int i=0; i<n; i++)

  {

    for(int j=0; j<3180; j++); // do nothing for 1 ms

  }

}





