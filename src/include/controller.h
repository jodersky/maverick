#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define AXIS_MIN -32768
#define AXIS_MAX 32767

#define CHANNEL_UNUSED 65535
#define CHANNELS 8

typedef uint16_t channel_t;

inline channel_t channel_clamp(channel_t channel, channel_t min, channel_t max) {
    channel_t upper = (channel < max) ? channel : max;
    return (min < upper) ? upper : min;
}

inline void channel_step(channel_t* channels, size_t channel_index, char step, channel_t min, channel_t max) {
    int current = (int) channels[channel_index];
    int next = current + step;
    if (next > (int) max) {
    	next = max;
    }
    if (next < (int) min) {
    	next = min;
    }
    channels[channel_index] = (channel_t) next;
}

void channel_reset(channel_t* channels);
void event_axis(channel_t* channels, int axis, int value);
void event_button(channel_t* channels, int button, bool value);


#endif