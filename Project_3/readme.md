-------------------
About
--------------------
- Read Me for Ultrasonic Room Capacity Reader
-------------------
Project Description
--------------------
- The Room Capacity Reader is a device that will deduct the capacity of the room by ultrasonic sensors stationed at the entrance of the room.
--------------------
Contributor List 
--------------------
- Bodhiswattwa Basu (author & sole contributor)
--------------------
Features 
--------------------
- LCD Screen
- Ultrasonic Transducer HC-SR04
- Vibration Motor
- 4x4 Keypad
- Nucleo L4R5ZI
--------------------
Required Materials
--------------------
- Nucleo L4R5ZI
- MBED Studio
- A solderless breadboard
- Male to Female Jumper Wires
- Resistors
- LCD
- Jumper wires
- 4x4 Keypad
- Vibration motor
- Ultrasonic transducer HC-SR04
--------------------
Resources and References
--------------------
 - STMicroelectronics, RM0432 Reference Manual, Rev. 7, August 2020. Available: https://www.st.com/resource/en/reference_manual/dm00310109-stm32l4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
 - STMicroelectronics, PM0214 Programming Manual, Rev. 10, March 2020. Available: https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortex-m4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf
 - LCD API/Library Instructions
 - EventQueue Documentation: https://os.mbed.com/docs/mbed-os/v6.15/apis/eventqueue.html
 - HC-SR04 Information: https://os.mbed.com/components/HC-SR04/
 - Watchdog documentation: https://os.mbed.com/cookbook/WatchDog-Timer
 - InterruptIn documentation: https://os.mbed.com/docs/mbed-os/v6.15/apis/interruptin.html
--------------------
Getting Started
--------------------
- Run the program.
- Press A on the 4x4 keypad to type in the room capacity.
- Type in room capacity. 
- Press B to start the room capacity reader. 
- Once room has reached capacity, vibration motor attached to the door handle will vibrate. LCD will also display 'Room Capacity Reached'
--------------------
Files
--------------------
- CSE321_project3_bodhiswa_main.cpp
- lcd1602.cpp
- lcd1602.h
--------------------
Global Variables Declared
----------
- state_flag
- on
- off
- flagDone
- flagA
- flagB
- flagC
- flag_2
- char key;
- capacity 
- update_cap
- row
- press_A
- press_B
- digits
- d1
- d2
- d3
- b1
- b2
- b3
- val
- timeout_wd
----------
API and Built In Elements Used
----------
- Interrupts
- Timer
- WatchDog API
- EventQueues
----------
Custom Functions
----------
- void count_B(void);
- void enter_cap(void); 
- void cap_val(void);
- void count_A(void);
- void state(void);
- void LCD_on(void);
- void LCD_ready(void);
- void LCD_start(void);
- void LCD_done(void);
- void done(void);
- void isr_col1(void);
- void isr_col2(void);
- void isr_col3(void);
- void isr_col4(void);
