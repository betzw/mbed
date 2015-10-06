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
 
#include "mbed.h"
#include "BLE.h"
#include "DiscoveredCharacteristic.h"
#include "DiscoveredService.h"

#include "BlueNRGGap.h"

#include "Samples.h"
#include "Utils.h"

#ifdef LED_DEMO_CENTRAL_ENABLED

#define SCAN_INT  0x30 // 30 ms = 48 * 0.625 ms
#define SCAN_WIND 0x30 // 30 ms = 48 * 0.625 ms

BLE ble;
DigitalOut led1(LED1);
 
const Gap::Address_t  BLE_address_BE       = {0xCC, 0x00, 0x00, 0xE1, 0x80, 0x02};
const Gap::Address_t  BLE_peer_address_BE  = {0xFD, 0x00, 0x25, 0xAA, 0x02, 0x04};
bool triggerLedCharacteristic = false;
DiscoveredCharacteristic ledCharacteristic;

uint8_t toggledValue = 0;
enum {
  READ = 0,
  WRITE,
  IDLE
};
static volatile unsigned int triggerOp = IDLE;

void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    printf("disconnected\n\r");
}

void advertisementCallback(const Gap::AdvertisementCallbackParams_t *params) {
    if (params->peerAddr[0] != BLE_peer_address_BE[0]) { /* !ALERT! Alter this filter to suit your device. */
        //DEBUG("advertisementCallback returning params->peerAddr[0] != 0xFD\r\n");
        return;
    }
    
    DEBUG("adv peerAddr[%02x %02x %02x %02x %02x %02x] rssi %d, isScanResponse %u, AdvertisementType %u\r\n",
           params->peerAddr[5], params->peerAddr[4], params->peerAddr[3], params->peerAddr[2], params->peerAddr[1], params->peerAddr[0],
           params->rssi, params->isScanResponse, params->type);
    
    if(!params->isScanResponse) {
      ble.gap().connect(params->peerAddr, Gap::ADDR_TYPE_PUBLIC, NULL, NULL);
    }
}

void discoveryTerminationCallback(Gap::Handle_t connectionHandle) {
    printf("terminated SD for handle %u\r\n", connectionHandle);
}

void serviceDiscoveryCallback(const DiscoveredService *service) {
    //DEBUG("serviceDiscoveryCallback\n\r");
    if (service->getUUID().shortOrLong() == UUID::UUID_TYPE_SHORT) {
        DEBUG("S UUID-%x attrs[%u %u]\r\n", service->getUUID().getShortUUID(), service->getStartHandle(), service->getEndHandle());
    } else {
        DEBUG("S UUID-");
        const uint8_t *longUUIDBytes = service->getUUID().getBaseUUID();
        for (unsigned i = 0; i < UUID::LENGTH_OF_LONG_UUID; i++) {
            DEBUG("%02X", longUUIDBytes[i]);
        }
        DEBUG(" attrs[%u %u]\r\n", service->getStartHandle(), service->getEndHandle());
    }
}
 
void characteristicDiscoveryCallback(const DiscoveredCharacteristic *characteristicP) {
    //DEBUG("  C UUID-%x valueAttr[%u] props[%x]\r\n", characteristicP->getShortUUID(), characteristicP->getValueHandle(), (uint8_t)characteristicP->getProperties().broadcast());
    if (characteristicP->getUUID().getShortUUID() == 0xa001) { /* !ALERT! Alter this filter to suit your device. */
      //DEBUG("  C UUID-%x valueAttr[%u] props[%x]\r\n", characteristicP->getShortUUID(), characteristicP->getValueHandle(), (uint8_t)characteristicP->getProperties().broadcast());
      ledCharacteristic        = *characteristicP;
      //triggerLedCharacteristic = true;
      triggerOp = READ;
    }
}

void connectionCallback(const Gap::ConnectionCallbackParams_t *params) {
  DEBUG("connectionCallback params->role=%d\n\r", params->role);

  uint16_t LED_SERVICE_UUID = 0xA000;
  uint16_t LED_STATE_CHARACTERISTIC_UUID = 0xA001;
  
  if (params->role == Gap::CENTRAL) {
    ble.gattClient().onServiceDiscoveryTermination(discoveryTerminationCallback);
    ble.gattClient().launchServiceDiscovery(params->handle, serviceDiscoveryCallback, characteristicDiscoveryCallback, LED_SERVICE_UUID, LED_STATE_CHARACTERISTIC_UUID);
  }
}

void triggerToggledWrite(const GattReadCallbackParams *response) {
  
  DEBUG("!!!triggerToggledWrite!!!\n\r");

  if (response->handle == ledCharacteristic.getValueHandle()) {
#if 0
    DEBUG("triggerToggledWrite: handle %u, offset %u, len %u\r\n", response->handle, response->offset, response->len);
    for (unsigned index = 0; index < response->len; index++) {
      DEBUG("%c[%02x]", response->data[index], response->data[index]);
    }
    DEBUG("\r\n");
#endif
    
    toggledValue = response->data[0] ^ 0x1;
    DEBUG("!!!triggerToggledWrite (toggledValue=%d)!!!\n\r", toggledValue);
    //ledCharacteristic.write(1, &toggledValue);
    triggerOp = WRITE;
  }
}

void triggerRead(const GattWriteCallbackParams *response) {
  DEBUG("!!!triggerRead!!!\n\r");

  if (response->handle == ledCharacteristic.getValueHandle()) {
    triggerOp = READ;
    //ledCharacteristic.read();
  }
}

void ledDemoCentral(void)
{
    led1 = 0;
    
    // Set BT Address
    ble.gap().setAddress(Gap::ADDR_TYPE_PUBLIC, BLE_address_BE);

    // Init the BLE dev
    ble.init();
    ble.gap().onConnection(connectionCallback);
    ble.onDisconnection(disconnectionCallback);
       
    ble.gattClient().onDataRead(triggerToggledWrite);
    ble.gattClient().onDataWrite(triggerRead);

    ble.gap().setScanParams(SCAN_INT, SCAN_WIND);
    ble.gap().startScan(advertisementCallback);
 
    // infinite loop
    while (1) {
      if (/*triggerLedCharacteristic && */!ble.gattClient().isServiceDiscoveryActive()) {
        //triggerLedCharacteristic = false;
        /*ledCharacteristic.read();*/ /* We could have issued this read just as easily from
        * characteristicDiscoveryCallback(); but
        * invoking it here demonstrates the use
        * of isServiceDiscoveryActive() and also
        * the fact that it is permitted to
        * operate on application-local copies of
        * DiscoveredCharacteristic. */
        switch(triggerOp) {
        case READ:
          triggerOp = IDLE;
          ledCharacteristic.read();
          break;
        case WRITE:
          triggerOp = IDLE;
          ledCharacteristic.write(1, &toggledValue);
          break;
        }
      }
      ble.waitForEvent();
    }
}
 
#endif /* SAMPLE_DEMO_CENTRAL_ENABLED */