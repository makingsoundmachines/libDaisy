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
 * Driver for ISSI IS31FL3731 144 ch PWM LED Matrix
 * Port for Daisy by Making Sound Machines  - https://github.com/makingsoundmachines
 *
 * LED driver for one or multiple ISSI IS31FL3731 144ch 8bit PWM chips connected to
 * a single I2C peripheral.
 * This driver uses two buffers - one for drawing, one for transmitting.
 * Multiple Is31fl3731 instances can be used at the same time.
 */

class Is31fl3731
{
  public:

    Is31fl3731() {}
    ~Is31fl3731() {}


    // configuration currently only uses SPI1, w/ soft chip select.

    /** 
    Initialize ISSI IS31FL3731 driver

     * \param i2c           The I2C peripheral to use.
     * \param addresses     An array of addresses for each of the driver chips.
     * \param numDrivers    The number of IS31FL3731 drivers attached to the I2C
     *                      peripheral.
    */    

    void Init(const I2CHandle i2c, const uint8_t * addresses, uint8_t numDrivers);

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

    uint8_t * addresses_;
    uint8_t numDrivers_;
      
};
/** @} */
} // namespace daisy

#endif
