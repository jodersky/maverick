#include <stdio.h>
#include <unistd.h>
#include <SDL.h>
#include "mavlink/common/mavlink.h"
#include "controller.h"

struct conf {

    //the joystick id to use by sdl
    int joystick_id;

    //maximum delay before the controller's state is printed
    int timeout;

    //mav system id of local device
    int mav_local_system;

    //mav component id of local device
    int mav_local_component;

    //mav system id of target device
    int mav_target_system;

};

static int run(const struct conf* conf);
static void mav_out(uint8_t system_id, uint8_t component_id, uint8_t rsystem_id, channel_t* channels);


int main(int argc, char *argv[]) {
    
    //initialize joystick library
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Couldn't initialize SDL joystick subsystem: %s\n", SDL_GetError());
        exit(1);
    }

    struct conf conf = {
        .joystick_id = 0,
        .timeout = 500,
        .mav_local_system = 42,
        .mav_local_component = 22,
        .mav_target_system = 0
    };

    return run(&conf);
}

static int run(const struct conf* conf) {

    channel_t channels[CHANNELS];
    channel_reset(channels);

    int joystick_id = conf->joystick_id;

    //pointer to joystick in use
    SDL_Joystick* joystick = NULL;

    //event loop
    SDL_Event e;
    bool cont = true;
    while (cont) {

        if(SDL_WaitEventTimeout(&e, conf->timeout)) {
            switch( e.type ) {

            case SDL_JOYDEVICEADDED:
                if (e.jdevice.which == joystick_id) {
                    fprintf(stderr, "Joystick %d connected\n", e.jdevice.which);
                    joystick=SDL_JoystickOpen(e.jdevice.which);
                    if(joystick) {
                        fprintf(stderr, "Opened joystick %d\n", e.jdevice.which);
                        fprintf(stderr, "Name: %s\n", SDL_JoystickName(joystick));
                        fprintf(stderr, "Number of axes: %d\n", SDL_JoystickNumAxes(joystick));
                        fprintf(stderr, "Number of buttons: %d\n", SDL_JoystickNumButtons(joystick));
                        fprintf(stderr, "Number of balls: %d\n", SDL_JoystickNumBalls(joystick));
                    } else {
                        fprintf(stderr, "Couldn't open joystick %d\n", e.jdevice.which);
                    }
                }
                break;

            case SDL_JOYDEVICEREMOVED:
                if (e.jdevice.which == joystick_id) {
                    if (SDL_JoystickGetAttached(joystick)) {
                        SDL_JoystickClose(joystick);
                    }
                    fprintf(stderr, "Joystick %d disconnected\n", e.jdevice.which);
                }
                break;

            case SDL_JOYAXISMOTION:
                event_axis(channels, e.jaxis.axis, e.jaxis.value);
                break;

            case SDL_JOYBUTTONDOWN:
                event_button(channels, e.jbutton.button, true);
                break;
                
            case SDL_JOYBUTTONUP:
                event_button(channels, e.jbutton.button, false);
                break;

            case SDL_QUIT:
                cont = false;
                break;

            default:
                fprintf(stderr,"Unsupported event type: %d\n", e.type);
                break;
            }
            
        }
        mav_out(
            conf->mav_local_system,
            conf->mav_local_component,
            conf->mav_target_system,
            channels
        );
    }
    return 0;
}

static void mav_out(uint8_t system_id, uint8_t component_id, uint8_t rsystem_id, channel_t* channels) {

    mavlink_message_t msg;
    uint8_t buf[MAVLINK_MAX_PACKET_LEN];

    mavlink_msg_rc_channels_override_pack(
        system_id,
        component_id,
        &msg,
        rsystem_id,
        MAV_COMP_ID_ALL,
        channels[0],
        channels[1],
        channels[2],
        channels[3],
        channels[4],
        channels[5],
        channels[6],
        channels[7]
    );
 
    uint16_t len = mavlink_msg_to_send_buffer(buf, &msg);
    fwrite(buf, len, 1, stdout);
    fflush(stdout);
    fsync(fileno(stdout));

    fprintf(stderr, "local system id: \t%u\n", system_id);
    fprintf(stderr, "local component id: \t%u\n", component_id);
    fprintf(stderr, "remote system id: \t%u\n", rsystem_id);
    fprintf(stderr, "remote component id: \t%u\n", MAV_COMP_ID_ALL);
    for (size_t i = 0; i < CHANNELS; ++i) {
        fprintf(stderr, "channel %zu: \t\t%u\n", i, channels[i]);
    }
    fprintf(stderr, "\n");
}