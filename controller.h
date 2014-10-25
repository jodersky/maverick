#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include <stddef.h>

#define AXIS_MIN -32768
#define AXIS_MAX 32767
#define CHANNELS 5

typedef unsigned char channel_t;

inline channel_t axis_to_channel(int value) {
	long centered = (long) value - AXIS_MIN;
	return (channel_t) (centered * 255 / (AXIS_MAX - AXIS_MIN));
}

inline channel_t channel_clamp(channel_t channel, channel_t min, channel_t max) {
    channel_t upper = (channel < max) ? channel : max;
    return (min < upper) ? upper : min;
}

inline void channel_increase(channel_t* channels, size_t channel_index, channel_t step, channel_t min, channel_t max) {
    channel_t current = channels[channel_index];
    channel_t next = channel_clamp(current + step, min, max);
    channels[channel_index] = next;
}

inline void channel_decrease(channel_t* channels, size_t channel_index, channel_t step, channel_t min, channel_t max) {
	channel_t current = channels[channel_index];
    channel_t next = channel_clamp(current - step, min, max);
    channels[channel_index] = next;
}

void channel_reset(channel_t* channels);
void event_axis(channel_t* channels, int axis, int value);
void event_button(channel_t* channels, int button, bool value);


#endif