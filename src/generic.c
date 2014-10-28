#include "controller.h"
#include <stdio.h>

#define CHANNEL_THROTTLE 0

#define THROTTLE_MIN 1000
#define THROTTLE_MAX 2000

//convert the value from an axis to an appropriate ppm value
//between 1000 and 2000
inline channel_t axis_to_channel(int value) {
	long centered = (long) value - AXIS_MIN;
	long proportional = centered * 1000 / (AXIS_MAX - AXIS_MIN);
	return (channel_t) 1000 + proportional;
}

void channel_reset(channel_t* channels) {
	for (size_t i = 0; i < CHANNELS; ++i) {
		channels[i] = CHANNEL_UNUSED;
	}
	channels[0] = THROTTLE_MIN;
	channels[1] = 1500;
	channels[2] = 1500;
	channels[3] = 1500;
	channels[4] = 1500;
}
void event_axis(channel_t* channels, int axis, int value) {
	channels[axis+1] = axis_to_channel(value);
}
void event_button(channel_t* channels, int button, bool value) {
	if (value) {
		switch(button) {
		case 0:
			channel_reset(channels);
			break;
		case 4:
			channel_step(channels, CHANNEL_THROTTLE, -10, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 5:
			channel_step(channels, CHANNEL_THROTTLE, 10, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 6:
			channel_step(channels, CHANNEL_THROTTLE, -100, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 7:
			channel_step(channels, CHANNEL_THROTTLE, 100, THROTTLE_MIN, THROTTLE_MAX);
			break;
		default:
			break;
		}
	}
}