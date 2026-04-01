#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

void drawPage(
    SDL_Renderer* renderer, 
    int menuPosX,
    int menuPosY,
    int menuWidth,
    int menuHeight,
    char* label,
    TTF_Font* font

) {
    drawGradientRect(renderer, menuPosX-(menuWidth/9), menuPosY, menuWidth/9, menuHeight, 100, {255, 255, 255, 255}, {224, 223, 222, 255}, 1);
    drawText90(renderer, menuPosX-(menuWidth/9), menuPosY-(menuHeight*0.25), menuWidth/9, menuHeight, font, 25, {78, 81, 86}, label);
}