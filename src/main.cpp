#include "menu.hpp"
#include "state.cpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

int main(int argc, char* argv[])
{

    int WINDOW_WIDTH = 1080;
    int WINDOW_HEIGHT = 720;

    // begin init renderer

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
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

    const Menu menuInstance;
    StateMachine stateMachine(menuInstance);
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
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}