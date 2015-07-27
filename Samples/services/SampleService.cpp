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

#include "SampleService.h"

const uint16_t SampleServiceShortUUID = 0x200C;

// Sample Service UUIDs
const uint8_t  SampleServiceUUID[UUID::LENGTH_OF_LONG_UUID] = {
  0x66, 0x9A, (uint8_t)(SampleServiceShortUUID >> 8), (uint8_t)(SampleServiceShortUUID & 0xFF), 0x00, 0x08, 0x96, 0x9E,
  0xE2, 0x11, 0x9E, 0xB1, 0xE0, 0xF2, 0x73, 0xD9
};
const uint8_t  TXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
  0x66, 0x9A, (uint8_t)(SampleServiceShortUUID >> 8), (uint8_t)(SampleServiceShortUUID & 0xFF), 0x00, 0x08, 0x96, 0x9E,
  0xE2, 0x11, 0x9E, 0xB1, 0xE1, 0xF2, 0x73, 0xD9
};
const uint8_t  RXCharacteristicUUID[UUID::LENGTH_OF_LONG_UUID] = {
  0x66, 0x9A, (uint8_t)(SampleServiceShortUUID >> 8), (uint8_t)(SampleServiceShortUUID & 0xFF), 0x00, 0x08, 0x96, 0x9E,
  0xE2, 0x11, 0x9E, 0xB1, 0xE2, 0xF2, 0x73, 0xD9
};

