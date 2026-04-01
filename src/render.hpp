#include "draw/rect.hpp"
#include "draw/button.hpp"
#include "draw/page.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

void renderMenu(
    SDL_Renderer* renderer, 
    TTF_Font* font, 
    int WINDOW_WIDTH, 
    int WINDOW_HEIGHT, 
    State state
) {
    float alpha = std::min(state.time / state.duration, 1.0f);
    float menuScale = 0.00f;
    int menuWidth = 0;
    int menuHeight = 0;
    int menuPosX = 0;
    int menuPosY = 0;
    int buttonSize = 50;
    
    if(state.phase == Phase::Opening) {
        menuScale = 0.45f * alpha;
        menuWidth  = WINDOW_WIDTH  * menuScale;
        if(alpha > 0.5f) {
            menuHeight = WINDOW_HEIGHT * menuScale;
        } else {
            menuHeight = WINDOW_HEIGHT * menuScale/2;
        }
        menuPosX = (WINDOW_WIDTH  - menuWidth)  / 2;
        menuPosY = (WINDOW_HEIGHT - menuHeight) / 2;
    }
    if(state.phase == Phase::Closing) {
        menuScale = 0.45f * (1.0f - alpha);
        menuWidth  = WINDOW_WIDTH  * menuScale;
        if(alpha < 0.5f) {
            menuHeight = WINDOW_HEIGHT * menuScale;
        } else {
            menuHeight = WINDOW_HEIGHT * menuScale/2;
        }
        menuPosX = (WINDOW_WIDTH  - menuWidth)  / 2;
        menuPosY = (WINDOW_HEIGHT - menuHeight) / 2;
    }
    if(state.phase == Phase::Idle) {
        menuScale = 0.45f;
        menuWidth  = WINDOW_WIDTH  * menuScale;
        menuHeight = WINDOW_HEIGHT * menuScale;
        menuPosX = (WINDOW_WIDTH  - menuWidth)  / 2;
        menuPosY = (WINDOW_HEIGHT - menuHeight) / 2;
    }

    drawPage(renderer, menuPosX, menuPosY, menuWidth, menuHeight, state.menu.pages[state.pageIndex].label.data(), font);
    drawOpaqueRect(renderer, menuPosX, menuPosY, menuWidth, menuHeight, {255, 255, 255, 255});
    for(int i = 0; i < state.menu.pages[state.pageIndex].options.size(); i++) {
        drawButton(
            renderer, 
            menuPosX, 
            menuPosY + i*buttonSize, 
            menuWidth, 
            menuHeight,
            font, 
            state.optionIndex == i, 
            state.menu.pages[state.pageIndex].options[i].label.c_str()
        );
    }
}