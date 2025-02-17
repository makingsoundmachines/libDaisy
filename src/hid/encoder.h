#pragma once
#ifndef DSY_ENCODER_H
#define DSY_ENCODER_H
#include "daisy_core.h"
#include "per/gpio.h"
#include "hid/switch.h"

namespace daisy
{
/** 
    @brief Generic Class for handling Quadrature Encoders \n 
    Inspired/influenced by Mutable Instruments (pichenettes) Encoder classes
    @author Stephen Hensley
    @date December 2019
    @ingroup controls
*/
class Encoder
{
  public:
    Encoder() {}
    ~Encoder() {}

    /** Initializes the encoder with the specified hardware pins.
     * Update rate is to be deprecated in a future release
     */
    void Init(dsy_gpio_pin a,
              dsy_gpio_pin b,
              dsy_gpio_pin click,
              float        update_rate = 0.f);
    /** Called at update_rate to debounce and handle timing for the switch.
     * In order for events not to be missed, its important that the Edge/Pressed checks be made at the same rate as the debounce function is being called.
     */
    void Debounce();
    void Debounce16();
    void Debounce32();
    /** Returns +1 if the encoder was turned clockwise, -1 if it was turned counter-clockwise, or 0 if it was not just turned.
*/
    inline int32_t Increment() const { return inc_; }

    /** Returns true if the encoder was just pressed. */
    inline bool RisingEdge() const { return sw_.RisingEdge(); }
    inline bool RisingEdge16() const { return sw_.RisingEdge16(); }
    inline bool RisingEdge32() const { return sw_.RisingEdge32(); }
    /** Returns true if the encoder was just released.
*/
    inline bool FallingEdge() const { return sw_.FallingEdge(); }
    inline bool FallingEdge16() const { return sw_.FallingEdge16(); }
    inline bool FallingEdge32() const { return sw_.FallingEdge32(); }
    /** Returns true while the encoder is held down.
*/
    inline bool Pressed() const { return sw_.Pressed(); }
    inline bool Pressed16() const { return sw_.Pressed16(); }
    inline bool Pressed32() const { return sw_.Pressed32(); }
    /** Returns the time in milliseconds that the encoder has been held down.
*/
    inline float TimeHeldMs() const { return sw_.TimeHeldMs(); }
    inline float TimeHeldMs16() const { return sw_.TimeHeldMs16(); }
    inline float TimeHeldMs32() const { return sw_.TimeHeldMs32(); }

    /** To be removed in breaking update
     * \param update_rate Does nothing
    */
    inline void SetUpdateRate(float update_rate) {}

  private:
    Switch   sw_;
    dsy_gpio hw_a_, hw_b_;
    uint8_t  a_, b_;
    uint16_t  a16_, b16_;
    uint32_t  a32_, b32_;
    int32_t  inc_;
};
} // namespace daisy
#endif
