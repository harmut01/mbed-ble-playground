//
// Created by Harrison Mutai on 13/10/2020.
//

#include "bleDemo.h"
#include <mbed.h>
#include <events/mbed_events.h>
//#include "pretty_printer.h"

// setup functions
void bleDemo::start(){
    _ble.gap().setEventHandler(this);
    _ble.init(this, &bleDemo::on_init_complete);

    _event_queue.call_every(500, this, &bleDemo::blink);
    _event_queue.call_every(1000, this, &bleDemo::update_sensor_value);

    _event_queue.dispatch_forever();
}

/** Callback triggered when the ble initialization process has finished */
void bleDemo::on_init_complete(BLE::InitializationCompleteCallbackContext *params) {
    if (params->error != BLE_ERROR_NONE) {
        printf("Ble initialization failed.");
        return;
    }

    //print_mac_address();

    start_advertising();
}

/* Event handlers */
void bleDemo::onDisconnectionComplete(const ble::DisconnectionCompleteEvent&) {
    _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);
    _connected = false;
}

// transmission
void bleDemo::start_advertising() {
    /* Create advertising parameters and payload */

    ble::AdvertisingParameters adv_parameters(
            ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
            ble::adv_interval_t(ble::millisecond_t(1000))
    );

    _adv_data_builder.setFlags();
    _adv_data_builder.setAppearance(ble::adv_data_appearance_t::GENERIC_HEART_RATE_SENSOR);
    _adv_data_builder.setLocalServiceList(mbed::make_Span(&_hr_uuid, 1));
    _adv_data_builder.setName(DEVICE_NAME);

    /* Setup advertising */

    ble_error_t error = _ble.gap().setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            adv_parameters
    );

    if (error) {
        printf("_ble.gap().setAdvertisingParameters() failed\r\n");
        return;
    }


    error = _ble.gap().setAdvertisingPayload(
            ble::LEGACY_ADVERTISING_HANDLE,
            _adv_data_builder.getAdvertisingData()
    );

    if (error) {
        printf("_ble.gap().setAdvertisingPayload() failed\r\n");
        return;
    }

    /* Start advertising */

    error = _ble.gap().startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error) {
        printf("_ble.gap().startAdvertising() failed\r\n");
        return;
    }
}

/* Heart Rate data handlers */
void bleDemo::update_sensor_value() {
    if (_connected) {
        // Do blocking calls or whatever is necessary for sensor polling.
        // In our case, we simply update the HRM measurement.
        _hr_counter++;

        //  100 <= HRM bps <=175
        if (_hr_counter == 175) {
            _hr_counter = 100;
        }

        _hr_service.updateHeartRate(_hr_counter);
    }
}




