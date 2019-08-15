# CryptoAuthLib - Microchip CryptoAuthentication Library
## Introduction
### Microchip CryptoAuth devices SDK for:
* ATECC*08A
* ATSHA204A

[Check out more infos on original repository](https://github.com/MicrochipTech/cryptoauthlib)

### More features
* Add support for Arduino devices using Wire library.
* Modify some code for ESP32 to be more easy to use.

## How to use
1. Include `cryptoauthlib.h` in your code.
2. Configure `cryptoauthlib.h` for boards.
```c=

// Enable for ESP32
// Disable for Arduino
// #define ATCA_HAL_ESP32

#ifdef ATCA_HAL_ESP32
#define USE_MBEDTLS

// Change I2C pinout here
#define SDA_PIN 25
#define SCL_PIN 26

#endif
```

3. Happy coding!
```c=

// Arduino
ATCAIfaceCfg cfg = cfg_ateccx08a_i2c_arduino;

// ESP32
// ATCAIfaceCfg cfg = cfg_ateccx08a_i2c_esp32;

ATCA_STATUS ret = atcab_init(&cfg);

// Get random number
uint8_t rand[32];
ret = atcab_random(rand);

if(ret != ATCA_SUCCESS){
    Serial.printf("ATCA Error ret = %02X\n", ret);
}

```
