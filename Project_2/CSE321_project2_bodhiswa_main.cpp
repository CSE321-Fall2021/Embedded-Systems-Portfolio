// CSE321 Project 2: Part 2: Designing an all-in-one count-down alarm system, utilizing bare metal methodology,
// Author: Bodhiswattwa Basu
// Date: 15 Oct, Fall 2021
// Course: Real-Time Embedded Operating Systems, CSE 321
// Function(s): 
//      1. main()
//      2. isr_3
//      3. isr_A
//      4. isr_B
//      5. isr_6
// Description: 
//      1. Main function controls the functionality of the program. 
//      2. ISR functions indicate when the appropriate key has been pressed on the serial monitor. 
// Parameters:
//      1. No parameters
// Input (Peripherals):
//      1. Solderless breadboard
//      2. 4x LEDs
//      3. Jumper wires
//      4. LCD
//      5. 4x4 Keypad
//      6. Nucleo
//      7. Resistors
// Output (Peripherals):
//      1. LEDs
//      2. LCD
// Sources/References Used: 
//      1. STMicroelectronics, RM0432 Reference Manual, Rev. 7, August 2020. Available: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
//      2. STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020. Available: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf


#include "mbed.h"
#include "mbed_thread.h"
#include <cstdio>

void isr_A(void);                                       // Need to be able to detect A being pressed. (to start timer)
void isr_3(void);                                       // Need to be able to detect 3 being pressed. (to start timer)
void isr_6(void);                                       // Need to be able to detect 6 being pressed. (to start timer)
void isr_B(void);                                       // Need to be able to detect B being pressed. (to start timer)

InterruptIn col_3(PB_8);                                // Detect interrupt when PB_8 is ACTIVE.
InterruptIn col_4(PB_9);                                // Detect interrupt when PB_9 is ACTIVE. 

int row = 0;                                            // Initialising variable row to iterate through matrix.

int main(){
    RCC -> AHB2ENR |= 0x4;                              // Enabling the peripheral clock for port C (row supply lines)
    GPIOC -> MODER |= (0x50000);                        // Configuring PC_8 and PC_9 to output mode.
    GPIOC -> MODER &=~ (0xA0000);
    col_3.rise(&isr_3);                                 // isr_3 will be executed when col_3 (PB_8) changes to 1.
    col_4.rise(&isr_A);                                 // isr_A will be executed when col_4 (PB_9) changes to 1.
    while(1){
        if(row == 1){
            GPIOC -> ODR |= (0x100);                    // Writing output to GPIO port C pin 8 (turning it on)
            GPIOC -> ODR &=~ (0x200);                    // Disabling GPIO port C pin 9 for output.
        } else {
            GPIOC -> ODR |= (0x200);                    // Writing output to GPIO port C pin 9 (turning it on)
            GPIOC -> ODR &=~ (0x100);                   // Disabling GPIO port C pin 8 for output. 
        }
    row = row + 1;                                      // Incrementing the row values. 
    row %=2;
    }
    return 0;
}

void isr_3(void){                                       // If row == 1, then we have found 3
    if(row == 1){ 
        printf("found 3\n");
        wait_us(5000);                                  // Delay of 5000 microseconds.
    } else {                                            // Else, we found 6.
        printf("found 6\n");
        wait_us(5000);                                  // Delay of 5000 microseconds.
    }
}

void isr_A(void){ 
    // or B
    if(row == 1){                                      // If row == 1, then we found B
        printf("found B\n");
        wait_us(5000);                                 // Delay of 5000 microseconds.
    } else {                                           // Else, we have found A
        printf("found A\n");
        wait_us(5000);                                 // Delay of 5000 microseconds.
    }
}

void isr_6(void){ printf("found 6\n");}                 // ISR for 6

void isr_B(void){ printf("found B\n");}                 // ISR for B