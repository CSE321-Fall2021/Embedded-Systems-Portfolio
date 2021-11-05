/** CSE321 Project 2:
Designing an all-in-one count-down alarm system that will accept user input on the 4x4 keypad
and display it on the LCD to set timer values. Press D to input timer values. Press A to start timer.
Press B to terminate timer at any point of execution. 
Author: Bodhiswattwa Basu 
Date: 5th November, 2021
Fall 2021 Course: Real-Time Embedded Operating Systems, CSE 321 
Function(s):
      1. main
      2. ISRs: isr_col1, isr_col2, isr_col3, isr_col4
      3. key_A
      4. key_numbers
      5. time_decrement
Description:
      1. Main function controls the functionality of the program by providing power to the columns of the keypad and by printing to the LCD.
      2. ISR functions indicate when the appropriate key has been pressed on
      the serial monitor.
      3. key_A takes the user input of the timer value and aggregates a total time value.
      4. key_numbers takes in the user input of the timer value
      5. timer_decrement is the function that the ticker visits every second to decrease the value of the timer.
Parameters: 
        N/A
Input (Peripherals):
      1. Solderless breadboard
      2. 7 x LEDs
      3. Jumper wires
      4. LCD
      5. 4x4 Keypad
      6. Nucleo L4R5ZI
      7. 7 x Resistors
      8. 8 Male to Female Jumper wires
Breadboard & Nucleo Interfacing: 
      1. Row 1 == PC_8              5. Column 1 == PA_6
      2. Row 2 == PC_9              6. Column 2 == PA_5
      3. Row 3 == PC_10             7. Column 3 == PA_7
      4. Row 4 == PC_11             8. Column 4 == PD_14
Output (Peripherals):
      1. LEDs --
        (i) LED 1 == PA_3
        (ii) LED 2 == PC_0
        (iii) LED 3 == PC_3
      2. LCD
Sources/References Used:
      1. STMicroelectronics, RM0432 Reference Manual, Rev. 7, August 2020.
      Available: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
      2. STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020.
      Available: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
      3. LCD API/Library Instructions

**/

#include "PinNamesTypes.h"
#include "Thread.h"
#include "lcd1602.h"
#include "mbed.h"
#include "mbed_events.h"
#include "stm32l4r5xx.h"

// ------------------------------------------- Interrupts and ISRs -------------------------------------------

void isr_col1(void);                                        // When column 1 rises
void isr_col2(void);                                        // When column 2 rises 
void isr_col3(void);                                        // When column 3 rises
void isr_col4(void);                                        // When column 4 rises

InterruptIn col_1(PA_6, PullDown);                          // Detect interrupt when PA_6 is ACTIVE.
InterruptIn col_2(PA_5, PullDown);                          // Detect interrupt when PA_5 is ACTIVE.
InterruptIn col_3(PA_7, PullDown);                          // Detect interrupt when PA_7 is ACTIVE.
InterruptIn col_4(PD_14, PullDown);                         // Detect interrupt when PD_14 is ACTIVE.

// ------------------------------------------- Timer Global Variables -------------------------------------------

char fin[5] = "m:ss";                                       // User input of keypad presses
char count_down[5] = "m:ss";                                // Countdown of keypad press timer value

int m_1;                                                    // Minutes in timer
int s_1;                                                    // Tenth second value in timer
int s_2;                                                    // Total seconds value in timer
int total_time;                                             // Total time in timer
Ticker time_tick;                                           // Ticker for count down.

// ------------------------------------------- Keypad Presses Global Variables -------------------------------------------

int row = 0;                                                // Rows of the keypad, initialised to 0.
void key_numbers(void);                                     // Function that takes in the user input from the keypad.
void key_A(void);                                           // Function that accumulates the total time.
void time_decrement(void);                                  // Function that decrements the total time according to the ticker.


char key;                                                   // Stores the key pressed value.
int digit_i = 0;                                            // Counts how many digits have been pressed by the user on the keypad.
int count_D = 0;                                            // Counts how many times user pressed D.
int count_A = 0;                                            // Counts how many times user pressed A.
int count_B = 0;                                            // Counts how many times user pressed B.


CSE321_LCD lcd_disp(16, 2, LCD_5x8DOTS, PB_9, PB_8);            // Calling LCD Class

// ------------------------------------------- Main Function Begins -------------------------------------------

int main() {
  
  RCC->AHB2ENR |= (0x5);                                        // Enabling peripheral clock for ports A and C
  GPIOC-> MODER |= (0x0041);                                    // Configuring PC_0 and PC_3 as output mode for LEDs
  GPIOC -> MODER &=~(0x00BE);
  GPIOA -> MODER |= (0x40);                                     // Configuring PA_3 as output mode for LED
  GPIOA -> MODER &=~(0xB0);
  GPIOC->MODER |= (0x50000);                                    // Configuring PC_8 and PC_9 to output mode for keypad presses. 
  GPIOC->MODER &= ~(0xA0000);
  GPIOC->MODER |= (0x500000);                                   // Configuring PC_10 and PC_11 to output mode for keypad presses.
  GPIOC->MODER &= ~(0xA00000);
  
  col_1.rise(&isr_col1);                                        // When column 1 of the keypad has power, execute ISR
  col_2.rise(&isr_col2);                                        // When column 2 of the keypad has power, execute ISR
  col_3.rise(&isr_col3);                                        // When column 3 of the keypad has power, execute ISR
  col_4.rise(&isr_col4);                                        // When column 4 of the keypad has power, execute ISR

  while (1) {

    if (row == 1) {                                             // Sensing which button in the row was pressed by...
      GPIOC->ODR |= (0x100);                                    // Enabling GPIO port C pin 8.
      GPIOC->ODR &= ~(0x200);                                   // Disabling GPIO port C pin 9 for output.
      GPIOC->ODR &= ~(0XE00);                                   // Disabling GPIO port C pins 10 and 11.
      thread_sleep_for(50);
    } else if (row == 2) {
      GPIOC->ODR |= (0x200);                                    // Enabling GPIO port C pin 9.
      GPIOC->ODR &= ~(0x100);                                   // Disabling GPIO port C pin 8 for output.
      GPIOC->ODR &= ~(0xC00);                                   // Disabling GPIO port C pins 10 and 11.
      thread_sleep_for(50);
    } else if (row == 3) {
      GPIOC->ODR |= (0x400);                                    // Enable GPIO port C pin 10
      GPIOC->ODR &= ~(0x100);                                   // Disabling GPIO port C pin 8
      GPIOC->ODR &= ~(0xB00);                                   // Disabling GPIO port C pins 9 and 11.
      thread_sleep_for(50);
    } else {
      GPIOC->ODR |= (0x800);                                    // Enabling GPIO port C pin 11
      GPIOC->ODR &= ~(0x700);                                   // Disabling GPIO port C pins 8, 9 and 10.
      thread_sleep_for(50);
    }


    // ------------------------------------------ LCD Display Interaction ------------------------------------------

    lcd_disp.begin();                                           // Start LCD

    // User can only start inputting timer values when D is pressed...

    // ------------------------------------------ Key D Functionality ------------------------------------------

    if (count_D == 0 ) {                                         // If D is not pressed...
      lcd_disp.print("Press D to");                             
      lcd_disp.setCursor(0, 1);
      lcd_disp.print("input the time");
    } else {                                                    // If D is pressed...
      lcd_disp.clear();
      lcd_disp.print("Time Remaining:");
      lcd_disp.setCursor(0, 1);
      lcd_disp.print(fin);                                      // Display timer value contained in character array fin
    }
    // ------------------------------------------ Key A Functionality ------------------------------------------

    if (count_A == 1) {                                         // If A is pressed...
      time_tick.attach(&time_decrement, 1s);                    // TIcker should go to the function 'time_decrement' every 1 second.
      lcd_disp.clear();
      lcd_disp.print("Time Remaining:");
      lcd_disp.setCursor(0, 1);
      lcd_disp.print(count_down);                               // Display the count down on the LCD.
    }

    // ------------------------------------------ Timer runs out Functionality ------------------------------------------

    
    if (count_down[0] == '0') {                                 // If the timer has reached a value of 0:00...
      if (count_down[2] == '0') {
        if (count_down[3] == '0') {
          lcd_disp.clear();
          lcd_disp.print("Times Up!");                          // LCD displays 'Times Up!'
          GPIOC -> ODR |= (0x0009);                             // Multiple LEDs turn on.
          GPIOA -> ODR |= (0x0008);
        }
      }
    }

    // ------------------------------------------ Key B Functionality ------------------------------------------

    if (count_B == 1) {                                         // If B is pressed...
      lcd_disp.clear();
      lcd_disp.print("Timer Stopped.");                         // Stop timer.
      lcd_disp.setCursor(0, 1);
      lcd_disp.print("Restart program.");
    }


    row = row + 1;                                              // Increment row values for iteration.
    row %= 4;                                                   // Row values cannot exceed 4. 
  }

  return 0;                                                     // To avoid memory leakage. 
}

// ------------------------------------------- Main Function Ends -------------------------------------------


void key_numbers(void) {
  digit_i++;                                                    // If key_numbers is called, increase the digit value.
  if (digit_i < 4) {                                            // If digit value is less than 4...
    if (digit_i == 1) {
      fin[0] = key;                                             // Store the key in 'm' of 'm:ss' (fin character array)
    } else if (digit_i == 2) {
      if (key < '6') {
        fin[2] = key;                                           // Store the key in 's_1' of 'm:s_1s_2' (fin character array)
      }
    } else if (digit_i == 3) {
      fin[3] = key;                                             // Store the key in 's_2' of 'm:s_1s_2' (fin character array)
    }
  }
}

void key_A(void) {                                                              // Execute when key A is pressed...
  m_1 = fin[0] - '0';                                                           // Convert minute value from ASCII character to integer
  s_1 = (fin[2] - '0') * 10;                                                    // Convert second value from ASCII character to integer
  s_2 = (fin[3] - '0') + s_1;                                                   // Convert second value from ASCII character to integer
  m_1 = m_1 * 60;                                                               // Convert minute to seconds.
  total_time = m_1 + s_2;                                                       // Total time in timer.
}

void time_decrement(void){                                                      // Timer decrementing function...
  if (total_time > 0) {                                                         // As long as total time is greater than 0...
    total_time--;                                                               // Decrement total time by 1.
    count_down[0] = ((total_time - (total_time % 60)) / 60);                    // Taking the minute digit out    
    count_down[2] = ((total_time % 60) - (total_time % 60) % 10) / 10;          // Taking the first digit of the seconds value
    count_down[3] = (total_time % 60) % 10;                                     // Taking the last digit of the seconds value

    count_down[0] += '0';                                                       // Converting back to ASCII character
    count_down[2] += '0';                                                       // Converting back to ASCII character                                     
    count_down[3] += '0';                                                       // Converting back to ASCII character
  }
}

void isr_col1(void) {                                                           // The keys in column 1...
  if (row == 1) {                                                               // If row 1, column 1 then key 1
    key = '1';
    key_numbers();                                                              // Execute key_numbers
  } else if (row == 2) {                                                        // If row 2, column 1 then key 4
    key = '4';
    key_numbers();
  } else if (row == 3) {                                                        // If row 3, column 1 then key 7
    key = '7';
    key_numbers();
  }
}

void isr_col2(void) {                                                           // The keys in column 2...
  if (row == 1) {                                                               // If row 1, column 2 then key 2               
    key = '2';
    key_numbers();
  } else if (row == 2) {                                                        // If row 2, column 2 then key 5
    key = '5';
    key_numbers();
  } else if (row == 3) {                                                        // If row 3, column 2 then key 8
    key = '8';
    key_numbers();
  } else {                                                                      // If row 4, column 2 then key 0
    key = '0';
    key_numbers();
  }
}

void isr_col3(void) {                                                           // The keys in column 3...
  if (row == 1) {                                                               // If row 1, column 3 then key 3 
    key = '3';
    key_numbers();
  } else if (row == 2) {                                                        // If row 2, column 3 then key 6
    key = '6';
    key_numbers();
  } else if (row == 3) {                                                        // If row 3, column 3 then key 9
    key = '9';
    key_numbers();
  }
}

void isr_col4(void) {                                                           // The keys in column 4...
  if (row == 1) {                                                               // If row 1, column 4 then key A
    count_A++;                                                                  // Increase counter for A
    key = 'A';     
    key_A();
  } else if (row == 2) {                                                        // If row 2, column 4 then key B
    count_B++;                                                                  // Increase counter for B
    key = 'B';
  } else if (row == 3) {                                                        // If row 3, column 4 then key C
    key = 'C';
  } else {                                                                      // If row 4, column 4 then key D
    count_D++;                                                                  // Increase counter for D
    key = 'D';
  }
}