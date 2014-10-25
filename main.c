#include <stdio.h>
#include <SDL.h>
#include "controller.h"

static int run(int joystick_id, int timeout, bool debug);
static void print_channels(channel_t* channels, bool debug);

int main(int argc, char *argv[]) { 

    //initialize joystick library
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Couldn't initialize SDL joystick subsystem: %s\n", SDL_GetError());
        exit(1);
    }

    run(0, 500, true);

   
    return 0;
}

static int run(int joystick_id, int timeout, bool debug) {

    channel_t channels[CHANNELS];
    channel_reset(channels);

    //pointer to joystick in use
    SDL_Joystick* joystick = NULL;

    //event loop
    SDL_Event e;
    bool cont = true;
    while (cont) {

        if(SDL_WaitEventTimeout(&e, timeout)) {
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
        print_channels(channels, debug);
    }
    return 0;
}

void print_channels(channel_t* channels, bool debug) {
    for(size_t i=0; i < CHANNELS; ++i) {
        fprintf(stdout, "%u\n", channels[i]);
        fprintf(stderr, "channel %zu: %u\n", i, channels[i]);
    }
    fflush(stdout);
    fflush(stderr);
}