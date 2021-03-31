#include "led_is31fl3731.h"
#include "../daisy_core.h"
#include "../per/i2c.h"
#include "../per/gpio.h"
#include "../sys/system.h"

// Driver for ISSI IS31FL3731 144 ch PWM LED Matrix
// Port for Daisy by Making Sound Machines  - https://github.com/makingsoundmachines

// Pin Names in Schematics
//
// Patch       Seed           STM32H7   IS31FL3731
//
// PIN_ENC_B   I2C1_SDA  11   PB8       SDA
// PIN_ENC_A   I2C1_SCL  12   PB9       SCL


// Commands

#define ISSI_ADDR_DEFAULT 0b01110100 // 0x74

#define ISSI_REG_CONFIG 0x00
#define ISSI_REG_CONFIG_PICTUREMODE 0x00
#define ISSI_REG_CONFIG_AUTOPLAYMODE 0x08
#define ISSI_REG_CONFIG_AUDIOPLAYMODE 0x18

#define ISSI_CONF_PICTUREMODE 0x00
#define ISSI_CONF_AUTOFRAMEMODE 0x04
#define ISSI_CONF_AUDIOMODE 0x08

#define ISSI_REG_PICTUREFRAME 0x01

#define ISSI_REG_SHUTDOWN 0x0A
#define ISSI_REG_AUDIOSYNC 0x06

#define ISSI_COMMANDREGISTER 0xFD
#define ISSI_BANK_FUNCTIONREG 0x0B // helpfully called 'page nine'


using namespace daisy;

//static uint8_t Dac8568_Buffer[128];

typedef struct
{
    uint8_t  Initialized;
} Is31fl3731_t;


static Is31fl3731_t Is31fl3731_;
static I2CHandle h_i2c;
static I2CHandle::Config _i2c_config; 

void Is31fl3731::Init(const I2CHandle::Config config) 
{
    
    _i2c_config = config;

    // Initialize I2C
    h_i2c.Init(_i2c_config);

    // now i2c points to the corresponding peripheral and can be used.
    // Init procedure used here is from the ISSI example code.

    
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0xFD, 0x0B); // 0xFD, 0x0B  write function register
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0x0A, 0x00); // 0x0A, 0x00  enter software shutdown mode
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0xFD, 0x00); // 0xFD, 0x00  write first frame

    //turn on all LED
    for(uint8_t k = 0; k < 0x12; k++)
    {
        WriteIs31fl3731(ISSI_ADDR_DEFAULT, k, 0xff);
    } 
    
    //Need to turn off the position where LED is not mounted
    //write all PWM set 0x00
    for(uint8_t k = 0x24; k < 0xB4; k++)
    {
        WriteIs31fl3731(ISSI_ADDR_DEFAULT, k, 0x00);
    }

    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0xFD, 0x0B); // 0xFD, 0x0B  write function register
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0x00, 0x00); // 0x00, 0x00  picture mode
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0x01, 0x00); // 0x01, 0x00  select first frame
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0x0A, 0x01); // 0x0A, 0x01  normal operation
    WriteIs31fl3731(ISSI_ADDR_DEFAULT, 0xFD, 0x00); // 0xFD, 0x00  write first frame

    //write all PWM set 0x80
    /*for(uint8_t k = 0x24; k < 0xB4; k++)
    {
        i2c_buff[0] = k;
        i2c_buff[1] = 0x80;
        i2c.TransmitBlocking(IS31FL3731_ADDRESS, i2c_buff, 2, 1000);
    }*/
    System::DelayUs(20); // wait for clear

    Is31fl3731_.Initialized = 1;
}

/*
void Is31fl3731::Write(int channel) {
    WriteIs31fl3731(WRITE_INPUT_REGISTER_UPDATE_N, channel, _values[channel], 15);
}

void Is31fl3731::Write() {
    for (int channel = 0; channel < Channels; ++channel) {
        WriteIs31fl3731(channel == 7 ? WRITE_INPUT_REGISTER_UPDATE_ALL : WRITE_INPUT_REGISTER, channel, _values[channel], 0);
    }
}
*/

void Is31fl3731::WriteIs31fl3731(uint8_t address, uint8_t command, uint8_t data) {
    uint8_t b[2];

    b[0] = command;
    b[1] = data;

    h_i2c.TransmitBlocking(address, b, 2, 1000);
}


void Is31fl3731::Test_mode()
{
    for(uint8_t j = 0x00; j < 0x80; j++) //all LED ramping up
    {
        for(uint8_t i = 0x24; i < 0xB4; i++)
        {
            WriteIs31fl3731(ISSI_ADDR_DEFAULT, i, j);
        }
    }

    for(uint8_t j = 0x80; j > 0x00; j--) //all LED ramping down
    {
        for(uint8_t i = 0x24; i < 0xB4; i++)
        {
            WriteIs31fl3731(ISSI_ADDR_DEFAULT, i, j);
        }
    }
}


/*
void Is31fl3731::Reset() {
    WriteIs31fl3731(RESET_POWER_ON, 0, 0, 0);
    // hal::Delay::delay_us(50);
}

void Is31fl3731::SetInternalRef(bool enabled) {
    WriteIs31fl3731(SETUP_INTERNAL_REF, 0, 0, enabled ? 1 : 0);
}

void Is31fl3731::SetClearCode(ClearCode code) {
    WriteIs31fl3731(LOAD_CLEAR_CODE_REGISTER, 0, 0, code);
}
*/


