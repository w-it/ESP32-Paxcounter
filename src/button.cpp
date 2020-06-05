#ifdef HAS_BUTTON

#include "globals.h"
#include "button.h"

using namespace simplebutton;

// Local logging tag
static const char TAG[] = __FILE__;

static Button *b = NULL;

void button_init(int pin) {
#ifdef BUTTON_PULLDOWN
  b = new Button(pin);
#else
  b = new ButtonPullup(pin);
#endif

  // attach events to the button

  b->setOnDoubleClicked([]() {});

  b->setOnClicked([]() {
    ESP_LOGI("button clicked");

#ifdef HAS_DISPLAY
    dp_refresh(true); // switch to next display page
#endif
#ifdef HAS_MATRIX_DISPLAY
    refreshTheMatrixDisplay(true); // switch to next display page
#endif
  });

  b->setOnHolding([]() {
    ESP_LOGI("button long press");

    payload.reset();
    payload.addButton(0x01);
    SendPayload(BUTTONPORT, prio_normal);
  });

  // attach interrupt to the button
  attachInterrupt(digitalPinToInterrupt(pin), ButtonIRQ, CHANGE);
}

void readButton() { b->update(); }
#endif