#include "globals.h"
#include "buttoncounter.h"

using namespace simplebutton;

// Local logging tag
static const char TAG[] = __FILE__;

static Button *b = NULL;

static unsigned long lastRegistered = 0;


void buttoncounter_init(int pin) {

#ifdef BUTTON_PULLDOWN
  b = new Button(pin);
#else
  b = new ButtonPullup(pin);
#endif

  // attach events to the button

  b->setOnDoubleClicked([]() {
    // ESP_LOGI(TAG,"button double clicked");
  });

  b->setOnClicked([]() {
   // ESP_LOGI(TAG,"button clicked");
  });

  b->setOnHolding([]() {
   // ESP_LOGI(TAG,"button long press");
  });
  b->setOnPushed([]() {
    ESP_LOGI(TAG,"button pushed");
    unsigned long now = millis();
    if(now - lastRegistered >  cfg.buttoninterval * 1000){
      nbr_start ++;
      lastRegistered = now;
      ESP_LOGI(TAG,"button pushed: registered");
    }
  });
  b->setOnReleased([]() {
   // ESP_LOGI(TAG,"button released");
  });

  // attach interrupt to the button
  attachInterrupt(digitalPinToInterrupt(pin), ButtonCounterIRQ, CHANGE);
}

void readButtonCounter() { b->update(); }
