// Project Description: This program creates a thread to drive an LED to have an on time of 500 ms and an off time of 2000ms
// Author: Bodhiswattwa Basu (bodhiswa@buffalo.edu)
// Date: October 4, 2021

#include "mbed.h"

Thread controller;              // Allows controller variable to be configured with scheduling and prority management. 
void handler();                 // Calling function for handler.

DigitalOut blue_LED(LED2);      // Establishing blue LED as an output
int trigger1=0;                  // Trigger 1 variable
InterruptIn button1(BUTTON1);   // InterruptIn will change when BUTTON1 changes. 
void rising_e();                // Function rising_e for rising edge. 
int trigger2 = 0;                 // Trigger 2 variable
void falling_e();               // Function Bfalling_e for falling edge. 


int main() {
  // Beginning of thread execution.
    printf("----------------START----------------\n");
	printf("Starting state of thread: %d\n", controller.get_state());                       // Initial state of thread. 
    controller.start(handler);                                                                // Beginning the thread
    printf("State of thread right after start: %d\n", controller.get_state());      // State of thread as it begins. 
    button1.rise(rising_e);                                                                   // rising_e function assigned to rising edge.                      
    button1.fall(falling_e);                                                                // falling_e function assigned to falling edge. 
    return 0;
}

// The code for the handler goes below: 

void handler() {
    while (true) {
        if(trigger1==0){
            blue_LED = !blue_LED;
            printf("LED is turned off.");                                        // If trigger1 is 0, the LED is turned off. Print statement as indicator.
            thread_sleep_for(2000);                                              // Thread_sleep is a time delay function, causes a 2000 unit delay
            blue_LED = !blue_LED;                                                // Check if LED is turned off again after delay. 
            printf("LED is turned off.");                                        // Print statement as indicator. 
            thread_sleep_for(500);                                               //Thread_sleep is a time delay function, causes a 500 unit delay
        }
    }
}

void rising_e() {
	while(trigger2 == 0){                                             // Toggling the state of the thread. 
      trigger1++;
      trigger1 %= 2;
    }  
    if(trigger1 == 1){                                                 //Flag is set. 
        trigger2=1;                                                   // Reset value of trigger2 to 1.											  
    }
}

void falling_e() {                                          
  if (trigger2 == 1){                                                  // Checking if the value in trigger 2 == 1
    trigger1++;                                                        // If so, increment value of trigger1 by 1.
    trigger1 %= 2; 	                                
    trigger2 = 0;                                                      // Reset value of trigger2 to 0.
    }
}
