#pragma once
#ifndef DSY_DEV_LED_IS31FL3731_H
#define DSY_DEV_LED_IS31FL3731_H /**< Macro */
#include <stdlib.h>
#include <stdint.h>
#include "daisy_core.h"
#include "per/i2c.h"


namespace daisy
{
/** @addtogroup led
    @{ 
*/

/** 
    Driver for ISSI IS31FL3731 144 ch PWM LED Matrix
    Port for Daisy by Making Sound Machines  - https://github.com/makingsoundmachines
*/

class Is31fl3731
{
  public:

    Is31fl3731() {}
    ~Is31fl3731() {}


    // configuration currently only uses SPI1, w/ soft chip select.

    /** 
    Initialize ISSI IS31FL3731
    */
    void Init(const I2CHandle::Config config);

    void Test_mode();

    /*void Set(int channel, Value value) {
        _values[channel] = value;
    }

    void Write(int channel);
    void Write();*/

  private:
    void WriteIs31fl3731(uint8_t address, uint8_t command, uint8_t data);

    void Reset();
    void SetInternalRef(bool enabled);
      
};
/** @} */
} // namespace daisy

#endif
