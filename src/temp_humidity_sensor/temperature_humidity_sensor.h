#ifndef __TEMP_HUMIDITY_SENSOR_H__
#define __TEMP_HUMIDITY_SENSOR_H__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
namespace temp {
#endif
    struct hum_temp_reading {
        float Humidity;
        float Temperature;
    };

    void init();
    hum_temp_reading read();

    void advanceStateMachine(bool Value, uint32_t Us);

#ifdef __cplusplus

} /* namespace temp */

#endif

#endif /* __TEMP_HUMIDITY_SENSOR_H__ */

