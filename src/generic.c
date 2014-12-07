#include "controller.h"
#include <stdio.h>

#define CHANNEL_THROTTLE 0
#define CHANNEL_ATTITUDE_OFFSET 1

#define THROTTLE_MIN 500
#define THROTTLE_MAX 2000
#define THROTTLE_STEP_SMALL 10
#define THROTTLE_STEP_LARGE 100

#define ATTITUDE_MIN 1000
#define ATTITUDE_MAX 2000

/* 
 * convert the value from an axis to an appropriate ppm value
 * between 1000 and 2000
 */
inline channel_t axis_to_attitude(int value) {
	long centered = (long) value - AXIS_MIN;
	long proportional = centered * ATTITUDE_MIN / (AXIS_MAX - AXIS_MIN);
	return (channel_t) ATTITUDE_MAX - ATTITUDE_MIN + proportional;
}

void channel_reset(channel_t* channels) {
	for (size_t i = 0; i < CHANNELS; ++i) {
		channels[i] = CHANNEL_UNUSED;
	}
	channels[0] = THROTTLE_MIN;
	channels[1] = ATTITUDE_MIN;
	channels[2] = ATTITUDE_MIN;
	channels[3] = ATTITUDE_MIN;
	channels[4] = ATTITUDE_MIN;
}

void event_axis(channel_t* channels, int axis, int value) {
	channels[CHANNEL_ATTITUDE_OFFSET+axis] = axis_to_attitude(value);
}

void event_button(channel_t* channels, int button, bool value) {
	if (value) {
		switch(button) {
		case 0:
			channel_reset(channels);
			break;
		case 4:
			channel_step(channels, CHANNEL_THROTTLE, -THROTTLE_STEP_SMALL, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 5:
			channel_step(channels, CHANNEL_THROTTLE, THROTTLE_STEP_SMALL, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 6:
			channel_step(channels, CHANNEL_THROTTLE, -THROTTLE_STEP_LARGE, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 7:
			channel_step(channels, CHANNEL_THROTTLE, THROTTLE_STEP_LARGE, THROTTLE_MIN, THROTTLE_MAX);
			break;
		default:
			printf("button unsupported %d\n", button);
			break;
		}
	}
}