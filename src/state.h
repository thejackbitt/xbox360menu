#include <SDL.h>
#include <SDL_ttf.h>

enum class Phase {
    Opening,
    PostOpening,
    Idle,
    Update,
    PreClosing,
    Closing
};

struct State {
    Menu menu;
    int pageIndex;
    int optionIndex;
    int subOptionIndex;
    float time;
    float duration;
    Phase phase;
};

class StateMachine {
public:
    StateMachine(const Menu& menu);

    void handleEvent(const SDL_Event& event);
    void update(float dt);
    void render(SDL_Renderer* renderer, TTF_Font* font, int WINDOW_WIDTH, int WINDOW_HEIGHT);

    bool isRunning() const;
    State& getState();

private:
    State currentState;
    bool running;
};