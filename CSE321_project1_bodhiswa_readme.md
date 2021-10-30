-------------------
About: Program drives an LED. 
-------------------
Project Description: This program creates a thread to drive an LED to have an on time of 500 ms and an off time of 2000ms.
Contributor List: Bodhiswattwa Basu (author & sole contributor)
--------------------
Features: Operates an LED with an on-time of 500ms and off-time of 2000ms. 
--------------------
Required Materials
--------------------
-Nucleo L4R5ZI
- MBED Studio
--------------------
Resources and References
--------------------
Thread Documentation: https://os.mbed.com/docs/mbed-os/v6.13/apis/thread.html
InterruptIn Documentation: https://os.mbed.com/handbook/InterruptIn
--------------------
Getting Started
--------------------
To get started with the program, please connect your Nucleo to your computer. And then run the program. 
--------------------
CSE321_project1_bodhiswa_corrected_code.cpp:
-------------------- 
Things Declared
----------
#include mbed.h
Thread
DigitalOut
InterruptIn
.start
.rise
.fall
.get_state
thread_sleep_for(500)
thread_sleep_for(2000)
printf
----------
API and Built In Elements Used
----------
InterruptIn API
printf
----------
Custom Functions
----------
main:
	Begins thread exception.  
	Inputs: None
	Globally referenced variables used:
	trigger1 and trigger2, printf

handler:
	Handler handling. 
	Inputs: None. 
	Globally referenced variables used: 
	trigger1, trigger2, blue_LED, printf, thread_sleep_for

rising_e: 
	Rising edge function for button. 
	Inputs: None. 
	Globally referenced variables used: 
	trigger1, trigger2

falling_e: 
	Falling edge function for button. 
	Inputs: None. 
	Globally referenced variables used: 
	trigger1, trigger2
