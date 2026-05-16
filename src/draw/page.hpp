#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <optional>

void drawPage(
    SDL_Renderer* renderer, 
    int menuPosX,
    int menuPosY,
    int menuWidth,
    int menuHeight,
    char* label,
    bool selected,
    Uint8 opacity,
    TTF_Font* font,
    std::optional<float> margin = 0.00f
) {
    if(selected) {
        drawGradientRect(renderer, menuPosX-(menuWidth/9), menuPosY, menuWidth/9, menuHeight, 100, {255, 255, 255, opacity}, {224, 223, 222, opacity}, 1);
        drawText90(renderer, menuPosX-(menuWidth/9), menuPosY-(menuHeight*0.25) + margin.value_or(0.00f), menuWidth/9, menuHeight, font, 25, {78, 81, 86, opacity}, label);
        return;
    }

    drawGradientRect(renderer, menuPosX-(menuWidth/9), menuPosY, menuWidth/9, menuHeight, 100, {73, 87, 99, opacity}, {66, 76, 86, opacity}, 1);
    drawText90(renderer, menuPosX-(menuWidth/9), menuPosY-(menuHeight*0.25) + margin.value_or(0.00f), menuWidth/9, menuHeight, font, 25, {206, 215, 222, opacity}, label);

    return;
}