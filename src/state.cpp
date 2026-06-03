#include "state.h"
#include "render.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <array>
#include <algorithm>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <string>
#include "./assets/open_wav.h"
#include "./assets/close_wav.h"
#include "./assets/nav_wav.h"
#include "./assets/nav_page_wav.h"
#include "./assets/select_wav.h"
#include "./assets/denied_wav.h"

bool saveMenuStateBin(const MenuState& state, const std::string& path) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;

    uint8_t useKM = state.useKM ? 1 : 0;
    ofs.write(reinterpret_cast<const char*>(&state.playerCount), sizeof(state.playerCount));
    ofs.write(reinterpret_cast<const char*>(&useKM), sizeof(useKM));
    ofs.write(reinterpret_cast<const char*>(state.controllerIndex), sizeof(state.controllerIndex));
    ofs.write(reinterpret_cast<const char*>(state.teamIndex), sizeof(state.teamIndex));
    ofs.write(reinterpret_cast<const char*>(state.playerColors), sizeof(state.playerColors));
    return ofs.good();
}

bool loadMenuStateBin(MenuState& state, const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return false;

    uint8_t useKM = 0;
    ifs.read(reinterpret_cast<char*>(&state.playerCount), sizeof(state.playerCount));
    ifs.read(reinterpret_cast<char*>(&useKM), sizeof(useKM));
    state.useKM = useKM != 0;
    ifs.read(reinterpret_cast<char*>(state.controllerIndex), sizeof(state.controllerIndex));
    ifs.read(reinterpret_cast<char*>(state.teamIndex), sizeof(state.teamIndex));
    ifs.read(reinterpret_cast<char*>(state.playerColors), sizeof(state.playerColors));
    return ifs.good();
}

StateMachine::StateMachine(const Menu& menuInstance, const std::array<Mix_Chunk*, 6>& sounds)
    : currentState{
        menuInstance,
        sounds, 
        0, 
        0, 
        0,
        {0,1,2,3,4,5}, 
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
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            currentState.optionIndex = std::max(0, currentState.optionIndex - 2);
        } else {
            Mix_PlayChannel(-1, currentState.sounds[3], 0);
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
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            currentState.optionIndex = std::min(static_cast<int>(std::size(currentState.menu.pages[currentState.pageIndex].options)) - 1, currentState.optionIndex + 2);
        } else {
            if(currentState.pageIndex >= std::size(currentState.menu.pages) - 1) {
                return;
            }
            Mix_PlayChannel(-1, currentState.sounds[3], 0);
            currentState.phase = Phase::ShiftRight;
            currentState.time = 0;
        }
    }
}

void StateMachine::handleUp() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Increment || currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement) {
            Mix_PlayChannel(-1, currentState.sounds[5], 0);
        } else {
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            currentState.optionIndex = std::max(0, currentState.optionIndex - 1);
        }
    }
    if(currentState.phase == Phase::InIdle) {
        Mix_PlayChannel(-1, currentState.sounds[2], 0);
        currentState.subOptionIndex = std::max(0, currentState.subOptionIndex - 1);
        if(currentState.subOptionIndex < currentState.subOptionWindow[0]) {
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            currentState.phase = Phase::ShiftUp;
            currentState.time = 0;
        }
    }
}

void StateMachine::handleDown() {
    if(currentState.phase == Phase::Idle) {
        if(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement) {
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            int maxOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options);
            currentState.optionIndex = std::min(maxOptionIndex - 1, currentState.optionIndex + 3);
        } else {
            Mix_PlayChannel(-1, currentState.sounds[2], 0);
            int maxOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options);
            currentState.optionIndex = std::min(maxOptionIndex - 1, currentState.optionIndex + 1);
        }
    }
    if(currentState.phase == Phase::InIdle) {
        int maxSubOptionIndex = std::size(currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].subOptions);
        Mix_PlayChannel(-1, currentState.sounds[2], 0);
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
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.controllerIndex[currentState.pageIndex - 1] = currentState.subOptionIndex;
            break;
        case 1: // primary color selection
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[0] = currentState.subOptionIndex;
            break;
        case 2: // secondary color selection 
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[1] = currentState.subOptionIndex;
            break;
        case 3: // tertiary color selection
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.playerColors[currentState.pageIndex - 1].colors[2] = currentState.subOptionIndex;
            break;
        default:
            break;
    }
    return;
}

void StateMachine::handleOption() {
    // confirm/select action
    if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Increment && currentState.menuState.playerCount < 4) {
        Mix_PlayChannel(-1, currentState.sounds[4], 0);
        currentState.menuState.playerCount = (currentState.menuState.playerCount) + 1;
    }
    if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Decrement && currentState.menuState.playerCount > 1) {
        Mix_PlayChannel(-1, currentState.sounds[4], 0);
        currentState.menuState.playerCount = (currentState.menuState.playerCount) - 1;
    }
    if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Boolean) {
        Mix_PlayChannel(-1, currentState.sounds[4], 0);
        currentState.menuState.useKM = !currentState.menuState.useKM;
    }
    if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::TeamToggle) {
        if(currentState.menuState.teamIndex[currentState.pageIndex - 1] == 0) {
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.teamIndex[currentState.pageIndex - 1] = 1;
        } else {
            Mix_PlayChannel(-1, currentState.sounds[4], 0);
            currentState.menuState.teamIndex[currentState.pageIndex - 1] = 0;
        }
    }
    if (currentState.phase == Phase::InIdle) {
        StateMachine::handleSubOption();
        currentState.phase = Phase::ShiftOut;
        currentState.time = 0;
    }
    if (currentState.menu.pages[currentState.pageIndex].options[currentState.optionIndex].type == OptionType::Subpage && currentState.phase == Phase::Idle) {
        Mix_PlayChannel(-1, currentState.sounds[4], 0);
        currentState.subOptionWindow = {0,1,2,3,4,5};
        currentState.phase = Phase::ShiftIn;
        currentState.time = 0;
    }
}

void StateMachine::handleClose() {
    Mix_PlayChannel(-1, currentState.sounds[1], 0);
    currentState.phase = Phase::Closing;
    currentState.time = 0;
}

void StateMachine::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_QUIT) {
        running = false;
        return;
    }

    if (currentState.phase == Phase::Idle ||
        currentState.phase == Phase::InIdle || 
        currentState.phase == Phase::PostOpening || 
        currentState.phase == Phase::PreClosing
    ) {

        {
            static int prevLX = 0; // -1 left, 0 neutral, 1 right
            static int prevLY = 0; // -1 up, 0 neutral, 1 down
            const int JOYSTICK_DEADZONE = 8000;
            if (event.type == SDL_CONTROLLERAXISMOTION) {
                if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
                    int v = event.caxis.value;
                    int dir = 0;
                    if (v < -JOYSTICK_DEADZONE) dir = -1;
                    else if (v > JOYSTICK_DEADZONE) dir = 1;
                    if (dir != prevLX) {
                        if (dir == -1) handleLeft();
                        else if (dir == 1) handleRight();
                        prevLX = dir;
                    }
                } else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
                    int v = event.caxis.value;
                    int dir = 0;
                    if (v < -JOYSTICK_DEADZONE) dir = -1;
                    else if (v > JOYSTICK_DEADZONE) dir = 1;
                    if (dir != prevLY) {
                        if (dir == -1) handleUp();
                        else if (dir == 1) handleDown();
                        prevLY = dir;
                    }
                }
            }
        }

        if(event.type == SDL_CONTROLLERBUTTONDOWN) {
            switch (event.cbutton.button)
            {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:
                    handleUp();
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                    handleDown();
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                    handleLeft();
                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                    handleRight();
                    break;
                
                case SDL_CONTROLLER_BUTTON_A:
                    handleOption();
                    break;

                case SDL_CONTROLLER_BUTTON_BACK:
                    handleClose();
                    break;
            }
        }

        if (event.type == SDL_KEYDOWN) {
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
                handleOption();
                break;
                case SDLK_ESCAPE:
                handleClose();
                break;
                default:
                break;
            }
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
            currentState.phase = Phase::InFadeIn;
        }
        if(currentState.phase == Phase::ShiftDown) {
            currentState.subOptionWindow.pop_front();
            currentState.subOptionWindow.push_back(currentState.subOptionWindow.back() + 1);
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