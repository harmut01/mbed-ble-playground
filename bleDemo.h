//
// Created by Harrison Mutai on 13/10/2020.
//

#ifndef MBED_BLINKY_APP_BLEDEMO_H
#define MBED_BLINKY_APP_BLEDEMO_H

#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include <events/mbed_events.h>
#include "ble/services/HeartRateService.h"

extern const char DEVICE_NAME[];

class bleDemo : ble::Gap::EventHandler {
private:
    BLE &_ble;
    events::EventQueue &_event_queue;
    DigitalOut _led1;

    bool _connected;

    uint8_t _hr_counter;
    HeartRateService _hr_service;
    UUID _hr_uuid;

    uint8_t _adv_buffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder _adv_data_builder;

public:
    bleDemo(BLE &ble, events::EventQueue &event_queue) :
        _ble(ble),
        _event_queue(event_queue),
        _led1(LED1, 1),
        _connected(false),
        _hr_uuid(GattService::UUID_HEART_RATE_SERVICE),
        _hr_counter(100),
        _hr_service(ble, _hr_counter, HeartRateService::LOCATION_FINGER),
        _adv_data_builder(_adv_buffer) { }
    void start();

private:
    /** Callback triggered when the ble initialization process has finished */
    void on_init_complete(BLE::InitializationCompleteCallbackContext *params);
    void start_advertising();
    void update_sensor_value();
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent&);
    virtual void onConnectionComplete(const ble::ConnectionCompleteEvent &event) {
        if (event.getStatus() == BLE_ERROR_NONE) {
            _connected = true;
        }
    }
    void blink(void) {
           _led1 = !_led1;
    }
};

void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context);


#endif //MBED_BLINKY_APP_BLEDEMO_H
