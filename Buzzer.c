
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

void Buzzer_off(){

  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2,0); //Dsiable buzzer by writing 0 to pin 2 port A

}

void Buzzer_on(){

  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2,1); //Enable buzzer by writing 1 to pin 2 port A

}

void Buzzer_init(){
  
  SysCtlPeripheralEnable (SYSCTL_PERIPH_GPIOA); // enable peripheral for GPIO Port A
  while (!SysCtlPeripheralReady (SYSCTL_PERIPH_GPIOA )); // wait until peripheral is ready
  GPIOPinTypeGPIOOutput (GPIO_PORTA_BASE , GPIO_PIN_2 ) ; // set pin 2 of GPIO Port A as output
  GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0); // set pin 2 of GPIO Port A to 0
  
}