#pragma once
#include <SDL.h>
#include <SDL_image.h>

void drawIcon(
    SDL_Renderer* renderer,
    int x, int y, int w, int h,
    const void* imageData,
    size_t imageSize
) {
    SDL_RWops* rw = SDL_RWFromConstMem(imageData, imageSize);
    if (!rw) {
        SDL_Log("SDL_RWFromConstMem failed: %s", SDL_GetError());
        return;
    }

    SDL_Surface* imageSurface = IMG_Load_RW(rw, 1); // 1 frees rw
    if (!imageSurface) {
        SDL_Log("Failed to load image from memory: %s", IMG_GetError());
        return;
    }

    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!imageTexture) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }

    SDL_Rect destRect = { x, y, w, h };
    SDL_RenderCopy(renderer, imageTexture, NULL, &destRect);
    SDL_DestroyTexture(imageTexture);
}