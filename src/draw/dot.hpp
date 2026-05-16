#pragma once
#include <SDL.h>
#include <cmath>

void drawDot(
    SDL_Renderer* renderer,
    int x,
    int y,
    int size,
    SDL_Color color
) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 48, 48, 48, color.a);

    int outlineRadius = size + 1;

    for (int dy = -outlineRadius; dy <= outlineRadius; ++dy)
    {
        int dx = static_cast<int>(
            std::sqrt(outlineRadius * outlineRadius - dy * dy)
        );

        SDL_RenderDrawLine(
            renderer,
            x - dx,
            y + dy,
            x + dx,
            y + dy
        );
    }

    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    for (int dy = -size; dy <= size; ++dy)
    {
        int dx = static_cast<int>(
            std::sqrt(size * size - dy * dy)
        );

        SDL_RenderDrawLine(
            renderer,
            x - dx,
            y + dy,
            x + dx,
            y + dy
        );
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}