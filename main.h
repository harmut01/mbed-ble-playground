//
// Created by Harrison Mutai on 12/10/2020.
//

#ifndef MBED_BLINKY_APP_MAIN_H
#define MBED_BLINKY_APP_MAIN_H

void led_toggle(led led);
void ble_connected(BLE ble_handler);
void ble_disconnected(BLE ble_handler);
#define BLINKING_RATE 500

#endif //MBED_BLINKY_APP_MAIN_H
