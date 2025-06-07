#ifndef __TEMP_HUMIDITY_SENSOR_H__
#define __TEMP_HUMIDITY_SENSOR_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
namespace temp {
#endif

    void init();
    void read();

    bool dht22_read(volatile uint8_t* data);
    void advanceStateMachine(bool value, uint32_t us);

#ifdef __cplusplus

} /* namespace temp */

#endif

#endif /* __TEMP_HUMIDITY_SENSOR_H__ */

