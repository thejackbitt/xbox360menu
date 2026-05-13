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
    float buttonOffset = 0.00f;
    int iconSize = 45;
    int pageCount = std::size(state.menu.pages);
    Uint8 globalAlpha = 255;
    
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
    if(state.phase == Phase::Idle) {
        globalAlpha = 255;
    }
    if(state.phase == Phase::FadeIn) {
        globalAlpha = static_cast<Uint8>(255 * alpha);
    }
    if(state.phase == Phase::ShiftRight || state.phase == Phase::ShiftLeft || state.phase == Phase::ShiftIn || state.phase == Phase::ShiftOut) {
        globalAlpha = static_cast<Uint8>(255 * (1.0f - alpha));
    }
    // rendering the left pages 
    if(state.phase != Phase::Closing && state.phase != Phase::Opening) {
        int offset = 0;
        int updateSize = 50;
        int extra = 0;
        if(state.phase == Phase::ShiftRight) {
            offset = -updateSize * (alpha) + 50;
            extra = 1;
        }
        if(state.phase == Phase::ShiftLeft) {
            offset = updateSize * (alpha) + 50;
            extra = 1;
        }
        if(state.phase == Phase::ShiftIn) {
            offset = updateSize * (alpha) * (alpha * 4);
        }
        if(state.phase == Phase::ShiftOut) {
            offset = -updateSize * (alpha) + updateSize;
        }
        if(state.phase == Phase::ShiftUp) {
            buttonOffset = (buttonSize*state.subOptionWindow[0]) * (alpha+state.subOptionWindow[0]+1);
        }
        if(state.phase == Phase::ShiftDown) {
            buttonOffset = (buttonSize*state.subOptionWindow[0]) * -(alpha+state.subOptionWindow[0]+1);
        }
        if(state.phase == Phase::InIdle || state.phase == Phase::InFadeIn) {
            buttonOffset = (buttonSize*state.subOptionWindow[0]) * -1;
        }

        if(state.pageIndex >= 0 && 
            state.phase != Phase::InIdle && 
            state.phase != Phase::InFadeIn && 
            state.phase != Phase::ShiftUp &&
            state.phase != Phase::ShiftDown
        ) {
            int prefixSize = -updateSize;
            for(int i = state.pageIndex-1+extra; i >= 0; i--) {
                drawPage(renderer, offset+menuPosX+prefixSize, menuPosY, menuWidth, menuHeight, state.menu.pages[i].label.data(), false, 255, font);
                prefixSize -= updateSize;
            }
        }
        // rendering the accents
        drawIcon(renderer, menuPosX + (menuWidth-iconSize), menuPosY - iconSize - 10, iconSize, iconSize, mccIcon_png, mccIcon_png_len);
        drawText(renderer, menuPosX - (menuWidth/2) + 10, menuPosY - 190, menuWidth, menuHeight, font, 25, {255, 255, 255, 255}, "MCC Guide");
        drawPage(renderer, menuPosX, menuPosY, menuWidth, menuHeight, state.menu.pages[state.pageIndex].label.data(), true, globalAlpha, font);
    }
    // rendering the right pages
    if(state.phase != Phase::Closing && state.phase != Phase::Opening && state.pageIndex <= pageCount - 1) {
        int offset = 0;
        int updateSize = 50;
        int extra = 0;
        if(state.phase == Phase::ShiftRight) {
            offset = -updateSize * (alpha) - 50;
            extra = 1;
        }
        if(state.phase == Phase::ShiftLeft) {
            offset = updateSize * (alpha) - 50;
            extra = 1;
        }
        if(state.phase == Phase::ShiftIn) {
            offset = -updateSize * (alpha) * (alpha * 4);
        }
        if(state.phase == Phase::ShiftOut) {
            offset = updateSize * (alpha) - updateSize;
        }
        if(
            state.phase != Phase::InIdle && 
            state.phase != Phase::InFadeIn &&
            state.phase != Phase::ShiftUp &&
            state.phase != Phase::ShiftDown
        ) {
            int suffixSize = menuWidth+updateSize;
            for(int i = state.pageIndex+1-extra; i < pageCount; i++) {
                drawPage(renderer, offset+menuPosX+suffixSize, menuPosY, menuWidth, menuHeight, state.menu.pages[i].label.data(), false, 255, font);
                suffixSize += updateSize;
            }
        }
    }
    // rendering the current page
    drawOpaqueRect(renderer, menuPosX, menuPosY, menuWidth, menuHeight, {255, 255, 255, 255});
    // normal states
    if(state.phase == Phase::Idle || state.phase == Phase::FadeIn || state.phase == Phase::ShiftRight || state.phase == Phase::ShiftLeft || state.phase == Phase::ShiftIn) {
        float buttonCount = 0.0f;
        for(int i = 0; i < state.menu.pages[state.pageIndex].options.size(); i++) {
            OptionType type = state.menu.pages[state.pageIndex].options[i].type;
            if(type == OptionType::Increment || type == OptionType::Decrement || type == OptionType::PointerDisplay) {
                drawButton(
                    renderer, 
                    menuPosX + i*(menuWidth/3), 
                    menuPosY,
                    menuWidth, 
                    menuHeight,
                    font, 
                    state.optionIndex == i,
                    globalAlpha,
                    std::to_string(state.menuState.playerCount).c_str(),
                    type,
                    0
                );
                buttonCount += 0.33f;
                continue;
            } else if (type == OptionType::Boolean) {
                drawButton(
                    renderer, 
                    menuPosX, 
                    menuPosY + buttonCount*buttonSize, 
                    menuWidth, 
                    menuHeight,
                    font, 
                    state.optionIndex == i,
                    globalAlpha,
                    state.menu.pages[state.pageIndex].options[i].label.c_str(),
                    type,
                    state.menuState.useKM ? 1: 0
                );
                buttonCount++;
            } else {
                drawButton(
                    renderer, 
                    menuPosX, 
                    menuPosY + buttonCount*buttonSize, 
                    menuWidth, 
                    menuHeight,
                    font, 
                    state.optionIndex == i,
                    globalAlpha,
                    state.menu.pages[state.pageIndex].options[i].label.c_str(),
                    type,
                    0
                );
                buttonCount++;
            }
        }
    }
    // sub states (sub pages)
    if(state.phase == Phase::InIdle || state.phase == Phase::InFadeIn || state.phase == Phase::ShiftOut || state.phase == Phase::ShiftUp || state.phase == Phase::ShiftDown) {
        float buttonCount = 0.0f;
        for(int i = 0; i < state.menu.pages[state.pageIndex].options[state.optionIndex].subOptions.size(); i++) {
            OptionType type = state.menu.pages[state.pageIndex].options[state.optionIndex].subOptions[i].type;
            drawButton(
                renderer, 
                menuPosX, 
                menuPosY + buttonCount*buttonSize + buttonOffset, 
                menuWidth, 
                menuHeight,
                font, 
                state.subOptionIndex == i,
                globalAlpha,
                state.menu.pages[state.pageIndex].options[state.optionIndex].subOptions[i].label.c_str(),
                type,
                0
            );
            buttonCount++;
        }
    }
}