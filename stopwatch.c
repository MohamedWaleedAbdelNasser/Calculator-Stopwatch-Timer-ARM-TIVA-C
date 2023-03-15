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

// Converts the given number of timer ticks to seconds
int Tickstosec(int x){
  
  int k = x / 15999999;  // Calculate the number of seconds by dividing the number of ticks by the number of ticks per second
  return k;
  
}

// Converts the given number of seconds to timer ticks
int secToTicks(int sec){
  
  return sec*15999999;  // Calculate the number of ticks by multiplying the number of seconds by the number of ticks per second and subtracting 1
  
}

// Interrupt handler for TIMER1
void Timer1_ISR(){
  
  int x = Tickstosec(TimerValueGet64(WTIMER0_BASE));  // Calculate the elapsed time in seconds using the Tickstosec() function
  TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);  // Clear the interrupt for TIMER1
  LCD_printString("Time: ");  // Display the elapsed time on the LCD display
  LCD_printInt_Timer_Stopwatch(x); // Display the elapsed time on the LCD display
  LCD_command(0x80);
}

// Interrupt handler for TIMER0
void Timer0_ISR(){
  
  TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntClear(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
  
}


// Interrupt handler for the button connected to GPIO port F
void pause(){
  // Static variable to keep track of the state of the stopwatch (running or paused)
  static int x=0;
  if (x==0){      
    // If the stopwatch is running, disable TIMER0 and TIMER1 and display the current elapsed time on the LCD display
    TimerDisable(WTIMER0_BASE,TIMER_BOTH);
    TimerDisable(TIMER1_BASE,TIMER_BOTH);
    LCD_command(0x80);
      LCD_printString("Time: ");  // Print the timer value to the LCD screen
    LCD_printInt_Timer_Stopwatch(Tickstosec(WTIMER0_TAR_R));
    x++;    // Update the state of the stopwatch to paused
    
  }
  else{
    // If the stopwatch is paused, enable TIMER0 and TIMER1 and move the cursor to the beginning of the first line of the LCD display
    TimerEnable(WTIMER0_BASE,TIMER_BOTH);
    TimerEnable(TIMER1_BASE,TIMER_BOTH);      
    LCD_command(0x80);
    x--;    // Update the state of the stopwatch to running
    
  }
  
  delayMs(200); // delay for debouncing
  GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_0); // clear intruppt of pin 0 port f
  
}


// Initializes the stopwatch feature
void stopwatch_init(){
  
    SYSCTL_RCGCGPIO_R |= 0X20;
  while((SYSCTL_PRGPIO_R & 0X20)==0);
  GPIO_PORTF_CR_R|= 0X11;
  GPIO_PORTF_DIR_R|= 0X00;
  GPIO_PORTF_LOCK_R=0x4c4f434b;
  GPIO_PORTF_PUR_R |= 0x11;
  GPIO_PORTF_DEN_R|= 0X11;
  
  // Enable and configure interrupt for pause button on Port F pin 0
  GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_0);
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
  GPIOIntRegisterPin(GPIO_PORTF_BASE,GPIO_INT_PIN_0,pause);
  
  
  // Initialize LCD display
  LCD_init(); 
  
  // Enable and configure Timer 0 for stopwatch
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);  // Enable the wide timer 0 peripheral
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER0));  // Wait for the timer to be ready
  TimerConfigure(WTIMER0_BASE,TIMER_CFG_ONE_SHOT_UP);  // Configure the timer as a one-shot timer that counts up
  TimerLoadSet64(WTIMER0_BASE,9221312036854775801);  // Load the timer with the desired timeout value
  TimerEnable(WTIMER0_BASE,TIMER_BOTH);  // Enable the timer
  TimerIntEnable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);  // Enable timer interrupts
  TimerIntRegister(WTIMER0_BASE,TIMER_BOTH,Timer0_ISR);  // Register the interrupt handler
  
  
  // Enable and configure Timer 1 for LCD refresh
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  // Enable timer1 peripheral
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));  // Wait for timer1 to be ready
  TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);  // Configure timer1 as a periodic timer  
  TimerLoadSet(TIMER1_BASE,TIMER_BOTH,15999999);  // Set the load value for timer1
  TimerEnable(TIMER1_BASE,TIMER_BOTH);  // Enable timer1  
  TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);  // Enable timeout interrupts for timer1
  TimerIntRegister(TIMER1_BASE,TIMER_BOTH,Timer1_ISR);  // Register ISR for timer1 timeout interrupts
  
  WTIMER0_TAR_R=0;
  WTIMER0_TAV_R=0;
  
  // Reset Timer 0
  
  
  
  while(1) {
    
    // If button 0 is pressed, call the pause function
    if ((DIO_ReadPin(&GPIO_PORTF_DATA_R,0)) == 0)
      pause(); // call pause function to activate
    
    // If button 4 is pressed, reset the timer registers
    if ((DIO_ReadPin(&GPIO_PORTF_DATA_R,4)) == 0){
      
      WTIMER0_TAR_R=0;
      WTIMER0_TAV_R=0;   // Reset the value of the timer's current actual value register (TAV) to 0
    }
    
    // If button 4 is pressed, exit the function and return to the main menu
    if ((DIO_ReadPin(&GPIO_PORTA_DATA_R,4)) == 1){
      
      TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT); // This line disables the timeout interrupt for Timer A of Timer 1
      LCD_command(0x01);
      LCD_printString("1-Calculator");
      LCD_command(0xC0);
      LCD_printString("2-Timer 3-StopW");
      
      return;
    }
    
  }
}
