/** CSE321 Project 2:
The Room Capacity Reader is a device that will deduct the capacity of the room by ultrasonic sensors stationed at the entrance of the room. 
The LCD will display a counter of the people who have walked in the room. As soon as the room reaches the desired capacity, 
which can be set by the administrator of the room, a vibration motor attached to the door handle will vibrate 
and the LCD will display that the room has reached capacity for social distancing purposes.

Author: Bodhiswattwa Basu 
Date: 10th December, 2021
Fall 2021 Course: Real-Time Embedded Operating Systems, CSE 321 
Function(s):
      1. main
      2. ISRs: isr_col1, isr_col2, isr_col3, isr_col4
      3. LCD_on
      4. LCD_ready
      5. LCD_start
      6. state
      7. enter_cap
      8. cap_val
      9. count_A
      10. count_B
Description:
      1. Main function controls the functionality of the program by providing power to the columns of the keypad, by printing to the LCD and triggering the ultrasonic sensor.
      2. ISR functions indicate when the appropriate key has been pressed on
      the serial monitor.
      3. LCD_on is the first state of the LCD when the user has just started the room capacity reader. 
      4. LCD_ready is a state that would display once user presses 'A' and then displays what the user typed in
      5. LCD_start is a state that decrements room capacity counter and sets flag to indicate room capacity reached.
      6. state is a function that determines the state flags and sets them according to other flags.
      7. enter_cap is a function that sets the digits in place on the LCD as user enters them.
      8. cap_val is a function that aggregates room capacity value.
      9. count_A counts if A is pressed and set the flag.
      10. count_B counts if B is pressed and set the flag.
Input (Peripherals):
      1. Ultrasonic transducer (PB10 & PB11)
      2. 4x4 Keypad
Breadboard & Nucleo Interfacing: 
      1. Row 1 == PC_8              5. Column 1 == PC_12
      2. Row 2 == PC_9              6. Column 2 == PD_2
      3. Row 3 == PC_10             7. Column 3 == PF_3
      4. Row 4 == PC_11             8. Column 4 == PF_5
Output (Peripherals):
      1. Vibration Motor (PA_5)
      2. LCD (PB_8 & PB_9)
Sources/References Used:
      1. STMicroelectronics, RM0432 Reference Manual, Rev. 7, August 2020.
      Available: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
      2. STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020.
      Available: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
      3. LCD API/Library Instructions
      4. EventQueue Documentation: https://os.mbed.com/docs/mbed-os/v6.15/apis/eventqueue.html
      5. HC-SR04 Information: https://os.mbed.com/components/HC-SR04/
      6. Watchdog documentation: https://os.mbed.com/cookbook/WatchDog-Timer
      7. InterruptIn documentation: https://os.mbed.com/docs/mbed-os/v6.15/apis/interruptin.html
**/

#include "Callback.h"
#include "DigitalIn.h"
#include "EventQueue.h"
#include "PinNames.h"
#include "ThisThread.h"
#include "mbed.h"
#include "PinNamesTypes.h"
#include "lcd1602.h"
#include "Thread.h"
#include "mbed_thread.h"
#include <cstdint>

// ----------------------------------------------- Events and Threads --------------------------------------------

EventQueue testing(32* EVENTS_EVENT_SIZE);
EventQueue test_wd(32* EVENTS_EVENT_SIZE);
EventQueue buzz(32* EVENTS_EVENT_SIZE);
Thread buzzing;
Thread testwd;
Thread testing1;

// ------------------------------ Input (ultrasonic transducer)  Peripheral Set-Up --------------------------------

DigitalOut trigger(PB_10);
DigitalIn echo(PB_11);

// ---------------------------------------- 'Sonar' Timer Set-Up -------------------------------------------------
Timer radar;
Ticker t1;

// ----------------------------------------- Watchdog Timer Set-Up -------------------------------------------------

int timeout_wd = 32670;
InterruptIn watch_dog(BUTTON1);

// ----------------------------------------- Capacity Counting Global Variables --------------------------------------
int digits = 0;
int d1;
int d2;
int d3;
int b1;
int b2;
int b3;
int val;

// ---------------------------------------------- Keypad Global Variables --------------------------------------------
char key;
char capacity[4] = "000";
char update_cap[5] = "000\0";
int row = 0;
int press_A = 0;
int press_B = 0;
void count_B(void);
void enter_cap(void); 
void cap_val(void);
void count_A(void);


// ---------------------------------------------- State of Affairs Global Variables --------------------------------------------

int state_flag = 0;
int on = 1;
int off = 0;
int flagDone = 0;
int flagA = 0;
int flagB = 0;
int flagC = 0;
int flag_2 = 0;
void state(void);
void LCD_on(void);
void LCD_ready(void);
void LCD_start(void);
void LCD_done(void);
void done(void);

// ---------------------------------------------- Sonar distance Global Variables --------------------------------------------

int dist1;

// ---------------------------------------------------------------------------------------------------------------------------

CSE321_LCD lcd_disp(16, 2, LCD_5x8DOTS, PB_9, PB_8);            // Calling LCD Class

// ------------------------------------------- Interrupts and ISRs -------------------------------------------

void isr_col1(void);                                        // When column 1 rises
void isr_col2(void);                                        // When column 2 rises 
void isr_col3(void);                                        // When column 3 rises
void isr_col4(void);                                        // When column 4 rises

InterruptIn col_1(PC_12, PullDown);                         // Detect interrupt when PC_12 is ACTIVE.
InterruptIn col_2(PD_2, PullDown);                          // Detect interrupt when PD_2 is ACTIVE.
InterruptIn col_3(PF_3, PullDown);                          // Detect interrupt when PF_3 is ACTIVE.
InterruptIn col_4(PF_5, PullDown);                          // Detect interrupt when PF_5 is ACTIVE.


int main(){
    RCC -> AHB2ENR |= (0x0000002D);                         // Enabling peripheral clock for ports C, D, A and F
    GPIOC -> MODER |= (0x550000);                           // Configuring port C pin 8, 9, 10 and 11 for general purpose output mode.
    GPIOC -> MODER &=~ (0xAA0000);
    GPIOA -> MODER |= (0x00000400);                         // Configuring port A pin 5 for general purpose output mode
    GPIOA -> MODER &=~ (0x00000B00);
    
    Watchdog &watchdog = Watchdog::get_instance();          // Watchdog setup
    watchdog.start(timeout_wd);                             // Watchdog timeout begins for 30 seconds
   

    testing1.start(callback(&testing, &EventQueue::dispatch_forever));          // Thread starts


    col_1.rise(&isr_col1);                                   // Interrupt in column 1
    col_2.rise(&isr_col2);                                   // Interrupt in column 2
    col_3.rise(&isr_col3);                                   // Interrupt in column 3
    col_4.rise(&isr_col4);                                   // Interrupt in column 4
    lcd_disp.begin();

    // User has to press A to input room capacity
    // sonar functionality
    radar.reset();
    radar.start();
    while(1){
        Watchdog::get_instance().kick();
        if(row == 1){
           GPIOC->ODR |= (0x100);                                    // Enabling GPIO port C pin 8.
           GPIOC->ODR &= ~(0x200);                                   // Disabling GPIO port C pin 9 for output.
           GPIOC->ODR &= ~(0XE00);                                   // Disabling GPIO port C pins 10 and 11.
           thread_sleep_for(50);                                      // Addressing bounce  
        } else if(row == 2){
            GPIOC->ODR |= (0x200);                                    // Enabling GPIO port C pin 9.
            GPIOC->ODR &= ~(0x100);                                   // Disabling GPIO port C pin 8 for output.
            GPIOC->ODR &= ~(0xC00);                                   // Disabling GPIO port C pins 10 and 11.
            thread_sleep_for(50);
        } else if(row == 3){
            GPIOC->ODR |= (0x400);                                    // Enable GPIO port C pin 10
            GPIOC->ODR &= ~(0x100);                                   // Disabling GPIO port C pin 8
            GPIOC->ODR &= ~(0xB00);                                   // Disabling GPIO port C pins 9 and 11.
            thread_sleep_for(50);
        } else {
            GPIOC->ODR |= (0x800);                                    // Enabling GPIO port C pin 11
            GPIOC->ODR &= ~(0x700);                                   // Disabling GPIO port C pins 8, 9 and 10.
            thread_sleep_for(50);
        }
        if(state_flag == 0){                                          // If state = initial, then call LCD_on
            LCD_on();
            thread_sleep_for(500);                                    
        }else if(state_flag == 1){                                    // If state = ready, then call LCD_ready
            LCD_ready();
            thread_sleep_for(500);
        }else if(state_flag == 2){                                    // If state = start, then begin ultrasonic transducer
            trigger = 1;                                              // Trigger ultrasonic transducer
            radar.reset();                                            
            wait_us(10.0);                                            // Wait for 10 microseconds
            trigger = 0;
            while(echo == 0){};                                       
            radar.start();                                            // Start the sonar timer
            while(echo == 1){};
            radar.stop();                                             // Stop the sonar timer
            dist1 = radar.read_ms();                                  // Determine the sonar time
            wait_us(2.0);
            if(dist1 != 69){                                          // If the distance to the person is not 69, then a person walks in
                LCD_start();                                          // LCD_start
            }
            //printf("time: %d\n", time1);
            thread_sleep_for(500);
        }
        row = row + 1;                                              // Increment row values for iteration.
        row %= 4;                                                   // Row values cannot exceed 4. 
    }
    
    //printf("cap: %s", capacity);
    return 0;                                                
}


// State 0 -- State that would display when the Room Capacity Reader turns on
void LCD_on(){
    lcd_disp.clear();
    lcd_disp.setCursor(0,0);
    lcd_disp.print("Press A to enter");
    lcd_disp.setCursor(0, 1);
    lcd_disp.print("room capacity");
}

// State 1 -- State that would display once user presses 'A' and then displays what the user typed in
void LCD_ready(){
    lcd_disp.clear();
    lcd_disp.setCursor(0,0);
    lcd_disp.print("Room Capacity: ");
    lcd_disp.setCursor(0,1);
    lcd_disp.print(capacity);
}


// Function that aggregates room capacity value 
void cap_val(){
    d1 = (capacity[0] - '0') * 100;
    d2 = (capacity[1] - '0') * 10;
    d3 = capacity[2] - '0';
    val = d1 + d2 + d3;
    done();
}

// State 2 -- Function that decrements room capacity counter and sets flag to indicate room capacity reached
void LCD_start(){
    val--;
    if(val > 0){
        b1 = (val/100);                                             // Sets hundredths value
        b2 = (val - (b1*100))/10;                                   // Sets tenths value
        b3 = (val - ((b1*100)+(b2*10)));                            // Sets ones value
        update_cap[0] = b1 + '0';                                   // ASCII version
        update_cap[1] = b2 + '0';
        update_cap[2] = b3 + '0';         
        done();
        lcd_disp.clear();
        lcd_disp.setCursor(0,0);
        lcd_disp.print("Room Capacity: ");
        lcd_disp.setCursor(0,1);
        lcd_disp.print(update_cap);
    } if(val == 0){
        GPIOA -> ODR |= (0x0020);                                   // Output vibration motor
        wait_us(50);                                                // Wait
        GPIOA -> ODR &=~ (0x00D0);                                  // Output vibration motor
        wait_us(50);
        update_cap[0] = '0';
        update_cap[1] = '0';
        update_cap[2] = '0';
        lcd_disp.clear();
        lcd_disp.print("Room Capacity");                            // LCD commands to print room capacity reached
        lcd_disp.setCursor(0,1);
        lcd_disp.print("Reached!");
        state_flag = 3;                                             // Set state flag
        state();
    }
}

// Determines the state flags and sets them according to other flags
void state(){
    if(flagA == 1 && flagC == 0){                                             // If A has been pressed, state = 1 = LCD_ready()
        state_flag = 1;
    } else if(flagB == 1 && flagC == 1){                                      // If capacity is filled + B has been pressed, state = 2 = LCD_start()
        state_flag = 2;
    } else if(flagDone == 1 && flag_2 == 1){                                  // If update cap is == '000', flagDone = 1, state = LCD_done()
        state_flag = 3;
    }
}

// Counts if A is pressed and set the flag
void count_A(){
    press_A++;
    if(press_A >= 1){
        flagA = 1;
    } else{
        flagA = 0;
    } 
    flagC = 0;
    state();
}

// Counts if B is pressed and set the flag
void count_B(){
    press_B++;
    if(press_B >= 1){
        flagB = 1;
    } else{
        flagB = 0;
    }
    flagC = 1;
    state();
}

// Function that sets the digits in place on the LCD as user enters them
void enter_cap(void){
    digits++;
    if(digits == 1){
        capacity[0] = key;
    } else if(digits == 2){
        capacity[1]= key;
    } else if (digits == 3){
        capacity[2] = key;
    }
    cap_val();
    done();
}



// ISR for column 1 interrupt of keypad matrix
void isr_col1(void){
    if(row == 1){
        key = '1';
        enter_cap();
    } else if(row == 2){
        key = '4';
        enter_cap();
    } else if(row == 3){
        key = '7';
        enter_cap();
    } else if(row == 4){
        key = '*';
    }
}

// ISR for column 2 interrupt of keypad matrix
void isr_col2(void){
    if(row == 1){
        key = '2';
        enter_cap();
    } else if(row == 2){
        key = '5';
        enter_cap();
    } else if(row == 3){
        key = '8';
        enter_cap();
    } else if(row == 4){
        key = '0';
        enter_cap();
    }
}

// ISR for column 3 interrupt of keypad matrix
void isr_col3(void){
    if(row == 1){
        key = '3';
        enter_cap();
    } else if(row == 2){
        key = '6';
        enter_cap();
    } else if(row == 3){
        key = '9';
        enter_cap();
    } else if(row == 4){
        key = '#';
    }
}

// ISR for column 4 interrupt of keypad matrix
void isr_col4(void){
    if(row == 1){
        key = 'A';
        count_A();
    } else if(row == 2){
        key = 'B';
        count_B();
    } else if(row == 3){
        key = 'C';
    } else if(row == 4){
        key = 'D';
    }
}
