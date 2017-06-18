#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <SDL2/SDL_mixer.h>
#include <SDL.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Mix_Music *music = NULL;
void close() {
    // Free-ing stuffs
    if ( music != NULL ) {
        Mix_FreeMusic( music );
    }

    Mix_Quit();
    SDL_Quit();
}


int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./msplayer [sound file]" << std::endl;
        return 0;
    }
    std::cout << "Track playing: " << argv[1] << std::endl;

    // Initialize SDL
    if ( SDL_Init( SDL_INIT_AUDIO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    // Initialize SDL_mixer
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    // Load the piece.
    music = Mix_LoadMUS(argv[1]);
    if (!music) {
        std::cout << "Mix_LoadMUS(\"" << argv[1] << "\"):" << Mix_GetError() << std::endl;
        exit(1);
    }

    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;
        
    // No music is playing in the background.
    while( !quit ) {
        // Handle events on queue
        while( SDL_PollEvent( &e ) != 0 ) {
            // User requests quit
            if ( e.type == SDL_QUIT ) quit = true;
            
            // Handle key press
            else if ( e.type = SDL_KEYDOWN ) 
            {
                switch( e.key.keysym.sym ) {
                    case SDLK_9:
                        if ( Mix_PlayingMusic() == 0) {
                            // Playback
                            if ( Mix_PlayMusic(music, -1) == -1 ) {
                                printf( "Mix_PlayMusic: %s\n", Mix_GetError() );
                                printf( "LUL no music for you.\n" );
                            }
                        } else {
                            // If the music is pause.
                            if ( Mix_PausedMusic() == 1) {
                                Mix_ResumeMusic();
                            } else {
                                // Pause the music if it's playing.
                                Mix_PauseMusic();
                            }
                        }
                    case SDLK_0:
                        // Stop the music
                        Mix_HaltMusic();
                        break;
                
                }
            }

        }
    }
    close();
    return 0;
}
