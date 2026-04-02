#include "draw/rect.hpp"
#include "draw/button.hpp"
#include "draw/page.hpp"
#include "draw/icon.hpp"
#include "assets/mccIcon.h"
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
    int iconSize = 45;
    
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
    if(state.phase != Phase::Closing && state.phase != Phase::Opening) {
        menuScale = 0.45f;
        menuWidth  = WINDOW_WIDTH  * menuScale;
        menuHeight = WINDOW_HEIGHT * menuScale;
        menuPosX = (WINDOW_WIDTH  - menuWidth)  / 2;
        menuPosY = (WINDOW_HEIGHT - menuHeight) / 2;
    }

    if(state.phase != Phase::Closing && state.phase != Phase::Opening) {
        int offset = 0;
        int updateSize = 50;
        if(state.phase == Phase::ShiftRight) {
            offset = -updateSize * (alpha);
        }
        if(state.phase == Phase::ShiftLeft) {
            offset = updateSize * (alpha);
        }
        if(state.pageIndex > 0) {
            int prefixSize = -updateSize;
            for(int i = state.pageIndex-1; i >= 0; i--) {
                drawPage(renderer, offset+menuPosX+prefixSize, menuPosY, menuWidth, menuHeight, state.menu.pages[i].label.data(), false, font);
                prefixSize -= updateSize;
            }
        }
        drawIcon(renderer, menuPosX + (menuWidth-iconSize), menuPosY - iconSize - 10, iconSize, iconSize, mccIcon_png, mccIcon_png_len);
        drawText(renderer, menuPosX - (menuWidth/2) + 10, menuPosY - 190, menuWidth, menuHeight, font, 25, {255, 255, 255, 255}, "MCC Guide");
        drawPage(renderer, menuPosX, menuPosY, menuWidth, menuHeight, state.menu.pages[state.pageIndex].label.data(), true, font);
    }
    if(state.phase != Phase::Closing && state.phase != Phase::Opening && state.pageIndex < std::size(state.menu.pages) - 1) {
        int offset = 0;
        int updateSize = 50;
        if(state.phase == Phase::ShiftRight) {
            offset = -updateSize * (alpha);
        }
        if(state.phase == Phase::ShiftLeft) {
            offset = updateSize * (alpha);
        }
        int suffixSize = menuWidth+updateSize;
        for(int i = state.pageIndex+1; i < std::size(state.menu.pages); i++) {
            drawPage(renderer, offset+menuPosX+suffixSize, menuPosY, menuWidth, menuHeight, state.menu.pages[i].label.data(), false, font);
            suffixSize += updateSize;
        }
    }
    drawOpaqueRect(renderer, menuPosX, menuPosY, menuWidth, menuHeight, {255, 255, 255, 255});
    if(state.phase != Phase::Closing && state.phase != Phase::Opening) {
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
}