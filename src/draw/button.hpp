#include "text.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

void drawButton(
    SDL_Renderer* renderer, 
    int x, 
    int y, 
    int w, 
    int h,
    TTF_Font* font,
    bool selected,
    const char* text
) {
    const int buttonSize = 10;
    const int borderSize = 5;
    const int textHeight = 0;
    const SDL_Color buttonColor = {255, 255, 255, 255}; 
    const SDL_Color borderColor = {224, 223, 222, 255};
    const SDL_Color textColor = {0, 0, 0};
    const SDL_Color buttonColorSelected = {9, 117, 6, 255};
    const SDL_Color textColorSelected = {255, 255, 255};
    const SDL_Color* currentButtonColor = &buttonColor;
    const SDL_Color* currentTextColor = &textColor;

    int width = w - buttonSize;
    int height = h/7;
    int buttonX = x + borderSize;
    int buttonY = y + borderSize;
    int borderX = buttonX - borderSize;
    int borderY = buttonY - borderSize;
    int borderW = width + 2*borderSize;
    int borderH = height + 2*borderSize;
    int textX = x;
    int textY = y + textHeight;

    if (selected)
    {
        currentButtonColor = &buttonColorSelected;
        currentTextColor = &textColorSelected;
    }
    

    // outline
    drawOpaqueRect(renderer, borderX, borderY, borderW, borderH, borderColor);
    // button
    drawOpaqueRect(renderer, buttonX, buttonY, width, height, currentButtonColor);

    drawText(renderer, textX, textY, width, height, font, 25, currentTextColor, text);

}