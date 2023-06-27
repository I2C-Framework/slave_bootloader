#include "mbed.h"
#include <cstdint>

#if !defined(POST_APPLICATION_ADDR)
#error "target.restrict_size must be set for your target in mbed_app.json"
#endif


#define HEADER_ADDRESS_0 0x08004000
#define APPLICATION_ADDRESS_0 0x08004400
#define HEADER_ADDRESS_1 0x08014000
#define APPLICATION_ADDRESS_1 0x08014400

typedef void (*p_function)(void);

typedef struct {
    uint32_t magic;
    uint16_t major_version;
    uint16_t minor_version;
    uint16_t fix_version;
}__attribute__((__packed__)) app_metadata_t;

uint32_t jump_address;
p_function jump_to_application;

int main()
{
    printf("Launching the bootloader\r\n");
    
    
    app_metadata_t *active_app_metadata = (app_metadata_t*) HEADER_ADDRESS_1;
    uint32_t application_address = APPLICATION_ADDRESS_1;
    
    if(active_app_metadata->magic != 0xdeadbeef) {
        printf("Wrong magic number, %x\r\n", active_app_metadata->magic);
        active_app_metadata = (app_metadata_t*) HEADER_ADDRESS_0;
        application_address = APPLICATION_ADDRESS_0;
    }

    printf("About to start the application at 0x%x\r\n\n", application_address);
    
    SCB->VTOR
    mbed_start_application(application_address);

/*
    // Disable interrupts
    __disable_irq();



    SysTick->CTRL = 0x00000000;
    SCB->VTOR 
    //powerdown_nvic();
    //powerdown_scb(application_address);
    mbed_mpu_manager_deinit();
    // Get the address of the applications
     jump_address = *(__IO uint32_t*) (application_address+4);
    // // Create a function pointer to our application
    // jump_to_application = (p_function) jump_address;
    // // Set our vector table to the applications vector table
    // __set_MSP(*(__IO uint32_t*) application_address);
    // // Jump to our application
    // jump_to_application();
    // // Because bootloader
    // __enable_irq();
    // printf("Error\n");

    __asm volatile(
    "movs   r2, #0      \n"
    "msr    control, r2 \n" // Switch to main stack
    "mov    sp, %0      \n"
    "msr    primask, r2 \n" // Enable interrupts
    "bx     %1          \n"
    :
    : "l"(application_address), "l"(jump_address)
    : "r2", "cc", "memory"
    );
    */
}
