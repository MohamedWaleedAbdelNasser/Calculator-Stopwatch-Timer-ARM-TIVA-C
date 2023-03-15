#include "keypad.h"
#include "LCD.h"
#include "string.h"
#include "DIO.h"

void perform_operation (float num1, float num2, char operator ) { // function to perform operation based on operator
  
  switch (operator) { // switch statement to determine which operation to perform
    
  case '+': // if operator is '+'
    printf("%f" , (num1 + num2) ); // print result to console
    LCD_printString("Result:");
    LCD_printFloat(num1+num2); // print result to LCD
    break;
    
  case '-': // if operator is '-'
    printf("%f" , (num1 - num2) ); // print result to console
    LCD_printString("Result:");
    LCD_printFloat(num1-num2); // print result to LCD
    break;
    
  case '/': // if operator is '/'
    printf("%f" , (num1 / num2) ); // print result to console
    LCD_printString("Result:");
    LCD_printFloat(num1/num2); // print result to LCD
    break;
    
  case '*': // if operator is '*'
    printf("%f" , (num1 * num2) ); // print result to console
    LCD_printString("Result:");
    LCD_printFloat(num1*num2); // print result to LCD
    break;
  }
  
}




void calc_init(){ // function to initialize calculator
  
  char char_returned; // variable to store character returned by Keypad_read()
  char char_returned2; // variable to store character returned by Keypad_read()
  char string1[20] = "0"; // initialize string1
  char string2[20] = "0"; // initialize string2
  float num1; // variable to store first number
  float num2; // variable to store second number
  char operator; // variable to store operator
  
  while(1) { // infinite loop
    
    while(1){ // infinite loop
      char_returned = Keypad_read(); // read character from keypad
      if ((char_returned == '+') || (char_returned == '-') || (char_returned == '*') || (char_returned == '/')) { // if character is an operator
        operator = char_returned; // store operator in operator variable
        while (1) { // infinite loop
          char_returned2 = Keypad_read(); // read character from keypad
          
          if(char_returned2==('+') || char_returned2==('-') || char_returned2==('*') || char_returned2==('/')){ // if character is an operator
            LCD_command(0xc0); // move cursor to second line
            LCD_printString("error"); // print "error" to LCD
            
            delayMs(4000);
            
            // Clear the LCD screen and print the available modes
            LCD_command(0x01);
            LCD_printString("1-Calculator");
            LCD_command(0xC0);
            LCD_printString("2-Timer 3-StopW");
            
            // Return to the calling function
            return;
            
          }
          
          if (char_returned2 == ( '=')) { // if character is '='
            break; // break out of loop
          }
          else { // if character is not '='
            strncat(string2, &char_returned2, 1); // append character to string2
          }
        }
        break; // break out of loop
      }
      else { // if character is not an operator
        strncat(string1, &char_returned, 1); // append character to string1
      }
    }
    
    num1 = atof(string1); // convert string1 to float and store in num1
    num2 = atof(string2); // convert string2 to float and store in num2
    LCD_command(0xc0); // move cursor to second line
    perform_operation(num1, num2, operator); // perform operation based on operator and operands
    
    // Wait indefinitely until button on GPIO Port A pin 4 is pressed
    while (1) {
      
      // Check if the button on GPIO Port A pin 4 is pressed
      if ((DIO_ReadPin(&GPIO_PORTA_DATA_R,4)) == 1) {
        
        // Clear the LCD screen and print the available modes
        LCD_command(0x01);
        LCD_printString("1-Calculator");
        LCD_command(0xC0);
        LCD_printString("2-Timer 3-StopW");
        
        // Return to the calling function
        return;
      } 
      
    }
  }
}






