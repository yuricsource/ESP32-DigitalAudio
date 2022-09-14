#pragma once

#include "freertos/FreeRTOS.h"

#define configMENUSVC_STACK_DEPTH (1024 * 3)
#define configAISVC_STACK_DEPTH (1024 * 2)
#define configINPUTSVC_STACK_DEPTH (1024 * 2)
#define configAUDIOSVC_STACK_DEPTH (1024 * 2)

#define configTOTAL_PROJECT_HEAP_SIZE_ALLOCATED (   configMENUSVC_STACK_DEPTH \
                                                    + configAISVC_STACK_DEPTH \
                                                    + configINPUTSVC_STACK_DEPTH \
                                                    + configAUDIOSVC_STACK_DEPTH)