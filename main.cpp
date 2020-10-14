/*
 * Created by Harrison Mutai on 12/10/2020.
 * */
#include "bleDemo.h"

using namespace ble;
const char DEVICE_NAME[] = "Heartrate";
static events::EventQueue event_queue(/* event count */ 16 * EVENTS_EVENT_SIZE);

int main()
{
    BLE &disco_ble_handler = BLE::Instance();
    disco_ble_handler.onEventsToProcess(schedule_ble_events);


    bleDemo app_handler(disco_ble_handler, event_queue);
    app_handler.start();

    return 0;
}


/* Schedule processing of events from the BLE middleware in the event queue. */
void schedule_ble_events(BLE::OnEventsToProcessCallbackContext *context) {
    event_queue.call(Callback<void()>(&context->ble, &BLE::processEvents));
};
