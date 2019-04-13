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

+------------------+
|                  |
|                  |   +--------+
|             P0.6 +---+ Switch +---------------+
|                  |   +--------+               |
|                  |                            |
|    nRF51822      |                          +-+-+ GND
|                  |
|                  |   +-------+   +-------+
|             P0.18+---+  LED  +---+   R   +----+
|                  |   +-------+   +-------+    |
|                  |                            |
+------------------+                          +-+-+ GND

 */
 
#include "mbed.h"
#include "ble/BLE.h"

DigitalOut    led(P0_18);
InterruptIn   button(P0_6);         // button to gnd
Timer         stopwatch;            // for button debouncing
volatile bool debouncing       = 0; // is high when button_isr is called


/* Optional: Device Name, add for human read-ability */
// 190223 andy - const static char     DEVICE_NAME[] = "ChangeMe!!";
const static char     DEVICE_NAME[] = "button_counter";

/* You have up to 26 bytes of advertising data to use. */
//const static uint8_t AdvData[] = {0x01,0x02,0x03,0x04,0x05};   /* Example of hex data */
union AdvData_u{
  uint8_t  u8[4];
  uint32_t u32;   // unify to 32 bit number to increment all 4 bytes
}AdvData;


//static uint8_t AdvData[] = {0x01,0x02,0x03,0x04,0x05};   /* Example of hex data */
//const static uint8_t AdvData[] = {"ChangeThisData"};         /* Example of character data */


/* Optional: Restart advertising when peer disconnects */
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *params)
{
    BLE::Instance().gap().startAdvertising();
}
/**
 * This function is called when the ble initialization process has failed
 */
void onBleInitError(BLE &ble, ble_error_t error)
{
    /* Avoid compiler warnings */
    (void) ble;
    (void) error;
   
    /* Initialization error handling should go here */
}   

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
    BLE&        ble   = params->ble;
    ble_error_t error = params->error;

    if (error != BLE_ERROR_NONE) {
        /* In case of error, forward the error handling to onBleInitError */
        onBleInitError(ble, error);
        return;
    }

    /* Ensure that it is the default instance of BLE */
    if(ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
        return;
    }
   
    /* Set device name characteristic data */
    ble.gap().setDeviceName((const uint8_t *) DEVICE_NAME);

    /* Optional: add callback for disconnection */
    ble.gap().onDisconnection(disconnectionCallback);

    /* Sacrifice 3B of 31B to Advertising Flags */
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE );
    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);

    /* Sacrifice 2B of 31B to AdvType overhead, rest goes to AdvData array you define */
    // 190407 andy - ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, AdvData, sizeof(AdvData));
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, AdvData.u8, sizeof(AdvData.u8));

    /* Optional: Add name to device */
    // 190223 andy +
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME));

    /* Set advertising interval. Longer interval == longer battery life */
    ble.gap().setAdvertisingInterval(3000); /* 3000ms */

    /* Start advertising */
    ble.gap().startAdvertising();
}


void button_isr(){
  debouncing = 1;
  stopwatch.reset();
}


void flash_led(uint8_t n){
  for(int i=0;i<n;i++){
    led = 1;
    wait_ms(100);
    led = 0;
    wait_ms(100);
  }
}
   


int main(void) {
  AdvData.u32 = 0;

  button.mode(PullUp);
  button.fall(button_isr);

  stopwatch.start();

  BLE& ble = BLE::Instance(BLE::DEFAULT_INSTANCE);

  ble.init(bleInitComplete); /* Initialize BLE baselayer, always do this first! */

  while(1){
    if(debouncing && stopwatch.read_ms() > 100){
      debouncing = 0;

      /* Update advertising data */
      ble.gap().stopAdvertising();
      AdvData.u32++;
      ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::MANUFACTURER_SPECIFIC_DATA, AdvData.u8, sizeof(AdvData.u8));
      ble.gap().startAdvertising();

      flash_led(1);
    }
    ble.waitForEvent(); /* Save power while waiting for callback events */
  }
}
