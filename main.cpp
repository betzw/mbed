/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "BLEDevice.h"
#include "URIBeaconConfigService.h"
#include "DFUService.h"
#include "DeviceInformationService.h"
#include "ConfigParamsPersistence.h"

BLEDevice ble;
URIBeaconConfigService *uriBeaconConfig;

void disconnectionCallback(Gap::Handle_t handle, Gap::DisconnectionReason_t reason)
{
    ble.startAdvertising();
}

int main(void)
{
    ble.init();
    ble.onDisconnection(disconnectionCallback);

    /*
     * Load parameters from (platform specific) persistent storage. Parameters
     * can be set to non-default values while the URIBeacon is in configuration
     * mode (within the first 60 seconds of power-up). Thereafter, parameters
     * get copied out to persistent storage before switching to normal URIBeacon
     * operation.
     */
    URIBeaconConfigService::Params_t params;
    loadURIBeaconConfigParams(&params);

    /* Initialize a URIBeaconConfig service providing config params, default URI, and power levels. */
    static URIBeaconConfigService::PowerLevels_t defaultAdvPowerLevels = {-20, -4, 0, 10}; // Values for ADV packets related to firmware levels
    uriBeaconConfig = new URIBeaconConfigService(ble, params, "http://uribeacon.org", defaultAdvPowerLevels);
    if (!uriBeaconConfig->configuredSuccessfully()) {
        error("failed to accommodate URI");
    }

    // Setup auxiliary services to allow over-the-air firmware updates, etc
    DFUService dfu(ble);
    DeviceInformationService deviceInfo(ble, "ARM", "UriBeacon", "SN1", "hw-rev1", "fw-rev1", "soft-rev1");

    ble.startAdvertising(); /* Set the whole thing in motion. After this call a GAP central can scan the URIBeaconConfig
                             * service, which then automatically switches to the URIBeacon after a timeout. */

    while (true) {
        ble.waitForEvent();
    }
}
