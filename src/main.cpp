#include "menu.hpp"
#include "state.cpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <array>
#include <iostream>
#include "./assets/open_wav.h"
#include "./assets/close_wav.h"
#include "./assets/nav_wav.h"
#include "./assets/nav_page_wav.h"
#include "./assets/select_wav.h"
#include "./assets/denied_wav.h"

int main(int argc, char* argv[])
{

    // int WINDOW_WIDTH = 1080;
    // int WINDOW_HEIGHT = 720;
    int WINDOW_WIDTH = 1620;
    int WINDOW_HEIGHT = 1080;

    // begin init renderer
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    // init sounds
    SDL_RWops* rw0 = SDL_RWFromConstMem(open_wav, open_wav_len);
    Mix_Chunk* openSound = Mix_LoadWAV_RW(rw0, 1);
    SDL_RWops* rw1 = SDL_RWFromConstMem(close_wav, close_wav_len);
    Mix_Chunk* closeSound = Mix_LoadWAV_RW(rw1, 1);
    SDL_RWops* rw2 = SDL_RWFromConstMem(nav_wav, nav_wav_len);
    Mix_Chunk* navSound = Mix_LoadWAV_RW(rw2, 1);
    SDL_RWops* rw3 = SDL_RWFromConstMem(nav_page_wav, nav_page_wav_len);
    Mix_Chunk* navPageSound = Mix_LoadWAV_RW(rw3, 1);
    SDL_RWops* rw4 = SDL_RWFromConstMem(select_wav, select_wav_len);
    Mix_Chunk* selectSound = Mix_LoadWAV_RW(rw4, 1);
    SDL_RWops* rw5 = SDL_RWFromConstMem(denied_wav, denied_wav_len);
    Mix_Chunk* deniedSound = Mix_LoadWAV_RW(rw5, 1);

    const std::array<Mix_Chunk*, 6> sounds = {
        openSound, 
        closeSound, 
        navSound, 
        navPageSound, 
        selectSound, 
        deniedSound
    };

    SDL_GameControllerEventState(SDL_ENABLE);

    SDL_GameController* controller = nullptr;
    int numJoysticks = SDL_NumJoysticks();
    for (int i = 0; i < numJoysticks; ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                std::cout << "Opened game controller: " << SDL_GameControllerName(controller) << std::endl;
                break;
            }
            std::cerr << "SDL_GameControllerOpen failed: " << SDL_GetError() << std::endl;
        }
    }

    if (!controller) {
        std::cerr << "No game controller opened. D-pad events may not be available." << std::endl;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        return 1;
    }
    TTF_Font* font = TTF_OpenFont("C:/Windows/Fonts/segoeui.ttf", 25);

    if (!font) {
        std::cerr << "Font load error: " << TTF_GetError() << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Xbox 360 Splitscreen Menu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // end init renderer


    // begin setup
    Mix_PlayChannel(-1, sounds[0], 0);
    const Menu menuInstance;
    StateMachine stateMachine(menuInstance, sounds);
    SDL_Event event;

    // end setup


    // render/input loop

    Uint32 lastTicks = SDL_GetTicks();

    while (stateMachine.isRunning()) {
        while (SDL_PollEvent(&event)) {
            stateMachine.handleEvent(event);
        }

        Uint32 now = SDL_GetTicks();
        float dt = (now - lastTicks) / 1000.0f;
        lastTicks = now;

        stateMachine.update(dt);

        stateMachine.render(renderer, font, WINDOW_WIDTH, WINDOW_HEIGHT);

        SDL_RenderPresent(renderer);
    }

    // on exit
    if (controller) {
        SDL_GameControllerClose(controller);
    }
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(openSound);
    Mix_FreeChunk(closeSound);
    Mix_FreeChunk(navSound);
    Mix_FreeChunk(navPageSound);
    Mix_FreeChunk(selectSound);
    Mix_FreeChunk(deniedSound);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}