#include "mbed.h"
#include "FlashIAP.h"
#include "ResetReason.h"
#include <cstdint>

// Flash Addresses
#define FIRMWARE_HEADER_ADDRESS (0x08009800)
#define FIRMWARE_APPLICATION_ADDRESS (0x08009C00)
#define APPLICATION_METADATA_ADDRESS (0x08009000)
#define END_MCU_ADDRESS (0x0801FFFF)
#define UNIQUE_ID_ADDR (0x1FFF7590)

// Values
#define I2C_FREQ (100000)
#define FIRMWARE_SIZE (0xF000)
#define BUFFER_DATASIZE (1024)
#define BUFFER_SIZE (BUFFER_DATASIZE + 1 + 1) // Buffer + block size + num block
#define MAGIC_FIRMWARE_VALID (0xDEADBEEF)
#define MAGIC_FIRMWARE_NEED_UPDATE (0xDEADBEEF)
#define MAGIC_FIRMWARE_NO_NEED_UPDATE (0xFFFFFFFF)

// Struct for firmware header
struct app_header_t {
    uint32_t magic;
    uint64_t firmware_size;
    uint32_t firmware_crc;
    uint8_t major_version;
    uint8_t minor_version;
    uint8_t fix_version;
}__attribute__((__packed__));

// Struct for bootloader metadata
struct app_metadata_t{
    uint32_t magic_firmware_need_update;
    uint8_t group;
    char sensor_type[30];
    char name[30];
};

// Boot on firmware
void start_firmware();
// Check if firmware need to be updated
int need_update_firmware();
// Set new metadata in flash
void set_new_metadata(app_metadata_t *metadata_ram);
// Init I2C for firmware update
void init_i2c(I2CSlave *slave);
// Wait for firmware update
void wait_for_update_firmware(I2CSlave *slave);
// Check if magic number of firmware is valid
int is_magic_valid();
// Calculate CRC of firmware and compare it with the one in the header
int is_crc_valid();

DigitalOut led(LED_STATUS);
app_metadata_t *metadata_flash = (app_metadata_t*) APPLICATION_METADATA_ADDRESS;
app_metadata_t metadata_ram = *((app_metadata_t*) metadata_flash);

int main()
{
    printf("Launching the bootloader\r\n");

    led = 1;

    if(need_update_firmware()){// || reset_reason == RESET_REASON_PIN_RESET){

        
        I2CSlave slave(I2C_FRAMEWORK_SDA, I2C_FRAMEWORK_SCL);
        init_i2c(&slave);

        printf("Bootloader ready for firmware update\r\n");

        wait_for_update_firmware(&slave);
   
    }

    if(!is_magic_valid()){
        printf("Magic number invalid\r\n");
        metadata_ram.magic_firmware_need_update = MAGIC_FIRMWARE_NEED_UPDATE;
        set_new_metadata(&metadata_ram);
        NVIC_SystemReset();
    }

    if(!is_crc_valid()){
        printf("CRC invalid\r\n");
        metadata_ram.magic_firmware_need_update = MAGIC_FIRMWARE_NEED_UPDATE;
        set_new_metadata(&metadata_ram);
        NVIC_SystemReset();
    }

    metadata_ram.magic_firmware_need_update = MAGIC_FIRMWARE_NO_NEED_UPDATE;
    set_new_metadata(&metadata_ram);

    led = 0;

    start_firmware();
}

int is_magic_valid(){
    app_header_t *normal_firmware_header = (app_header_t*) FIRMWARE_HEADER_ADDRESS;

    if(normal_firmware_header->magic != MAGIC_FIRMWARE_VALID){
        return 0;
    } else {
        return 1;
    }
}

int is_crc_valid(){
    app_header_t *normal_firmware_header = (app_header_t*) FIRMWARE_HEADER_ADDRESS;
    MbedCRC<POLY_32BIT_ANSI, 32> crc32;
    uint32_t crc;

    crc32.compute((uint8_t*)FIRMWARE_APPLICATION_ADDRESS, normal_firmware_header->firmware_size, &crc);
    if(crc != normal_firmware_header->firmware_crc) {
        return 0;
    } else {
        return 1;
    }
}

void wait_for_update_firmware(I2CSlave *slave){
    int rc;
    char firmware_part_buffer[BUFFER_SIZE];
    FlashIAP flash;
    flash.init();
    uint8_t response = 0x55;

    led = 1;

    while(1){
        int i = slave->receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                // Need for i2cdetect for specific range
                rc = slave->write(response);
                break;
            case I2CSlave::WriteGeneral:
                break;
            case I2CSlave::WriteAddressed:
                rc = slave->read(firmware_part_buffer, BUFFER_SIZE);

                printf("Programming firmware part %d/%d\r\n", firmware_part_buffer[0], firmware_part_buffer[1]);
                
                // If first part, erase all firmware
                if(firmware_part_buffer[0] == 1){
                    printf("Erasing...\n");
                    rc = flash.erase(FIRMWARE_HEADER_ADDRESS, firmware_part_buffer[1] * BUFFER_DATASIZE);
                    if(rc != 0){
                        printf("Error on erase : %d\r\n", rc);
                        NVIC_SystemReset();
                    }
                }

                rc = flash.program(firmware_part_buffer + 2, FIRMWARE_HEADER_ADDRESS + (firmware_part_buffer[0] - 1) * BUFFER_DATASIZE, BUFFER_DATASIZE);
                if(rc != 0){
                    printf("Error on flash : %d\r\n", rc);
                    NVIC_SystemReset();
                }
                
                if(firmware_part_buffer[0] == firmware_part_buffer[1]){
                    printf("Firmware update done\r\n");
                    flash.deinit();
                    return;            
                }
                break;
        }
    }

    
}

void init_i2c(I2CSlave *slave){
    int rc;
    const uint32_t id = *((uint32_t *)UNIQUE_ID_ADDR);

    uint16_t slave_addr = (id) % 95 + 0x10;

    uint16_t wait_time = (id) % 1000;

    slave->frequency(I2C_FREQ);
    slave->address(0);

    //ThisThread::sleep_for(wait_time);
    HAL_Delay(wait_time);

    led = 1;

    I2C master(I2C_FRAMEWORK_SDA, I2C_FRAMEWORK_SCL);

    master.frequency(I2C_FREQ);

    char data[1] = {0x0};

    //led = 1;

    do{
        rc = master.write(slave_addr << 1, data, 1, false);

        if(rc != 0){
            slave->address(slave_addr << 1);
        } else {
            slave_addr++;
        }
    
    } while (rc == 0);
}

void set_new_metadata(app_metadata_t *metadata_ram){
    FlashIAP flash;
    int rc;

    flash.init();
    rc = flash.erase(APPLICATION_METADATA_ADDRESS, 2048);
    if(rc != 0){
        printf("Error erasing firmware status\r\n");
    }

    rc = flash.program(metadata_ram, APPLICATION_METADATA_ADDRESS, sizeof(app_metadata_t));
    if(rc != 0){
        printf("Error writing firmware status\r\n");
    }
    flash.deinit();
}

int need_update_firmware(){
    app_metadata_t *metadata = (app_metadata_t*) APPLICATION_METADATA_ADDRESS;

    if(metadata->magic_firmware_need_update == MAGIC_FIRMWARE_NEED_UPDATE){
        return 1;
    } else {
        return 0;
    }
}

void start_firmware(){
    volatile uintptr_t application_address = FIRMWARE_APPLICATION_ADDRESS;

    printf("Starting the application at 0x%x\r\n", application_address);
    
    mbed_start_application(application_address);
}