#define clock_gating_REG (*(unsigned int *) 0x400FE608) 
//this is the clock gating register which is used to enable clock on the GPIO registers
#define direction_REG (*(unsigned int *) 0x40025400)              
//this gives the direction for the input and output
#define digital_function_REG (*(unsigned int *)0x4002551C)    
//this is the digital function register
#define LED (*(unsigned int *)0x40025038)
//address for red == 0x40025008, address for blue = 0x40025010, address for green = 0x40025020; thus enabling the LED for all the red,
//blue and green LEDs will be the sum i.e. 0x40025038

void delay(int time);
//this is the function for adding delay in between enabling the different LEDs

void red();
//this is the function for enabling red LED
void blue();
//this is the function for enabling blue LED
void green();
//this is the function for enabling green LED
void off();
//this is the function for disabling all the LEDs

void red()
{
   LED = 0x00000002;
}//this turns on the red LED

void blue()
{
   LED = 0x00000004;
}//this turns on the blue LED

void green()
{
   LED = 0x00000008;
}//this turns on the green LED

void off()
{
   LED = 0x00000000;
}//this turns of all the LEDs

int main(void)
{
    clock_gating_REG = 0x00000020;
    //bits 31:6 =0 bit 5= 1 bits 4:0 = 0  in hex = 0x 00000020
    direction_REG = 0x0000000E;        
    //bits 31:8 = 0, bits 7:4 also = 0, bits 3:1 = 1 , bit 0 = 0, port 0 is unused as it is a special GPIO PIN which needs to be 
    //unlocked to be used
    digital_function_REG = 0x0000000E;
    
    while(1)
    {
          red();
          delay(1);
          off();
          blue();
          delay(2);
          off();
          green();
          delay(3);
          off();
    }
}

void delay(int time)
{
     int i; int j;
     for(i = 0; i < time; i = i+1)
     {
        for(j = 0; j < 1000000; j = j+1)
        {
        }
     }
}