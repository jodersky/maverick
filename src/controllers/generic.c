#include "generic.h"
#include <stdio.h>

#define CHANNEL_THROTTLE 0
#define CHANNEL_ATTITUDE_OFFSET 1

#define THROTTLE_MIN 1000
#define THROTTLE_MAX 2000
#define THROTTLE_STEP_SMALL 10
#define THROTTLE_STEP_LARGE 100

#define ATTITUDE_MIN 1000
#define ATTITUDE_MAX 2000

/* 
 * convert the value from an axis to an appropriate ppm value
 * between 1000 and 2000
 */
inline channel_t axis_to_attitude(int value)
{
	long centered = (long) value - AXIS_MIN;
	long proportional = centered * ATTITUDE_MIN / (AXIS_MAX - AXIS_MIN);
	return (channel_t) ATTITUDE_MAX - ATTITUDE_MIN + proportional;
}

inline void channel_step(channel_t* channels, size_t channel_index, char step, channel_t min, channel_t max)
{
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

static void reset(struct controller* controller)
{
	channel_t *channels = controller->channels;
	
	for (size_t i = 0; i < CHANNELS; ++i) {
		channels[i] = CHANNEL_UNUSED;
	}
	
	channels[0] = THROTTLE_MIN;
	channels[1] = ATTITUDE_MIN;
	channels[2] = ATTITUDE_MIN;
	channels[3] = ATTITUDE_MIN;
	channels[4] = ATTITUDE_MIN;
}

static void axis(struct controller* controller, int axis_id, int value)
{
	controller->channels[CHANNEL_ATTITUDE_OFFSET+axis_id] = axis_to_attitude(value);
}

static void button(struct controller* controller, int button_id, bool down)
{
	if (!down){
		return;
	}
	
	switch(button_id) {
	case 0:
		reset(controller);
		break;
	case 4:
		channel_step(controller->channels, CHANNEL_THROTTLE, -THROTTLE_STEP_SMALL, THROTTLE_MIN, THROTTLE_MAX);
		break;
	case 5:
		channel_step(controller->channels, CHANNEL_THROTTLE, THROTTLE_STEP_SMALL, THROTTLE_MIN, THROTTLE_MAX);
		break;
	case 6:
		channel_step(controller->channels, CHANNEL_THROTTLE, -THROTTLE_STEP_LARGE, THROTTLE_MIN, THROTTLE_MAX);
		break;
	case 7:
		channel_step(controller->channels, CHANNEL_THROTTLE, THROTTLE_STEP_LARGE, THROTTLE_MIN, THROTTLE_MAX);
		break;
	default:
		fprintf(stderr, "Button number %d unsupported.\n", button_id);
		break;
	}
}

struct controller controller_generic = {
	.reset = reset,
	.axis = axis,
	.button = button
};
