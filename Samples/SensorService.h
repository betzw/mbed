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

#ifndef __SENSOR_SERVICE_H__
#define __SENSOR_SERVICE_H__

#include "UUID.h"
#include "BLEDevice.h"

#include "Utils.h"

#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )
typedef struct {
  int32_t AXIS_X;
  int32_t AXIS_Y;
  int32_t AXIS_Z;
} AxesRaw_t;

extern AxesRaw_t axes_data;

// Motion Sensor UUIDs
extern const uint8_t  AccServiceUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  AccServiceFreeFallCharacteristicUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  AccServiceAccCharacteristicUUID[LENGTH_OF_LONG_UUID];

// Environmental Sensor UUIDs
extern const uint8_t  EnvSensServiceUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  TempCharacteristicUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  PressCharacteristicUUID[LENGTH_OF_LONG_UUID];
extern const uint8_t  HumidityCharacteristicUUID[LENGTH_OF_LONG_UUID];

/**
* @class SensorService
* @brief 
*/
class SensorService {
public:
    /**
     * @brief Constructor
     */
    SensorService(BLEDevice &_ble) :
        ble(_ble),
        accValueBytes(),
        freeFall(0),
        temperature(0),
        pressure(0),
        humidity(0),
        accChar(AccServiceAccCharacteristicUUID, accValueBytes.getPointer(),
                AccValueBytes::ACC_BUFF_BYTES, AccValueBytes::ACC_BUFF_BYTES,
                GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        freeFallChar(AccServiceFreeFallCharacteristicUUID, &freeFall, 1, 1, GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY),
        tempChar(TempCharacteristicUUID, &temperature),
        pressChar(PressCharacteristicUUID, &pressure),
        humidityChar(HumidityCharacteristicUUID, &humidity) {

        GattCharacteristic *accServiceCharTable[] = {&freeFallChar, &accChar};
        GattService         accService(AccServiceUUID, accServiceCharTable, sizeof(accServiceCharTable) / sizeof(GattCharacteristic *));
        
        ble.addService(accService);
        
        GattCharacteristic *envSensServiceCharTable[] = {&tempChar, &pressChar, &humidityChar};
        GattService         envSensService(EnvSensServiceUUID, envSensServiceCharTable, sizeof(envSensServiceCharTable) / sizeof(GattCharacteristic *));
        
        ble.addService(envSensService);

        ble.onDataRead(this, &SensorService::onDataRead);
    }

    /**
     * 
     */
    void updateAcc(bool newVal) {
        if(newVal) {
            accValueBytes.updateAcc();
        }
        ble.updateCharacteristicValue(accChar.getValueAttribute().getHandle(), accValueBytes.getPointer(), accValueBytes.getNumValueBytes());
    }

    /**
     * 
     */
    void updateFreeFall(void) {
        freeFall = 1;
        ble.updateCharacteristicValue(freeFallChar.getValueAttribute().getHandle(), &freeFall, 1);
    }

    /**
     * 
     */
    void updateTemperature(void) {
        temperature = 270 + ((uint64_t)rand()*15)/RAND_MAX; //sensor emulation
        
        ble.updateCharacteristicValue(tempChar.getValueAttribute().getHandle(), (uint8_t*)&temperature, 2);
    }

    /**
     * 
     */
    void updatePressure(void) {
        pressure = 100000 + ((uint64_t)rand()*1000)/RAND_MAX;
        
        ble.updateCharacteristicValue(pressChar.getValueAttribute().getHandle(), (uint8_t*)&pressure, 3);
    }

    /**
     * 
     */
    void updateHumidity(void) {
        humidity = 450 + ((uint64_t)rand()*100)/RAND_MAX;
        
        ble.updateCharacteristicValue(humidityChar.getValueAttribute().getHandle(), (uint8_t*)&humidity, 2);
    }

    void onDataRead(const GattCharacteristicReadCBParams *params) {
        uint16_t charHandle = params->charHandle;
        if (charHandle == accChar.getValueAttribute().getHandle()+1) {
            updateAcc(false);
        } else if (charHandle == tempChar.getValueAttribute().getHandle()+1) {
            updateAcc(false);
            updateTemperature();
        } else if (charHandle == pressChar.getValueAttribute().getHandle()+1) {
            updatePressure();
        } else if (charHandle == humidityChar.getValueAttribute().getHandle()+1) {
            updateHumidity();
        }
    }
    
private:
    /* Private internal representation for the bytes used to work with the vaulue of the Acc characteristic. */
    struct AccValueBytes {
        static const unsigned ACC_BUFF_BYTES = 6;

        AccValueBytes(void) : valueBytes() {
            
            for(int i=0; i<ACC_BUFF_BYTES; i++) {
                valueBytes[i]=0;
            }
        }

        void updateAcc(void) {
            axes_data.AXIS_X += 100;
            axes_data.AXIS_Y += 100;
            axes_data.AXIS_Z += 100;
            
            //DEBUG("ACC: X=%6d Y=%6d Z=%6d\r\n", axes_data.AXIS_X, axes_data.AXIS_Y, axes_data.AXIS_Z);
            
            STORE_LE_16(valueBytes,axes_data.AXIS_X);
            STORE_LE_16(valueBytes+2,axes_data.AXIS_Y);
            STORE_LE_16(valueBytes+4,axes_data.AXIS_Z);
            
        }

        uint8_t       *getPointer(void) {
            return valueBytes;
        }

        const uint8_t *getPointer(void) const {
            return valueBytes;
        }

        unsigned       getNumValueBytes(void) const {
            return ACC_BUFF_BYTES;
        }

private:
        uint8_t valueBytes[ACC_BUFF_BYTES];
    };
    
private:
    BLEDevice           &ble;
    
    AccValueBytes       accValueBytes;
    uint8_t             freeFall;
    int16_t             temperature;
    int32_t             pressure;
    int16_t             humidity;

    GattCharacteristic  accChar;
    GattCharacteristic  freeFallChar;
    
    ReadOnlyGattCharacteristic<int16_t> tempChar;
    ReadOnlyGattCharacteristic<int32_t> pressChar;
    ReadOnlyGattCharacteristic<int16_t> humidityChar;
};

#endif /* __SENSOR_SERVICE_H__ */