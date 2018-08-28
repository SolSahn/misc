#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include <cstdlib>

// Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

// Music
Mix_Chunk *music = NULL;

// Surfaces
SDL_Surface *particle = NULL;
SDL_Surface *rocket = NULL;
SDL_Surface *image = NULL;
SDL_Surface *screen = NULL;

// Event structure
SDL_Event event;

// Optimized image load function
SDL_Surface *load_image(std::string img) {

    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load(img.c_str());

    if (loadedImage != NULL) {

        optimizedImage = SDL_DisplayFormat(loadedImage);
        SDL_FreeSurface(loadedImage);

    }

    return optimizedImage;

}

// Optimized surface blitting function
void apply_surface(int x, int y, SDL_Surface* src, SDL_Surface* dest) {

    SDL_Rect offset;
    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(src, NULL, dest, &offset);

}

int posOrNeg() {
    switch(rand() % 2) {
        case 0: return -1; break;
        case 1: return 1; break;
    }
}

// Initialization function, returns false if setup fails
bool init() {

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        return false;
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    if (screen == NULL) {
        return false;
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        return false;
    }

    SDL_WM_SetCaption("Poopy Fart", NULL);

    return true;

}

// Load files function, returns false if fails
bool load_files() {

    image = load_image("Assets/x.png");
    rocket = load_image("Assets/rocket.bmp");
    particle = load_image("Assets/update.png");

    music = Mix_LoadWAV("Assets/beenis.wav");

    if(image == NULL || rocket == NULL || particle == NULL || music == NULL) {
        return false;
    }

    return true;

}

// Free up surfaces and quit
void clean_up() {

    SDL_FreeSurface(image);
    SDL_FreeSurface(rocket);
    SDL_FreeSurface(particle);

    Mix_FreeChunk(music);

    SDL_Quit();

}

void particle_generate(int srcX, int srcY, int r, SDL_Surface* img, int c) {
    for(int i = 0; i < c; i++) {
        apply_surface(srcX + ((rand() % r) * posOrNeg()), srcY + ((rand() % r) * posOrNeg()), img, screen);
    }
}

int main(int argc, char* args[]) {

    float xvel = 0;
    float yvel = 0;
    float xpos = SCREEN_WIDTH/2;
    float ypos = SCREEN_HEIGHT/2;

    // Variable waits for program quit
    bool quit = false;

    // Runs, init and file load functions, terminates if either fail
    if(init() == false) {
        return 1;
    }

    if(load_files() == false) {
        return 1;
    }

    while (!quit) {

        while (SDL_PollEvent(&event)) {

            if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP: yvel-=0.1; break;
                    case SDLK_DOWN: yvel+=0.1; break;
                    case SDLK_LEFT: xvel-=0.1; break;
                    case SDLK_RIGHT: xvel+=0.1; break;
                }
            }

            else if(event.type == SDL_QUIT) {
                quit = true;
                clean_up();
                return 0;

            }

        }

        xpos += xvel;
        ypos += yvel;

        // Apply image to screen
        apply_surface(0, 0, image, screen);
        apply_surface(xpos, ypos, rocket, screen);
        particle_generate(xpos, ypos, 100, particle, 1);

        if(Mix_PlayChannel(-1, music, -1) == NULL) {
            printf("Mix_PlayChannel: %s\n",Mix_GetError());
        }

        // Update, terminate if fail
        if(SDL_Flip(screen) == -1) {
            return 1;
        }

    }

}
