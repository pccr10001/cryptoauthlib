/*
 * Copyright 2018 Espressif Systems (Shanghai) PTE LTD
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <Wire.h>
#include <stdio.h>
#include <string.h>
#include "../atca_iface.h"
#include "../cryptoauthlib.h"
#include "atca_hal.h"

#ifndef ATCA_HAL_ESP32

void atca_delay_ms(uint32_t ms) { delay(ms); }

void hal_i2c_change_baud(ATCAIface iface, uint32_t speed) {
  Wire.setClock(speed);
}

ATCA_STATUS hal_i2c_init(void *hal, ATCAIfaceCfg *cfg) {
  Wire.begin();
  Wire.setClock(cfg->atcai2c.baud);
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_post_init(ATCAIface iface) { return ATCA_SUCCESS; }

ATCA_STATUS hal_i2c_send(ATCAIface iface, uint8_t *txdata, int txlength) {
  ATCAIfaceCfg *cfg = iface->mIfaceCFG;
  txdata[0] = 0x03;
  txlength++;

  Wire.beginTransmission(cfg->atcai2c.slave_address);
  Wire.write(txdata, txlength);
  Wire.endTransmission();

  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_receive(ATCAIface iface, uint8_t *rxdata,
                            uint16_t *rxlength) {
  ATCAIfaceCfg *cfg = iface->mIfaceCFG;
  int addr = cfg->atcai2c.slave_address;

  Wire.requestFrom(addr, 1);

  rxdata[0] = Wire.read();
  *rxlength = rxdata[0];

  if (*rxlength > 1) {
    Wire.requestFrom(addr, *rxlength - 1);
    Wire.readBytes(&rxdata[1], *rxlength - 1);
  }

  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_release(void *hal_data) {
  Wire.end();
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_wake(ATCAIface iface) {
  ATCAIfaceCfg *cfg = atgetifacecfg(iface);
  int addr = cfg->atcai2c.slave_address;

  uint8_t wake = 0x00;

  uint16_t rxlen;
  uint8_t data[4] = {0};
  const uint8_t expected[4] = {0x04, 0x11, 0x33, 0x43};

  Wire.beginTransmission(addr);
  Wire.write(wake);
  Wire.endTransmission();

  atca_delay_ms(10);
  
  Wire.requestFrom(addr, 4);
  rxlen = 4;
  Wire.readBytes(data, rxlen);

  if (memcmp(data, expected, 4) == 0) {
    return ATCA_SUCCESS;
  }
  return ATCA_COMM_FAIL;
}

ATCA_STATUS hal_i2c_idle(ATCAIface iface) {
  ATCAIfaceCfg *cfg = atgetifacecfg(iface);
  int addr = cfg->atcai2c.slave_address;
  uint8_t idle_data = 0x02;

  Wire.beginTransmission(addr);
  Wire.write(idle_data);

  Wire.endTransmission();
  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_sleep(ATCAIface iface) {
  ATCAIfaceCfg *cfg = atgetifacecfg(iface);
  int addr = cfg->atcai2c.slave_address;
  uint8_t sleep_data = 0x01;

  Wire.beginTransmission(addr);
  Wire.write(sleep_data);
  Wire.endTransmission();

  return ATCA_SUCCESS;
}

ATCA_STATUS hal_i2c_discover_buses(int i2c_buses[], int max_buses) {
  return ATCA_UNIMPLEMENTED;
}

ATCA_STATUS hal_i2c_discover_devices(int bus_num, ATCAIfaceCfg *cfg,
                                     int *found) {
  return ATCA_UNIMPLEMENTED;
}

#endif