#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "tm4c123gh6pm.h"
#include "inc/hw_ints.h"
#include "keypad.h"
#include "stdio.h"
#include <stdlib.h>
#include "DIO.h"
#include "inc/hw_ints.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "stdio.h"
#include <stdlib.h>
#include "LCD.h"
#include "stopwatch.h"
#include "timer.h"
#include "calculator.h"
#include <string.h>

int Counter;

void blinky1(){
  
  while(1){
  Counter = 0;
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);    
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_1));
  
    while(Counter < 5){
      break;
    }
    
  
  }
}

void blinky2(){
  
  while(1){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);    
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2,~GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_2));
    while(Counter < 10){
      break;
    }
    
  
  }
}

void sys_handler(){
  
  Counter ++;
  
}

int main(){
  
  __asm("CPSIE I");
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  SysTickEnable();
  SysTickIntEnable();
  SysTickIntRegister(sys_handler);
  SysTickPeriodSet(0.1*16e6-1);
  blinky1();
  blinky2();
  
  while(1){
    
  }
  
}
//
//int min;
//int sec;
//char mins[20] = "0";
//char secs[20] = "0";
//char charSec_returned;
//char charMin_returned;
//
//int main()
//{
//  __asm("CPSIE I"); //Enable I bit for use of intrupts
//  LCD_init(); // Initialize LCD screen
//    
//  // Enable and configure GPIO Port F
//  SYSCTL_RCGCGPIO_R |= 0X20;  // Set the clock for Port F
//  while((SYSCTL_PRGPIO_R & 0X20)==0);  // Wait until the clock is ready
//  GPIO_PORTF_CR_R|= 0X11;  // Set the commit register for Port F
//  GPIO_PORTF_DIR_R|= 0X00;  // Set the direction of Port F as input
//  GPIO_PORTF_LOCK_R=0x4c4f434b; // Set the lock register for Port F
//  GPIO_PORTF_PUR_R |= 0x11;  // Set the pull-up resistors for Port F
//  GPIO_PORTF_DEN_R|= 0X11;  // Set the digital enable register for Port F
//  
//  // Enable and configure GPIO Port A
//  
//  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
//  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
//  GPIOPinTypeGPIOInput(GPIO_PORTA_BASE,(GPIO_PIN_3 | GPIO_PIN_4));
//  
//  
//  // Display menu on LCD screen
//  
//  LCD_command(0x01);
//  LCD_printString("1-Calculator");
//  LCD_command(0xC0);
//  LCD_printString("2-Timer 3-StopW");
//  
//  // Wait for user input
//  
//  while(1){
//    
//    
//    // If user selects calculator mode
//    if((DIO_ReadPin(&GPIO_PORTF_DATA_R,0)) == 0){
//      LCD_command(0x01);  // Clear LCD screen
//      KeyPad_Init();  // Initialize keypad
//      LCD_printString("Calc:"); // Prompt user to enter equation
//      calc_init();  // Initialize calculator mode
//    }
//    
//    
//    // If user selects timer mode
//    if((DIO_ReadPin(&GPIO_PORTF_DATA_R,4)) == 0){
//      KeyPad_Init(); // Initialize keypad
//      LCD_command(0x01); // Clear LCD screen
//      LCD_printString("Mins :"); // Prompt user to enter minutes
//      
//      // Read minutes input from
//      while (1) {
//        
//        charMin_returned = Keypad_read();  // Read input from keypad
//        
//        if ( charMin_returned == '=') { // If user is done entering minutes     
//          break; // Exit loop
//        }
//        else // If user is still entering minutes
//          strncat(mins, &charMin_returned, 1);  // Add input to mins string
//      }
//      
//      int min = atoi(mins); // Convert mins string to integer
//      LCD_command(0xC0);  // Move cursor to second line
//      LCD_printString("Secs :");  // Prompt user to enter seconds
//      
//      // Read seconds input from keypad 
//      while (1) {
//        
//        char charSec_returned = Keypad_read(); // Read input from keypad 
//        if (charSec_returned == '=') { // If user is done entering seconds
//          break; // Exit loop  
//        }    
//        else // If user is still entering seconds 
//          strncat(secs, &charSec_returned, 1); // Add input to secs string
//      }
//      
//      int sec = atoi(secs); // Convert secs string to integer 
//      GTimer_init(sec,min);  // Initialize timer with user input values
//    }
//    
//    
//    // If user selects stopwatch mode  
//    if (DIO_ReadPin(&GPIO_PORTA_DATA_R,3) == 1){
//      LCD_command(0x01);// Clear LCD screen
//      stopwatch_init(); // Initialize stopwatch mode
//    }
//  }
//  
//  return 0;
//}