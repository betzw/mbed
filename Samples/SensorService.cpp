/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SensorService.h"

AxesRaw_t axes_data = {0, 0, 0};

const uint16_t SensServiceShortUUID = 0xA5D5;

// Motion Sensor UUIDs
const uint8_t  AccServiceUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0xB4, 0x9A,
    0xE1, 0x11, 0x3A, 0xCF, 0x80, 0x6E, 0x36, 0x02
};
const uint8_t  AccServiceFreeFallCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0xFC, 0x8F,
    0xE1, 0x11, 0x4A, 0xCF, 0xA0, 0x78, 0x3E, 0xE2
};
const uint8_t  AccServiceAccCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0x36, 0xAC,
    0xE1, 0x11, 0x4B, 0xCF, 0x80, 0x1B, 0x0A, 0x34
};


// Environmental Sensor UUIDs
const uint8_t  EnvSensServiceUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0xD0, 0x82,
    0xE2, 0x11, 0x77, 0xE4, 0x40, 0x1A, 0x82, 0x42
};
const uint8_t  TempCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0xE3, 0xA9,
    0xE2, 0x11, 0x77, 0xE4, 0x20, 0x55, 0x2E, 0xA3
};
const uint8_t  PressCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0x0B, 0x84,
    0xE2, 0x11, 0x8B, 0xE4, 0x80, 0xC4, 0x20, 0xCD
};
const uint8_t  HumidityCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
    0x1B, 0xC5, (uint8_t)(SensServiceShortUUID >> 8), (uint8_t)(SensServiceShortUUID & 0xFF), 0x02, 0x00, 0x73, 0xA0,
    0xE2, 0x11, 0x8C, 0xE4, 0x60, 0x0B, 0xC5, 0x01
};
