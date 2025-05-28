#ifndef __TEMPERATURE_SENSOR_TIMER_H__
#define __TEMPERATURE_SENSOR_TIMER_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int MX_TIMER_Init(void);
void sleepUs(int us);

uint32_t getCurrentUsTick(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEMPERATURE_SENSOR_TIMER_H__ */

