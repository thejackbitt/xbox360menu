// #include "state.hpp"
// #include "menu.hpp"
// #include <SDL.h>

// void handleInput(State& state, Menu& menu)
// {
//     SDL_Event event;

//     while (SDL_PollEvent(&event))
//     {
//         if (event.type == SDL_KEYDOWN)
//         {
//             switch(event.key.keysym.sym)
//             {
//                 case SDLK_RIGHT:
//                     state.pageIndex++;
//                     if (state.pageIndex >= menu.pages.length())
//                         state.pageIndex = 0;
//                     break;

//                 case SDLK_LEFT:
//                     state.pageIndex--;
//                     if (state.pageIndex < 0)
//                         state.pageIndex = menu.pages.length() - 1;
//                     break;
//             }
//         }
//     }
// }