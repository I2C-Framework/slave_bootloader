#include "mbed.h"
#include "FlashIAP.h"
#include "ResetReason.h"

#if !defined(POST_APPLICATION_ADDR)
#error "target.restrict_size must be set for your target in mbed_app.json"
#endif

#define FIRMWARE_HEADER_ADDRESS 0x08009000
#define FIRMWARE_APPLICATION_ADDRESS 0x08009400
#define FIRMWARE_STATUS_ADDRESS 0x0801FF00
#define END_MCU_ADDRESS 0x0801FFFF

#define I2C_FREQ (400000)
#define FIRMWARE_SIZE (0xF000)

#define BUFFER_DATASIZE (2048)
#define BUFFER_SIZE (BUFFER_DATASIZE + 1 + 1) // Buffer + block size + num block
#define UNIQUE_ID_ADDR (0x1FFF7590)

#define MAGIC_NUMBER (0xdeadbeef)
#define FIRMWARE_UPDATE_OK (0x97)

typedef struct {
    uint32_t magic;
    uint64_t firmware_size;
    uint32_t firmware_crc;
    uint16_t major_version;
    uint16_t minor_version;
    uint16_t fix_version;
}__attribute__((__packed__)) app_metadata_t;

void start_firmware();
int is_soft_reset();
int need_update_firmware();
void set_update_firmware_flag(uint8_t flag);
void init_i2c(I2CSlave *slave);
void wait_for_update_firmware(I2CSlave *slave);
int is_magic_valid();
int is_crc_valid();

int main()
{
    printf("Launching the bootloader\r\n");

    if(is_soft_reset()){

        if(need_update_firmware()){

            I2CSlave slave(D14, D15);
            init_i2c(&slave);

            printf("Bootloader ready for firmware update\r\n");

            wait_for_update_firmware(&slave);
            
        } else {
            printf("No need update\r\n");
        }        
    }

    if(!is_magic_valid()){
        printf("Magic number invalid\r\n");
        set_update_firmware_flag(0);
        NVIC_SystemReset();
    }

    if(!is_crc_valid()){
        printf("CRC invalid\r\n");
        set_update_firmware_flag(0);
        NVIC_SystemReset();
    }

    set_update_firmware_flag(FIRMWARE_UPDATE_OK);

    start_firmware();
}

int is_magic_valid(){
    app_metadata_t *normal_firmware_header = (app_metadata_t*) FIRMWARE_HEADER_ADDRESS;

    if(normal_firmware_header->magic != MAGIC_NUMBER){
        return 0;
    } else {
        return 1;
    }
}

int is_crc_valid(){
    app_metadata_t *normal_firmware_header = (app_metadata_t*) FIRMWARE_HEADER_ADDRESS;
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

    while(1){
        int i = slave->receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                break;
            case I2CSlave::WriteGeneral:
                break;
            case I2CSlave::WriteAddressed:
                rc = slave->read(firmware_part_buffer, BUFFER_SIZE);

                //printf("0:%d, 1:%d, 2:%d, 3:%d\r\n", firmware_part_buffer[0], firmware_part_buffer[1], firmware_part_buffer[2], firmware_part_buffer[3]);

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

    thread_sleep_for(wait_time);

    I2C master(D14, D15);

    master.frequency(I2C_FREQ);

    char data[1] = {0x0};

    do{
        rc = master.write(slave_addr << 1, data, 1, false);

        if(rc != 0){
            slave->address(slave_addr << 1);
        } else {
            slave_addr++;
        }
    
    } while (rc == 0);
}

void set_update_firmware_flag(uint8_t flag){
    FlashIAP flash;
    int rc;
    char status_buffer[1];

    flash.init();
    uint32_t sector_size = flash.get_sector_size(FIRMWARE_STATUS_ADDRESS);
    uint32_t sector_address = FIRMWARE_STATUS_ADDRESS - (FIRMWARE_STATUS_ADDRESS % sector_size);

    rc = flash.erase(sector_address, sector_size);
    if(rc != 0){
        printf("Error erasing firmware status\r\n");
    }

    status_buffer[0] = flag;
    rc = flash.program(status_buffer, sector_address, 1);
    if(rc != 0){
        printf("Error writing firmware status\r\n");
    }
    flash.deinit();
}

int need_update_firmware(){
    FlashIAP flash;
    char status_buffer[1];

    flash.init();
    uint32_t sector_size = flash.get_sector_size(FIRMWARE_STATUS_ADDRESS);
    uint32_t sector_address = FIRMWARE_STATUS_ADDRESS - (FIRMWARE_STATUS_ADDRESS % sector_size);
    flash.read(status_buffer, sector_address, 1);

    flash.deinit();

    if(status_buffer[0] == FIRMWARE_UPDATE_OK){
        return 0;
    } else {
        return 1;
    }
}

int is_soft_reset()
{
    const reset_reason_t reason = ResetReason::get();
    switch (reason) {
        case RESET_REASON_POWER_ON:
            return 0;//Power On";
        case RESET_REASON_PIN_RESET:
            return 0;//"Hardware Pin";
        case RESET_REASON_SOFTWARE:
            return 1;//"Software Reset";
        case RESET_REASON_WATCHDOG:
            return 0;//"Watchdog";
        default:
            return 0;//"Other Reason";
    }
}

void start_firmware(){
    volatile uintptr_t application_address = FIRMWARE_APPLICATION_ADDRESS;

    printf("Starting the application at 0x%x\r\n", application_address);
    
    mbed_start_application(application_address);
}