#include <SDL.h>
#include <SDL_ttf.h>

void drawText(SDL_Renderer* renderer, const int x, const int y, const int w, const int h, TTF_Font* font, int fontSize, SDL_Color textColor, const char* text) {
    TTF_SetFontSize(font, fontSize);
    // text setup
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, textColor);

    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    int textW = textSurface->w;
    int textH = textSurface->h;

    SDL_Rect textRect;

    textRect.w = textW;
    textRect.h = textH;

    // rectangle setup
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    textRect.x = rect.x + (rect.w - textW) / 2;
    textRect.y = rect.y + (rect.h - textH) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return;
}

void drawText(SDL_Renderer* renderer, const int x, const int y, const int w, const int h, TTF_Font* font, int fontSize, const SDL_Color* textColor, const char* text) {
    TTF_SetFontSize(font, fontSize);
    // text setup
    const SDL_Color* currentTextColor = textColor;
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, *currentTextColor);

    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    int textW = textSurface->w;
    int textH = textSurface->h;

    SDL_Rect textRect;

    textRect.w = textW;
    textRect.h = textH;

    // rectangle setup
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    textRect.x = rect.x + (rect.w - textW) / 2;
    textRect.y = rect.y + (rect.h - textH) / 2;

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return;
}

void drawText90(SDL_Renderer* renderer, const int x, const int y, const int w, const int h, TTF_Font* font, int fontSize, SDL_Color textColor, const char* text) {
    TTF_SetFontSize(font, fontSize);
    // text setup
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, textColor);

    SDL_Texture* textTexture =
        SDL_CreateTextureFromSurface(renderer, textSurface);

    int textW = textSurface->w;
    int textH = textSurface->h;

    SDL_Rect textRect;

    textRect.w = textW;
    textRect.h = textH;

    // rectangle setup
    SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    textRect.x = rect.x + (rect.w - textW) / 2;
    textRect.y = rect.y + (rect.h - textH) / 2;

    SDL_Point center = {textW / 2, textH / 2};
    SDL_RenderCopyEx(renderer, textTexture, NULL, &textRect, 90.0, &center, SDL_FLIP_NONE);
    
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    return;
}