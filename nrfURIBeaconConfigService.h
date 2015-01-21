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

#ifndef __NRF_BLE_URI_BEACON_CONFIG_SERVICE_H__
#define __NRF_BLE_URI_BEACON_CONFIG_SERVICE_H__

#include "pstorage.h"
#include <nrf_error.h>
#include "URIBeaconConfigService.h"

/**
* @class nrfURIBeaconConfigService
* @brief UriBeacon Configuration Service for nordic devices.
*/
class nrfURIBeaconConfigService : public URIBeaconConfigService {
public:
    /**
     * @param[ref] ble
     *                 BLEDevice object for the underlying controller.
     * @param[in]  uridata
     *                 URI as a null-terminated string.
     * @param[in]  flagsIn
     *                 UriBeacon Flags.
     * @param[in]  powerLevels[]
     *                 Table of UriBeacon Tx Power Levels in dBm.
     * @param[in]  powerMode
     *                 Currently effective power mode.
     * @param[in]  beaconPeriodIn
     *                 The period in milliseconds that a UriBeacon packet is
     *                 transmitted. A value of zero disables UriBeacon
     *                 transmissions.
     */
    nrfURIBeaconConfigService(BLEDevice      &bleIn,
                              const char     *uriDataIn,
                              uint8_t         flagsIn                        = 0,
                              const int8_t    powerLevelsIn[NUM_POWER_MODES] = NULL,
                              URIBeaconConfigService::TXPowerModes_t powerModeIn = URIBeaconConfigService::TX_POWER_MODE_LOW,
                              uint16_t        beaconPeriodIn                 = 1000) :
        URIBeaconConfigService(bleIn, uriDataIn, flagsIn, powerLevelsIn, powerModeIn, beaconPeriodIn),
        pstorageHandle(),
        persistentData() {
        pstorage_init();
        pstorage_module_param_t pstorageParams = {
            .cb          = pstorageNotificationCallback,
            .block_size  = sizeof(PersistentData_t),
            .block_count = 1
        };
        pstorage_register(&pstorageParams, &pstorageHandle);
        storage_loadLockBits();
    }

private:
    static const uint32_t MAGIC = 0x1BEAC000;
    struct PersistentData_t {
        uint32_t magic; /* indicates a successful previous write */
        URIBeaconConfigService::LockBits_t lockBits;
    };

    /**
     * APIs around making lockBits persistent.
     */
private:
    /**
     * Have we previously saved lockedBits? Once set, this state is expected to persist.
     * @return true if we've previously saved locked bits.
     */
    virtual bool storage_haveSavedLockBits() const {
        return (persistentData.magic == MAGIC);
    }

    /**
     * Save the current value of lockBits into persistent storage; this value is
     * then retrievable by lockLockBits() until a subsequent call to saveLockBits().
     */
    virtual void storage_saveLockBits() {
        bool initialSave = !storage_haveSavedLockBits();

        persistentData.magic = MAGIC;
        copyLockBitsInto(persistentData.lockBits);

        if (initialSave) {
            if (pstorage_store(&pstorageHandle,
                               reinterpret_cast<std::uint8_t *>(&persistentData),
                               sizeof(PersistentData_t),
                               0 /* offset */) != NRF_SUCCESS) {
                // printf("nrfURIBeaconConfigService::saveLockBits: store() failed\r\n");
                return;
            }
        } else {
            if (pstorage_update(&pstorageHandle,
                                reinterpret_cast<std::uint8_t *>(&persistentData),
                                sizeof(PersistentData_t),
                                0 /* offset */) != NRF_SUCCESS) {
                // printf("nrfURIBeaconConfigService::saveLockBits: update() failed\r\n");
                return;
            }
        }
    }

    /**
     * Retrieve the saved lockBits from persistent storage and update the class
     * member 'lockBits'.
     */
    virtual void storage_loadLockBits() {
        if (pstorage_load(reinterpret_cast<std::uint8_t *>(&persistentData),
                          &pstorageHandle,
                          sizeof(PersistentData_t),
                          0 /* offset */) != NRF_SUCCESS) {
            // printf("nrfURIBeaconConfigService::loadLockBits: load() failed\r\n");
            return;
        }
        if (storage_haveSavedLockBits()) {
            updateLockBits(persistentData.lockBits);
        }
    }

    /* Dummy callback handler needed by Nordic's pstorage module. */
    static void pstorageNotificationCallback(pstorage_handle_t *p_handle,
                                             uint8_t            op_code,
                                             uint32_t           result,
                                             uint8_t *          p_data,
                                             uint32_t           data_len) {
        /* APP_ERROR_CHECK(result); */
    }

/* private state related to operation on nRF micros */
private:
    pstorage_handle_t pstorageHandle;
    PersistentData_t  persistentData;
};

#endif /* #ifndef __NRF_BLE_URI_BEACON_CONFIG_SERVICE_H__*/
