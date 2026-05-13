#include "state.h"
#include "render.hpp"
#include <SDL.h>
#include <algorithm>
#include <iostream>

StateMachine::StateMachine(const Menu& menuInstance)
    : currentState{
        menuInstance, 
        0, 
        0, 
        0,
        {0,1,2,3,4,5,6}, 
        0,
        0.25f,
        Phase::Opening, 
        {1, false, {0,0,0,0}, {0,1,0,1}, {{0,0,0},{0,0,0},{0,0,0},{0,0,0}} }
    }, 
    running(true)
{
}

void StateMachine::handleLeft() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Increment) {
            currentState.optionIndex = std::max(0, currentState.optionIndex - 2);
        } else {
            if(currentState.pageIndex > 0) {
                currentState.phase = Phase::ShiftLeft;
                currentState.time = 0;
            }
        }
    }
}

void StateMachine::handleRight() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement) {
            currentState.optionIndex = std::min(static_cast<int>(std::size(currentState.menu.pages[currentState.pageIndex].options)) - 1, currentState.optionIndex + 2);
        } else {
            if(currentState.pageIndex >= std::size(currentState.menu.pages) - 1) {
                return;
            }
            currentState.phase = Phase::ShiftRight;
            currentState.time = 0;
        }
    }
}

void StateMachine::handleUp() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Increment || currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement) {
        } else {
            currentState.optionIndex = std::max(0, currentState.optionIndex - 1);
        }
    }
    if(currentState.phase == Phase::InIdle) {
        currentState.subOptionIndex = std::max(0, currentState.subOptionIndex - 1);
        if(currentState.subOptionIndex != 0 && currentState.subOptionIndex < currentState.subOptionWindow[0]) {
            currentState.phase = Phase::ShiftUp;
            currentState.time = 0;
        }
    }
}

void StateMachine::handleDown() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement) {
            int maxOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options);
            currentState.optionIndex = std::min(maxOptionIndex - 1, currentState.optionIndex + 3);
        } else {
            int maxOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options);
            currentState.optionIndex = std::min(maxOptionIndex - 1, currentState.optionIndex + 1);
        }
    }
    if(currentState.phase == Phase::InIdle) {
        int maxSubOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].subOptions);
        currentState.subOptionIndex = std::min(maxSubOptionIndex - 1, currentState.subOptionIndex + 1);
        int maxSubOptionWindowIndex = std::size(currentState.subOptionWindow);
        if(currentState.subOptionIndex > currentState.subOptionWindow[maxSubOptionWindowIndex - 1]) {
            currentState.phase = Phase::ShiftDown;
            currentState.time = 0;
        }
    }
}

void StateMachine::handleSubOption() {
    switch (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].subOptions[currentState.subOptionIndex].subOptionType) {
        case 0: // controller selection
            currentState.menuState.controllerIndex[currentState.pageIndex - 1] = currentState.subOptionIndex;
            break;
        case 1: // primary color selection
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[0] = currentState.subOptionIndex;
            break;
        case 2: // secondary color selection 
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[1] = currentState.subOptionIndex;
            break;
        case 3: // tertiary color selection
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[2] = currentState.subOptionIndex;
            break;
        default:
            break;
    }
    return;
}

void StateMachine::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        running = false;
        return;
    }

    if (event.type == SDL_KEYDOWN && (
            currentState.phase == Phase::Idle ||
            currentState.phase == Phase::InIdle || 
            currentState.phase == Phase::PostOpening || 
            currentState.phase == Phase::PreClosing
        )) {
        // std::cout << "Page index: " << currentState.pageIndex << std::endl;
        // std::cout << "Option index: " << currentState.optionIndex << std::endl;
        // std::cout << "Sub-option index: " << currentState.subOptionIndex << std::endl;
        switch (event.key.keysym.sym) {
        case SDLK_UP:
            handleUp();
            break;
        case SDLK_DOWN:
            handleDown();
            break;
        case SDLK_LEFT:
            handleLeft();
            break;
        case SDLK_RIGHT:
            handleRight();
            break;
        case SDLK_RETURN:
            if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Increment && currentState.menuState.playerCount < 4) {
                currentState.menuState.playerCount = (currentState.menuState.playerCount) + 1;
            }
            if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement && currentState.menuState.playerCount > 1) {
                currentState.menuState.playerCount = (currentState.menuState.playerCount) - 1;
            }
            if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Boolean) {
                currentState.menuState.useKM = !currentState.menuState.useKM;
            }
            if (currentState.phase == Phase::InIdle) {
                handleSubOption();
                currentState.phase = Phase::ShiftOut;
                currentState.time = 0;
            }
            if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Subpage && currentState.phase == Phase::Idle) {
                currentState.phase = Phase::ShiftIn;
                currentState.time = 0;
            }
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
        if(currentState.phase == Phase::ShiftRight) {
            currentState.optionIndex = 0;
            currentState.phase = Phase::FadeIn;
            int maxPageIndex = std::size(currentState.menu.pages);
            currentState.pageIndex = std::min(maxPageIndex - 1, currentState.pageIndex + 1);
        }
        if(currentState.phase == Phase::ShiftLeft) {
            currentState.optionIndex = 0;
            currentState.phase = Phase::FadeIn;
            currentState.pageIndex = std::max(0, currentState.pageIndex - 1);
        }
        if(currentState.phase == Phase::ShiftUp) {
            currentState.subOptionWindow.pop_back();
            currentState.subOptionWindow.push_front(currentState.subOptionWindow.front() - 1);
            std::cout << currentState.subOptionWindow[0] << "," << currentState.subOptionWindow[6] << std::endl;
            currentState.phase = Phase::InFadeIn;
        }
        if(currentState.phase == Phase::ShiftDown) {
            currentState.subOptionWindow.pop_front();
            currentState.subOptionWindow.push_back(currentState.subOptionWindow.back() + 1);
            std::cout << currentState.subOptionWindow[0] << "," << currentState.subOptionWindow[6] << std::endl;
            currentState.phase = Phase::InFadeIn;
        }
        if(currentState.phase == Phase::FadeIn) {
            currentState.optionIndex = 0;
            currentState.phase = Phase::Idle;
            currentState.time = 0;
        }
        if(currentState.phase == Phase::InFadeIn) {
            currentState.phase = Phase::InIdle;
            currentState.time = 0;
        }
        if(currentState.phase == Phase::ShiftIn) {
            currentState.subOptionIndex = 0;
            currentState.phase = Phase::InFadeIn;
        }
        if(currentState.phase == Phase::ShiftOut) {
            currentState.phase = Phase::FadeIn;
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