#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"

void joystick_open(int id, SDL_Joystick* joystick) {

    joystick=SDL_JoystickOpen(id);
  
    if(joystick) {
        fprintf(stderr, "Opened joystick %d\n", id);
        fprintf(stderr, "Name: %s\n", SDL_JoystickName(joystick));
        fprintf(stderr, "Number of axes: %d\n", SDL_JoystickNumAxes(joystick));
        fprintf(stderr, "Number of buttons: %d\n", SDL_JoystickNumButtons(joystick));
        fprintf(stderr, "Number of balls: %d\n", SDL_JoystickNumBalls(joystick));
    } else {
        fprintf(stderr, "Couldn't open joystick %d\n", id);
    }
}

void joystick_close(SDL_Joystick* joystick) {
    if (SDL_JoystickGetAttached(joystick)) {
        SDL_JoystickClose(joystick);
    }
}

int main(int argc, char *argv[]) { 

    //initialize joystick library
    if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
        fprintf(stderr, "Couldn't initialize SDL joystick: %s\n", SDL_GetError());
        exit(1);
    }

    //id of controlling joystick
    int joystick_id = 0;

    int throttle = 0;

    //pointer to joystick in use
    SDL_Joystick* joystick = NULL;

    //event loop
    SDL_Event e;
    bool cont = true;
    while (cont) {

        while( SDL_WaitEventTimeout(&e, 500)) {
            switch( e.type ) {

            case SDL_JOYDEVICEADDED:
                if (e.jdevice.which == joystick_id) {
                    fprintf(stderr, "Joystick connected\n");
                    joystick_open(joystick_id, joystick);
                }
                break;

            case SDL_JOYDEVICEREMOVED:
                if (e.jdevice.which == joystick_id) {
                    fprintf(stderr, "Joystick disconnected\n");
                    joystick_close(joystick);
                }
                break;

            case SDL_JOYAXISMOTION:
                fprintf(stderr, "axis event\n");
                fprintf(stderr, "%d\n", e.jaxis.value);
                break;
            case SDL_JOYHATMOTION:
                fprintf(stderr, "hat event\n");
                break;

            case SDL_JOYBUTTONDOWN:
                switch (e.jbutton.button) {
                case 0:
                    throttle = 12;
                    break;
                case 4:
                    throttle -= 1;
                    break;
                case 5:
                    throttle += 1;
                    break;
                case 6:
                    throttle -= 25;
                    break;
                case 7:
                    throttle += 25;
                    break;
                default:
                    break;
                }

                fprintf(stderr, "button down\n");
                break;

            case SDL_JOYBUTTONUP:
                fprintf(stderr, "button up\n");
                break;

            case SDL_QUIT:
                cont = false;
                break;

            default:
                fprintf(stderr,"Unsupported event type: %d\n", e.type);
                break;
            }

            if (throttle < 0) {
                throttle = 0;
            }
            if (throttle > 255) {
                throttle = 255;
            }
            unsigned char tc = (unsigned char) throttle;
            fprintf(stderr, "%u\n", tc);
            printf("%u\n", tc);
            fflush(stdout);

        }

        unsigned char tc = (unsigned char) throttle;
        //fwrite(&tc, 1, 1, stdout);
        //fflush(stdout);
        printf("%u\n", tc);
        fflush(stdout);

        

        //print_control_data(&c);
        //printf("--\n");
    }

    return 0;
}