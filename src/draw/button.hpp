#include "text.hpp"
#include "icon.hpp"
#include "dot.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include "./assets/unchecked_dark.h"
#include "./assets/unchecked_light.h"
#include "./assets/checked_light.h"
#include "./assets/checked_dark.h"

void drawButton(
    SDL_Renderer* renderer, 
    int x, 
    int y, 
    int w, 
    int h,
    TTF_Font* font,
    bool selected,
    Uint8 opacity,
    const char* text,
    OptionType type,
    int state,
    std::optional<SDL_Color> colorValue = std::nullopt
) {
    const int buttonSize = 10;
    const int borderSize = 5;
    const int textHeight = 0;
    const SDL_Color buttonColor = {255, 255, 255, opacity}; 
    const SDL_Color borderColor = {224, 223, 222, opacity};
    const SDL_Color textColor = {0, 0, 0, opacity};
    const SDL_Color buttonColorSelected = {9, 117, 6, opacity};
    const SDL_Color textColorSelected = {255, 255, 255, opacity};
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

    if (type == OptionType::Increment || type == OptionType::Decrement || type == OptionType::PointerDisplay) {
        // outline
        drawOpaqueRect(renderer, borderX, borderY, borderW/3, borderH, borderColor);
        // button
        drawOpaqueRect(renderer, buttonX, buttonY, width/3, height, currentButtonColor);
        
        if (type == OptionType::Increment || type == OptionType::Decrement) {
            const char* symbol = type == OptionType::Increment ? "+" : "-";
            drawText(renderer, textX, textY, width/3, height, font, 25, currentTextColor, symbol);
        } else {
            const char* symbol = text;
            drawText(renderer, textX, textY, width/3, height, font, 25, currentTextColor, symbol);
        }
    } else if(type == OptionType::Boolean) {
        // outline
        drawOpaqueRect(renderer, borderX, borderY, borderW, borderH, borderColor);
        // button
        drawOpaqueRect(renderer, buttonX, buttonY, width, height, currentButtonColor);

        const void* checkmarkData = selected ? (state == 1 ? checked_light_png : unchecked_light_png) : (state == 1 ? checked_dark_png : unchecked_dark_png);
        size_t checkmarkLen = selected ? (state == 1 ? sizeof(checked_light_png) : sizeof(unchecked_light_png)) : (state == 1 ? sizeof(checked_dark_png) : sizeof(unchecked_dark_png));

        drawIcon(renderer, buttonX + width - height, buttonY + height/5, height/1.5, height/1.5, checkmarkData, checkmarkLen);
        
        drawText(renderer, textX, textY, width, height, font, 25, currentTextColor, text);
    } else if(type == OptionType::TeamToggle) {
        // outline
        drawOpaqueRect(renderer, borderX, borderY, borderW, borderH, borderColor);
        // button
        drawOpaqueRect(renderer, buttonX, buttonY, width, height, currentButtonColor);

        
        if(state == 0) {
            drawDot(renderer, textX + width/3, textY + height/2, height/4, {212, 44, 44, opacity});
            drawText(renderer, textX, textY, width, height, font, 25, currentTextColor, "Red Team");
        } else {
            drawDot(renderer, textX + width/3, textY + height/2, height/4, {44, 78, 212, opacity});
            drawText(renderer, textX, textY, width, height, font, 25, currentTextColor, "Blue Team");
        }
    } else {
        // outline
        drawOpaqueRect(renderer, borderX, borderY, borderW, borderH, borderColor);
        // button
        drawOpaqueRect(renderer, buttonX, buttonY, width, height, currentButtonColor);

        if(colorValue.has_value()) {
            drawDot(renderer, textX + width/4, textY + height/2, height/4, colorValue.value());
        }
        
        drawText(renderer, textX, textY, width, height, font, 25, currentTextColor, text);
    }

}