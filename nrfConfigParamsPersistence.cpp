/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#include "pstorage.h"
#include "nrf_error.h"
#include "ConfigParamsPersistence.h"

static pstorage_handle_t pstorageHandle;

/* Dummy callback handler needed by Nordic's pstorage module. */
static void pstorageNotificationCallback(pstorage_handle_t *p_handle,
                                  uint8_t            op_code,
                                  uint32_t           result,
                                  uint8_t *          p_data,
                                  uint32_t           data_len)
{
    /* APP_ERROR_CHECK(result); */
}

/* Platform-specific implementation for persistence on the nRF5x. Based on the
 * pstorage module provided by the Nordic SDK. */
void loadURIBeaconConfigParams(URIBeaconConfigService::Params_t *paramsP)
{
    pstorage_init();
    static pstorage_module_param_t pstorageParams = {
        .cb          = pstorageNotificationCallback,
        .block_size  = sizeof(URIBeaconConfigService::Params_t),
        .block_count = 1
    };

    pstorage_register(&pstorageParams, &pstorageHandle);
    if (pstorage_load(reinterpret_cast<uint8_t *>(paramsP), &pstorageHandle, sizeof(URIBeaconConfigService::Params_t), 0) != NRF_SUCCESS) {
        // On failure zero out and let the service reset to defaults
        memset(paramsP, 0, sizeof(URIBeaconConfigService::Params_t));
    }
}

/* Platform-specific implementation for persistence on the nRF5x. Based on the
 * pstorage module provided by the Nordic SDK. */
void saveURIBeaconConfigParams(URIBeaconConfigService::Params_t *paramsP)
{
    if (paramsP->persistenceSignature != URIBeaconConfigService::Params_t::MAGIC) {
        paramsP->persistenceSignature = URIBeaconConfigService::Params_t::MAGIC;
        pstorage_store(&pstorageHandle,
                       reinterpret_cast<uint8_t *>(paramsP),
                       sizeof(URIBeaconConfigService::Params_t),
                       0 /* offset */);
    } else {
        pstorage_update(&pstorageHandle,
                        reinterpret_cast<uint8_t *>(paramsP),
                        sizeof(URIBeaconConfigService::Params_t),
                        0 /* offset */);
    }
}
