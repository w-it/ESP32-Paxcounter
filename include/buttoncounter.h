#ifndef _BUTTONCOUNTER_H
#define _BUTTONCOUNTER_H

#include <SimpleButton.h>
#include "irqhandler.h"

void buttoncounter_init(int pin);
void readButtonCounter();

#endif