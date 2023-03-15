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
#include <inttypes.h>
#include "buzzer.h"


int TicksTosecTimer(int x){
  
  return (x / 15999999);  // This function converts the timer period in ticks to seconds

}

int secToTicksTimer(int sec){
  
  return sec* 15999999;  // This function converts seconds to timer period in ticks

}

// This is the interrupt service routine for Timer 1
void Timer1_ISR_Timer(){
  
  int x = TicksTosecTimer(TimerValueGet64(WTIMER0_BASE));  // Convert the timer period in ticks to seconds
  TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);  // Clear the timer interrupt
  LCD_printString("Time: ");  // Print the timer value to the LCD screen
  LCD_printInt_Timer_Stopwatch(x);  // Print the timer value to the LCD screen
  LCD_command(0x80);  // If the timer value is not zero, send a command to the LCD screen
  
}
  

// This is the interrupt service routine for Timer 0
void Timer0_ISR_Timer(){ 
  
  TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntClear(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);
  Buzzer_on();
  delayMs(4000);
  Buzzer_off();  
  
}


void GTimer_init (uint64_t s,uint64_t m) { // function to initialize timer
  
  uint64_t Time = ((m*60) + s) * 16000000 ; // calculate time in microseconds
  
  LCD_init (); // initialize LCD
  Buzzer_init(); // initialize Buzzer
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);  // Enable the Wide Timer 0 peripheral
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_WTIMER0));  // Wait for the Wide Timer 0 peripheral to be ready
  TimerConfigure(WTIMER0_BASE,TIMER_CFG_ONE_SHOT);  // Configure the Wide Timer 0 peripheral as a one-shot timer
  TimerLoadSet64(WTIMER0_BASE,(Time));  // Load the specified time value into the Wide Timer 0 load register
  TimerEnable(WTIMER0_BASE,TIMER_BOTH);  // Enable both the A and B timers of Wide Timer 0
  TimerIntEnable(WTIMER0_BASE,TIMER_TIMA_TIMEOUT);  // Enable the timeout interrupt for the A timer of Wide Timer 0
  TimerIntRegister(WTIMER0_BASE,TIMER_BOTH,Timer0_ISR_Timer);  // Register the Timer0_ISR_Timer function as the handler for Wide Timer 0 interrupts
  
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // enable peripheral for Timer 1
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)); // wait until peripheral is ready
  TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC); // configure Timer 1 as periodic timer
  TimerLoadSet(TIMER1_BASE,TIMER_BOTH,15999999); // set Timer 1 to 15999999
  TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT); // enable interrupt for Timer 1
  TimerIntRegister(TIMER1_BASE,TIMER_BOTH,Timer1_ISR_Timer); // register interrupt service routine for Timer 1
  TimerEnable(TIMER1_BASE,TIMER_BOTH); // enable Timer 1
  
  while(1) {
    // Check if button on GPIO port A pin 4 is pressed
    if (DIO_ReadPin(&GPIO_PORTA_DATA_R,4) == 1){
      TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);   // Disable timer 1 interrupt
      
      // Clear LCD screen and display menu options
      LCD_command(0x01);
      LCD_printString("1-Calculator");
      LCD_command(0xC0);
      LCD_printString("2-Timer 3-StopW");
      
      // Return to previous menu
      return;
    }
  }
  
}



