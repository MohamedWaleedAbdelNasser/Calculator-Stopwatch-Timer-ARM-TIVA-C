#include "LCD.h"

void LCD_init (void) // function to initialize LCD
{
  // enable clock to GPIO Port A and Port B
  SYSCTL_RCGCGPIO_R |= 0x01;
  SYSCTL_RCGCGPIO_R |= 0x02;
  
  // set Port A pins 5, 6, and 7 as output
  GPIO_PORTA_DIR_R |= 0xE0;
  // enable digital functionality for Port A pins 5, 6, and 7
  GPIO_PORTA_DEN_R |= 0xE0;
  
  // set Port B as output
  GPIO_PORTB_DIR_R = 0xFF;
  // enable digital functionality for Port B
  GPIO_PORTB_DEN_R = 0xFF;
  
  // delay for 20ms
  delayMs (20);
  // send command to LCD
  LCD_command (0x30) ;
  // delay for 5ms
  delayMs (5);
  // send command to LCD
  LCD_command (0x30) ;
  // delay for 100us
  delayUs (100);
  // send command to LCD
  LCD_command (0x38) ;
  // send command to LCD
  LCD_command (0x06) ;
  // send command to LCD
  LCD_command (0x01) ;
  // send command to LCD
  LCD_command (0x0F) ;
}

// function to send command to LCD
void LCD_command (uint8 command)
{
  // set Port A pin 5, 6, and 7 to low
  GPIO_PORTA_DATA_R=0;
  // set Port B to command
  GPIO_PORTB_DATA_R=command;
  // set Port A pin 6 to high
  GPIO_PORTA_DATA_R=EN;
  // delay for 0us
  delayUs(0);
  // set Port A pin 6 to low
  GPIO_PORTA_DATA_R=0;
  // delay for 2ms or 40us depending on command
  if (command<4)
  {
    delayMs(2);
  }
  else
  {
    delayUs(40);
  }
}

// function to send data to LCD
void LCD_data(uint8 data)
{
  // set Port A pin 5 to high
  GPIO_PORTA_DATA_R=RS;
  // set Port B to data
  GPIO_PORTB_DATA_R=data;
  // set Port A pin 6 and 5 to high
  GPIO_PORTA_DATA_R=(EN | RS) ;
  // delay for 0us
  delayUs(0);
  // set Port A pin 6 and 5 to low
  GPIO_PORTA_DATA_R=0;
  // delay for 40us
  delayUs(40);
}

// function to delay for specified number of milliseconds
void delayMs(uint32 n)
{
  // loop n times
  for(int i=0;i<n;i++)
  {
    // inner loop to create the delay
    for(int j=0;j<3180;j++)
    {}
  }
}

// function to delay for a given number of microseconds
void delayUs(uint32 n)
{
  // loop n times
  for(int i=0;i<n;i++)
  {
    // inner loop to create the delay
    for(int j=0;j<3;j++)
    {}
  }
}

// function to print an integer to the LCD
void LCD_printInt(uint32 no)
{
  // create char array to store integer as a string
  char toprint[4] = {0};
  // convert integer to string and store in toprint
  sprintf(toprint, "%d", no);
  // loop through toprint array and print each character to LCD
  for (int i = 0; i < sizeof(toprint)/sizeof(char); i++)
  {
    LCD_data(toprint[i]);
  }
}

// function to print an integer to the LCD in the format "00:00" for a stopwatch or timer
void LCD_printInt_Timer_Stopwatch(uint32 no)
{
  char toprint[10] = {0};

  // Calculate the number of minutes and seconds
  int minutes = no / 60;
  int seconds = no % 60;

  // Format the string to print
  sprintf(toprint, "%02d:%02d", minutes, seconds);

  // Print the string to the LCD
  int i = 0;
  while(toprint[i] != '\0')
  {
    LCD_data(toprint[i]);
    i++;
  }
}

// function to print a float to the LCD
void LCD_printFloat(float no)
{
  // create char array to store float as a string
  char toprint[10] = {0};
  // convert float to string and store in toprint
  sprintf(toprint, "%f", no);
  // loop through toprint array and print each character to LCD
  
  for (int i = 0; i < sizeof(toprint)/sizeof(char); i++)
  {
    LCD_data(toprint[i]);
  }
}

// function to print a string to the LCD
void LCD_printString(char* str)
{
  // set LCD cursor to beginning of second line
  int i = 0;
  // Send each character of the string to the LCD screen
  while(str[i] != '\0') {
    LCD_data(str[i]);
    i++;
  }
}
