#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <time.h>

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <smpeg2/smpeg.h>
#include <vlc/vlc.h>

using std::string;

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Mix_Music *music = NULL;
SMPEG *mpeg = NULL;
SMPEG_Info info;
const char* track_name;

void close() {
    // Free-ing stuffs
    if ( music != NULL ) {
        Mix_FreeMusic( music );
    }

    Mix_Quit();
    SDL_Quit();
}


bool play_smpeg() {
    // Code using SMPEG2 for mp3.
    int use_audio = 1;
    mpeg = SMPEG_new(track_name, &info, use_audio);

    if (SMPEG_error(mpeg)) {
        fprintf(stderr, "%s: %s\n", track_name, SMPEG_error(mpeg));
        SMPEG_delete(mpeg);
        return false;
    }

    if (mpeg != NULL) {
        printf("[Debug] mpeg object is not null.\n");
        if ( info.has_audio ) printf("\tAudio %s\n", info.audio_string);
        if ( info.total_size ) printf("\tSize: %d\n", info.total_size);
        if ( info.total_time ) printf("\tTotal time: %f\n", info.total_time);
        
        SMPEG_enableaudio(mpeg, use_audio);
        SMPEG_setvolume(mpeg, 100);
        SMPEG_play(mpeg);
        while(SMPEG_status(mpeg) == SMPEG_PLAYING) {

        }
        
        SMPEG_delete(mpeg);
        exit(0);
    }
    return true;
}

bool play_vlc() {

    struct timespec start, stop;

    // Code using vlc library.
    libvlc_instance_t *inst;
    libvlc_media_player_t *mp;
    libvlc_media_t *m;

    // Load the vlc engine.
    inst = libvlc_new(0, NULL);

    // Create a new item
    m = libvlc_media_new_path(inst, track_name);

    // Create a media play playing venvironment
    mp = libvlc_media_player_new_from_media(m);

    // No need to keep the media now
    libvlc_media_release(m);


    // Play the media player
    libvlc_media_player_play(mp);


    do {
        sleep(5);
    }
    while (libvlc_media_player_is_playing(mp));

    // stop playing
    libvlc_media_player_stop(mp);

    // Free the media player
    libvlc_media_player_release(mp);

    libvlc_release(inst);
    return false;
}

void play_sdl_mixer() {
    // Initialize SDL_mixer
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        printf( "SDL_mixer could not initialize! SDL_Mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    // Load the piece.
    music = Mix_LoadMUS(track_name);
    if (!music) {
        std::cout << "Mix_LoadMUS(\"" << track_name << "\"):" << Mix_GetError() << std::endl;
        exit(1);
    }
    // Main loop flag
    bool quit = false;

    // Event handler
    SDL_Event e;
        
    // No music is playing in the background.
    while( !quit ) {
        // Handle events on queue
        if ( Mix_PlayingMusic() == 0 ) {
            if ( Mix_PlayMusic(music, -1) == -1 ) {
                printf( "Mix_PlayMusic: %s\n", Mix_GetError() );
                quit = true;
            }
        }
        /*
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
                        break;
                    case SDLK_0:
                        // Stop the music
                        Mix_HaltMusic();
                        break;
                    case SDLK_1:
                        quit = true;
                        break;
                
                }
            }

        }
        */
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./msplayer [sound file]" << std::endl;
        return 0;
    }

    std::cout << "Track playing: " << argv[1] << std::endl;

    track_name = argv[1];

    // Initialize SDL
    if ( SDL_Init( SDL_INIT_AUDIO ) < 0 ) {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exit(1);
    }

    // Use vlc
    play_vlc();
    
    // Use SMPEG
    //if (play_smpeg())
      //  printf("[Debug] mpeg object is null. Switching to SDL_Mixer\n");
    
    // Use SDL_mixer
    //play_sdl_mixer();
    close();
    return 0;
}
