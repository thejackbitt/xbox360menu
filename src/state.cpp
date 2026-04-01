#include "state.h"
#include "render.hpp"
#include <SDL.h>
#include <algorithm>
#include <iostream>

StateMachine::StateMachine(const Menu& menuInstance)
    : currentState{menuInstance, 0, 0, 0, 0, 0.25f, Phase::Idle}, running(true)
{
}

void StateMachine::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        running = false;
        return;
    }

    if (event.type == SDL_KEYDOWN && (
            currentState.phase == Phase::Idle || 
            currentState.phase == Phase::PostOpening || 
            currentState.phase == Phase::PreClosing
        )) {
        // std::cout << "Page index: " << currentState.pageIndex << std::endl;
        // std::cout << "Option index: " << currentState.optionIndex << std::endl;
        // std::cout << "Sub-option index: " << currentState.subOptionIndex << std::endl;
        int maxPageIndex = std::size(currentState.menu.pages);
        int maxOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options);
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            currentState.optionIndex = std::max(0, currentState.optionIndex - 1);
            break;
        case SDLK_DOWN:
            currentState.optionIndex = std::min(maxOptionIndex - 1, currentState.optionIndex + 1);
            break;
        case SDLK_LEFT:
            currentState.pageIndex = std::max(0, currentState.pageIndex - 1);
            break;
        case SDLK_RIGHT:
            currentState.pageIndex = std::min(maxPageIndex - 1, currentState.pageIndex + 1);
            break;
        case SDLK_RETURN:
            // confirm/select action
            break;
        case SDLK_ESCAPE:
            currentState.phase = Phase::Closing;
            currentState.time = 0;
            break;
        default:
            break;
        }
    }
}

void StateMachine::update(float dt)
{
    if(currentState.time < currentState.duration) {
        currentState.time += dt;
    }
    if(currentState.time >= currentState.duration) {
        if(currentState.phase == Phase::Opening) {
            currentState.phase = Phase::Idle;
            currentState.time = 0;
        } else if(currentState.phase == Phase::Closing) {
            running = false;
        }
    }
}

void StateMachine::render(SDL_Renderer* renderer, TTF_Font* font, int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderMenu(renderer, font, WINDOW_WIDTH, WINDOW_HEIGHT, currentState);
}

bool StateMachine::isRunning() const
{
    return running;
}

State& StateMachine::getState()
{
    return currentState;
}