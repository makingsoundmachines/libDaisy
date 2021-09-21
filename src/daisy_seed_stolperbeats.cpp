#include "daisy_seed_stolperbeats.h"

extern "C"
{
#include "dev/codec_ak4556.h"
}

using namespace daisy;

#define SEED_LED_PORT DSY_GPIOC
#define SEED_LED_PIN 7

#define SEED_TEST_POINT_PORT DSY_GPIOG
#define SEED_TEST_POINT_PIN 14

const dsy_gpio_pin seedgpio[31] = {
    // GPIO 1-8
    //{DSY_GPIOA, 8}, // removed on Rev4
    {DSY_GPIOB, 12},
    {DSY_GPIOC, 11},
    {DSY_GPIOC, 10},
    {DSY_GPIOC, 9},
    {DSY_GPIOC, 8},
    {DSY_GPIOD, 2},
    {DSY_GPIOC, 12},
    // GPIO 9-16
    {DSY_GPIOG, 10},
    {DSY_GPIOG, 11},
    {DSY_GPIOB, 4},
    {DSY_GPIOB, 5},
    {DSY_GPIOB, 8},
    {DSY_GPIOB, 9},
    {DSY_GPIOB, 6},
    {DSY_GPIOB, 7},
    // GPIO 17-24
    {DSY_GPIOC, 0},
    {DSY_GPIOA, 3},
    {DSY_GPIOB, 1},
    {DSY_GPIOA, 7},
    {DSY_GPIOA, 6},
    {DSY_GPIOC, 1},
    {DSY_GPIOC, 4},
    {DSY_GPIOA, 5},
    // GPIO 17-24
    {DSY_GPIOA, 4},
    {DSY_GPIOA, 1},
    {DSY_GPIOA, 0},
    {DSY_GPIOD, 11},
    {DSY_GPIOG, 9},
    {DSY_GPIOA, 2},
    {DSY_GPIOB, 14},
    {DSY_GPIOB, 15},
};

// Public Initialization

void DaisySeedStolperbeats::Configure()
{
    // Configure internal peripherals
    // ConfigureSdram(); // Stolperbeats has no SDRAM
    //ConfigureQspi();
    //ConfigureDac();

    // Configure the built-in GPIOs.
    /* led.pin.port       = SEED_LED_PORT;
    led.pin.pin        = SEED_LED_PIN;
    led.mode           = DSY_GPIO_MODE_OUTPUT_PP;
    testpoint.pin.port = SEED_TEST_POINT_PORT;
    testpoint.pin.pin  = SEED_TEST_POINT_PIN;
    testpoint.mode     = DSY_GPIO_MODE_OUTPUT_PP; */
}

void DaisySeedStolperbeats::Init(bool boost)
{
    //dsy_system_init();
    System::Config syscfg;
    boost ? syscfg.Boost() : syscfg.Defaults();
    system.Init(syscfg);

    // dsy_sdram_init(&sdram_handle);
    // dsy_qspi_init(&qspi_handle);
    // dsy_gpio_init(&led);
    // dsy_gpio_init(&testpoint);
    ConfigureAudio();

    callback_rate_ = AudioSampleRate() / AudioBlockSize();
    // Due to the added 16kB+ of flash usage,
    // and the fact that certain breakouts use
    // both; USB won't be initialized by the
    // SEED file.
    //usb_handle.Init(UsbHandle::FS_INTERNAL);
}

dsy_gpio_pin DaisySeedStolperbeats::GetPin(uint8_t pin_idx)
{
    dsy_gpio_pin p;
    pin_idx = pin_idx < 32 ? pin_idx : 0;
#ifndef SEED_REV2
    p = seedgpio[pin_idx];
#else
    p = {seed_ports[pin_idx], seed_pins[pin_idx]};
#endif
    return p;
}

void DaisySeedStolperbeats::DelayMs(size_t del)
{
    system.Delay(del);
}

void DaisySeedStolperbeats::StartAudio(AudioHandle::InterleavingAudioCallback cb)
{
    audio_handle.Start(cb);
}

void DaisySeedStolperbeats::StartAudio(AudioHandle::AudioCallback cb)
{
    audio_handle.Start(cb);
}

void DaisySeedStolperbeats::ChangeAudioCallback(AudioHandle::InterleavingAudioCallback cb)
{
    audio_handle.ChangeCallback(cb);
}

void DaisySeedStolperbeats::ChangeAudioCallback(AudioHandle::AudioCallback cb)
{
    audio_handle.ChangeCallback(cb);
}

void DaisySeedStolperbeats::StopAudio()
{
    audio_handle.Stop();
}

void DaisySeedStolperbeats::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
    audio_handle.SetSampleRate(samplerate);
    callback_rate_ = AudioSampleRate() / AudioBlockSize();
}

float DaisySeedStolperbeats::AudioSampleRate()
{
    return audio_handle.GetSampleRate();
}

void DaisySeedStolperbeats::SetAudioBlockSize(size_t blocksize)
{
    audio_handle.SetBlockSize(blocksize);
    callback_rate_ = AudioSampleRate() / AudioBlockSize();
}

size_t DaisySeedStolperbeats::AudioBlockSize()
{
    return audio_handle.GetConfig().blocksize;
}

float DaisySeedStolperbeats::AudioCallbackRate() const
{
    return callback_rate_;
}

void DaisySeedStolperbeats::SetLed(bool state)
{
    dsy_gpio_write(&led, state);
}

void DaisySeedStolperbeats::SetTestPoint(bool state)
{
    dsy_gpio_write(&testpoint, state);
}

// Private Implementation

void DaisySeedStolperbeats::ConfigureSdram()
{
    dsy_gpio_pin *pin_group;
    sdram_handle.state             = DSY_SDRAM_STATE_ENABLE;
    pin_group                      = sdram_handle.pin_config;
    pin_group[DSY_SDRAM_PIN_SDNWE] = dsy_pin(DSY_GPIOH, 5);
}
void DaisySeedStolperbeats::ConfigureQspi()
{
    dsy_gpio_pin *pin_group;
    qspi_handle.device = DSY_QSPI_DEVICE_IS25LP064A;
    qspi_handle.mode   = DSY_QSPI_MODE_DSY_MEMORY_MAPPED;
    pin_group          = qspi_handle.pin_config;


    pin_group[DSY_QSPI_PIN_IO0] = dsy_pin(DSY_GPIOD, 11);
    pin_group[DSY_QSPI_PIN_IO1] = dsy_pin(DSY_GPIOD, 12);
    pin_group[DSY_QSPI_PIN_IO2] = dsy_pin(DSY_GPIOE, 2);
    pin_group[DSY_QSPI_PIN_IO3] = dsy_pin(DSY_GPIOD, 13);
    pin_group[DSY_QSPI_PIN_CLK] = dsy_pin(DSY_GPIOB, 2);
    pin_group[DSY_QSPI_PIN_NCS] = dsy_pin(DSY_GPIOB, 10);
}
void DaisySeedStolperbeats::ConfigureAudio()
{
    // SAI2 - config
    // Example Config
    //      SAI2 Pins (available on pinout)
    //    pin_group = sai_handle.sai2_pin_config;
    //    pin_group[DSY_SAI_PIN_MCLK] = dsy_pin(DSY_GPIOA, 1);
    //    pin_group[DSY_SAI_PIN_FS]   = dsy_pin(DSY_GPIOG, 9);
    //    pin_group[DSY_SAI_PIN_SCK]  = dsy_pin(DSY_GPIOA, 2);
    //    pin_group[DSY_SAI_PIN_SIN]  = dsy_pin(DSY_GPIOD, 11);
    //    pin_group[DSY_SAI_PIN_SOUT] = dsy_pin(DSY_GPIOA, 0);

    // SAI1 -- Peripheral
    // Configure
    SaiHandle::Config sai_config;
    sai_config.periph          = SaiHandle::Config::Peripheral::SAI_1;
    sai_config.sr              = SaiHandle::Config::SampleRate::SAI_48KHZ;
    sai_config.bit_depth       = SaiHandle::Config::BitDepth::SAI_24BIT;
    sai_config.a_sync          = SaiHandle::Config::Sync::MASTER;
    sai_config.b_sync          = SaiHandle::Config::Sync::SLAVE;
    sai_config.a_dir           = SaiHandle::Config::Direction::TRANSMIT;
    sai_config.b_dir           = SaiHandle::Config::Direction::RECEIVE;
    sai_config.pin_config.fs   = {DSY_GPIOE, 4};
    sai_config.pin_config.mclk = {DSY_GPIOE, 2};
    sai_config.pin_config.sck  = {DSY_GPIOE, 5};
    sai_config.pin_config.sa   = {DSY_GPIOE, 6};
    sai_config.pin_config.sb   = {DSY_GPIOE, 3};
    // Then Initialize
    SaiHandle sai_1_handle;
    sai_1_handle.Init(sai_config);

    // Device Init
    dsy_gpio_pin codec_reset_pin;
    codec_reset_pin = {DSY_GPIOB, 11};
    //codec_ak4556_init(codec_reset_pin);
    Ak4556::Init(codec_reset_pin);

    // Audio
    AudioHandle::Config audio_config;
    audio_config.blocksize  = 48;
    audio_config.samplerate = SaiHandle::Config::SampleRate::SAI_48KHZ;
    audio_config.postgain   = 1.f;
    audio_handle.Init(audio_config, sai_1_handle);
}
void DaisySeedStolperbeats::ConfigureDac()
{
    // This would be the equivalent initialization as previously existed.
    // However, not all platforms have the DAC, and many use those pins
    // for other things.
    //    DacHandle::Config cfg;
    //    cfg.bitdepth   = DacHandle::Config::BitDepth::BITS_12;
    //    cfg.buff_state = DacHandle::Config::BufferState::ENABLED;
    //    cfg.mode       = DacHandle::Config::Mode::POLLING;
    //    cfg.chn        = DacHandle::Config::Channel::BOTH;
    //    dac.Init(cfg);
}
/*void DaisySeedStolperbeats::ConfigureI2c()
{
    dsy_gpio_pin *pin_group;
    // TODO: Add Config for I2C3 and I2C4
    // I2C 1 - (On daisy patch this controls the LED Driver, and the WM8731).
    i2c1_handle.periph              = DSY_I2C_PERIPH_1;
    i2c1_handle.speed               = DSY_I2C_SPEED_400KHZ;
    pin_group                       = i2c1_handle.pin_config;
    pin_group[DSY_I2C_PIN_SCL].port = DSY_GPIOB;
    pin_group[DSY_I2C_PIN_SCL].pin  = 8;
    pin_group[DSY_I2C_PIN_SDA].port = DSY_GPIOB;
    pin_group[DSY_I2C_PIN_SDA].pin  = 9;
    // I2C 2 - (On daisy patch this controls the on-board WM8731)
    i2c2_handle.periph              = DSY_I2C_PERIPH_2;
    i2c2_handle.speed               = DSY_I2C_SPEED_400KHZ;
    pin_group                       = i2c2_handle.pin_config;
    pin_group[DSY_I2C_PIN_SCL].port = DSY_GPIOH;
    pin_group[DSY_I2C_PIN_SCL].pin  = 4;
    pin_group[DSY_I2C_PIN_SDA].port = DSY_GPIOB;
    pin_group[DSY_I2C_PIN_SDA].pin  = 11;
}*/
