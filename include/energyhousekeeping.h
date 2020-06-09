#ifndef _NRGYHOUSEKEEPING_H
#define _NRGYHOUSEKEEPING_H

#include "timekeeper.h"

#include "lorawan.h"



extern TaskHandle_t NrgyHouseKeepingTask;

void nrgy_loop(void *pvParameters);

#endif