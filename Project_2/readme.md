-------------------
About:
--------------------
Read Me for Count-Down Alarm System
-------------------
Project Description
--------------------
This is an all-in-one count-down alarm system that a user can input timer values into, start and stop the timer. 
--------------------
Contributor List 
--------------------
Bodhiswattwa Basu (author & sole contributor)
--------------------
Features 
--------------------
- LCD Screen
- 4x4 Keypad
- Nucleo L4R5ZI
--------------------
Required Materials
--------------------
- Nucleo L4R5ZI
- MBED Studio
- A solderless breadboard
- 8 x Male to Female Jumper Wires
- 7 x LEDs
- 7 x Resistors
- LCD
- Jumper wires
- 4x4 Keypad
--------------------
Resources and References
--------------------
- GPIO Reference Sheet: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
- STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf 
- InterruptIn Documentation: https://os.mbed.com/handbook/InterruptIn
- Ticker Documentation: https://os.mbed.com/handbook/Ticker
--------------------
Getting Started
--------------------
Step 1: To get started with the program, select 'D' to input time. 
Step 2: Use the keypad to enter in a timer value of less than 9 mins and 59 seconds. Everytime a key is pressed, an LED at the top of the breadboard will light up. 
Step 3: Start the timer by pressing 'A', you will be able to see the timer value on the LCD.
Step 4: If at any point, you want to turn the timer off, select 'B'. 
Step 5: Once the timer has run out, the three LEDs at the bottom of the breadboard will light up and the LCD will display 'Times Up'. 
--------------------
Files
--------------------
- CSE321_project2_bodhiswa_LCD1602.cpp
- CSE321_project2_bodhiswa_LCD1602.h
- CSE321_project2_bodhiswa_main.cpp
----------
Things Declared
----------
- #include "PinNamesTypes.h"
- #include "Thread.h"
- #include "lcd1602.h"
- #include "mbed.h"
- #include "mbed_events.h"
- #include "stm32l4r5xx.h"
----------
Global Variables Declared
----------
- m_1
- s_1
- s_2
- total_time
- row
- key
- digit_i
- count_D
- count_A
- count_B
----------
API and Built In Elements Used
----------
InterruptIn
Ticker
----------
Custom Functions
----------
main: 
    GPIO and peripheral clock enabler. 
    Keypad configurations.
    LCD display commands. 
isr_col1: 
    Function to indicate 1/4/7 being pressed on keyboard.
isr_col2: 
    Function to indicate 2/5/8/0 being pressed on keyboard.
isr_col3: 
    Function to indicate 3/6/9 being pressed on keyboard.
isr_col4: 
    Function to indicate A/B/C/D being pressed on keyboard.
key_numbers:
    Function that takes in the user input of the timer value.
key_A:
    Function takes the user input of the timer value and aggregates a total time value.
timer_decrement:
    Function that the ticker visits every second to decrease the value of the timer. 