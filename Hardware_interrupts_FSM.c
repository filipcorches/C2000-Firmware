#include "device.h"
#include "driverlib.h"
#include <stdint.h>
#include <stdbool.h>

volatile bool is_paused = false;

void Hardware_Init(void);
void Task_Read_Button(void);
__interrupt void cpu_timer0_isr(void); 


int main(void) 
{
    Hardware_Init();
    
    while(1) 
    {
        Task_Read_Button();
    }
}

void Hardware_Init(void) 
{
    // 1. Core Device Setup
    Device_init();
    Device_initGPIO();
    Interrupt_initModule();
    Interrupt_initVectorTable();

    // 2. Configure Pins
    GPIO_setDirectionMode(31, GPIO_DIR_MODE_OUT); // LED
    GPIO_setPadConfig(31, GPIO_PIN_TYPE_STD);
    GPIO_writePin(31, 1); // Turn LED OFF

    GPIO_setDirectionMode(14, GPIO_DIR_MODE_IN);  // Button
    GPIO_setPadConfig(14, GPIO_PIN_TYPE_PULLUP);

    // 3. Configure Hardware Timer 0 Interrupt
    Interrupt_register(INT_TIMER0, &cpu_timer0_isr);
    
    // 200MHz clock. 100,000,000 ticks = 0.5 seconds.
    CPUTimer_setPeriod(CPUTIMER0_BASE, 100000000); 
    CPUTimer_enableInterrupt(CPUTIMER0_BASE);
    Interrupt_enable(INT_TIMER0);
    CPUTimer_startTimer(CPUTIMER0_BASE);

    // 4. Enable Global Interrupts
    EINT;
    ERTM;
}

void Task_Read_Button(void) 
{
    static uint16_t previous_button = 1;
    uint16_t current_button = GPIO_readPin(14);
    
    if (current_button == 1 && previous_button == 0) 
    {
        is_paused = !is_paused;
    }
    previous_button = current_button;
}

// Hardware timer interrupt: Runs every 0.5 seconds
__interrupt void cpu_timer0_isr(void) 
{
    typedef enum { STATE_LED_OFF, STATE_LED_ON } BlinkyState_t;
    static BlinkyState_t currentState = STATE_LED_OFF;

    if (is_paused == false) 
    {
        switch(currentState) 
        {
            case STATE_LED_OFF:
                GPIO_writePin(31, 0); // Turn ON (Active-Low)
                currentState = STATE_LED_ON;
                break;
                
            case STATE_LED_ON:
                GPIO_writePin(31, 1); // Turn OFF
                currentState = STATE_LED_OFF;
                break;
        }
    }
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP1);
}