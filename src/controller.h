#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CHANNEL_UNUSED 65535
#define CHANNELS 8

#define AXIS_MIN -32768
#define AXIS_MAX 32767

typedef uint16_t channel_t;

struct controller {
	void (*reset)(struct controller* cont);
	void (*axis)(struct controller* cont, int axis_id, int value);
	void (*button)(struct controller* cont, int button_id, bool down);
	channel_t channels[CHANNELS];
};

inline channel_t channel_clamp(channel_t channel, channel_t min, channel_t max)
{
    channel_t upper = (channel < max) ? channel : max;
    return (min < upper) ? upper : min;
}

#endif
