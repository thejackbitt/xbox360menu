#include <SDL.h>

void drawGradientRect(SDL_Renderer * renderer,
        const int x, const int y, const int w, const int h, const float steps,
        const SDL_Color c1, const SDL_Color c2, const int fill) 
    {
    float yt = y;
    float rt = c1.r;
    float gt = c1.g;
    float bt = c1.b;
    float at = c1.a;
    
    float ys = h/steps;
    float rs = (c2.r - c1.r)/steps;
    float gs = (c2.g - c1.g)/steps;
    float bs = (c2.b - c1.b)/steps;
    float as = (c2.a - c1.a)/steps;

    for(int i = 0; i < steps ; i++)
    {
        SDL_Rect rect = { x, yt, w, ys+1 };

        SDL_SetRenderDrawColor(renderer, rt, gt, bt, at);

        if(fill)
            SDL_RenderFillRect(renderer, &rect);
        else
            SDL_RenderDrawRect(renderer, &rect);

        yt += ys;
        rt += rs;
        gt += gs;
        bt += bs;
        at += as;
    }

    return;
}

void drawOpaqueRect(SDL_Renderer * renderer,
        const int x, const int y, const int w, const int h,
        const SDL_Color c) 
    {
    SDL_Rect rect = { x, y, w, h };

    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(renderer, &rect);

    return;
}

void drawOpaqueRect(SDL_Renderer * renderer,
        const int x, const int y, const int w, const int h,
        const SDL_Color* &c) 
    {
    SDL_Rect rect = { x, y, w, h };

    SDL_SetRenderDrawColor(renderer, c->r, c->g, c->b, c->a);
    SDL_RenderFillRect(renderer, &rect);

    return;
}