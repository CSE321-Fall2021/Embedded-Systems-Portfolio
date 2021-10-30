-------------------
About: ReadMe for Count-Down Alarm System
-------------------
Project Description: This is an all-in-one count-down alarm system that a user can input timer values into, start and stop the timer. 
--------------------
Contributor List: Bodhiswattwa Basu (author & sole contributor)
--------------------
Features: LEDs that light up when digits are inputted, all LEDs will light up when timer ends. LCD screen will display timer value, and 'Times Up' once time is reached.  
--------------------
Required Materials
--------------------
- Nucleo L4R5ZI
- MBED Studio
- A solderless breadboard
- LEDs
- Resistors
- LCD
- Jumper wires
--------------------
Resources and References
--------------------
GPIO Reference Sheet: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf 
Thread Documentation: https://os.mbed.com/docs/mbed-os/v6.13/apis/thread.html
InterruptIn Documentation: https://os.mbed.com/handbook/InterruptIn
--------------------
Getting Started
--------------------
To get started with the program, select 'D' to input time. Use the keypad to enter in a timer value of less than 9mins and 59 seconds. Start the timer by pressing 'A', you will be able to see the timer value on the LCD. If at any point, you want to turn the timer off, select 'B'. Once the timer has run out, all of the LEDs will light up and the LCD will display 'Times Up'. 
--------------------
CSE321_project1_bodhiswa_corrected_code.cpp:
-------------------- 
Things Declared
----------
#include mbed.h
Thread
DigitalOut
InterruptIn
#include "mbed_thread.h"
#include <cstdio>
----------
API and Built In Elements Used
----------
InterruptIn API
printf
----------
Custom Functions
----------
main: 
    GPIO and peripheral clock enabler. 
    Keypad configurations. 
isr_3: 
    Function to indicate 3 being pressed on keyboard.
isr_A: 
    Function to indicate A being pressed on keyboard.
isr_6: 
    Function to indicate 6 being pressed on keyboard.
isr_B: 
    Function to indicate B being pressed on keyboard.