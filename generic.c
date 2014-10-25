#include "controller.h"

#define CHANNEL_THROTTLE 0

#define THROTTLE_MIN 12
#define THROTTLE_MAX 255

void channel_reset(channel_t* channels) {
	channels[0] = THROTTLE_MIN;
	channels[1] = 127;
	channels[2] = 127;
	channels[3] = 127;
	channels[4] = 127;
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
			channel_decrease(channels, CHANNEL_THROTTLE, 1, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 5:
			channel_increase(channels, CHANNEL_THROTTLE, 1, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 6:
			channel_decrease(channels, CHANNEL_THROTTLE, 10, THROTTLE_MIN, THROTTLE_MAX);
			break;
		case 7:
			channel_increase(channels, CHANNEL_THROTTLE, 10, THROTTLE_MIN, THROTTLE_MAX);
			break;
		default:
			break;
		}

	}
}