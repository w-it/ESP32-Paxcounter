#include "globals.h"
#include "energyhousekeeping.h"

// Local logging tag
static const char TAG[] = __FILE__;

TaskHandle_t NrgyHouseKeepingTask;


void nrgy_loop(void *pvParameters){
    while(1){
        ESP_LOGI(TAG, "NRGY HOUSE KEEPING");

#if (HAS_LORA)
        ESP_LOGI(TAG, "Send status");
        payload.reset();
        payload.addStatus(read_voltage(), uptime() / 1000, temperatureRead(),
                    getFreeRAM(), rtc_get_reset_reason(0),
                    rtc_get_reset_reason(1));
         SendPayload(STATUSPORT, prio_high);
#endif
#if (HAS_GPS)
          //enable GPS 
          ESP_LOGI(TAG, "GPS enabled");
          setGpsOn();
#endif
        delay(3 * 60 * 1000);
    }
}
