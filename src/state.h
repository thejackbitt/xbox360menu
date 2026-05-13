#include <deque>
#include <SDL.h>
#include <SDL_ttf.h>

enum class Phase {
    Opening,
    PostOpening,
    FadeIn,
    InFadeIn,
    Idle,
    InIdle,
    ShiftIn,
    ShiftOut,
    ShiftRight,
    ShiftLeft,
    ShiftUp,
    ShiftDown,
    PreClosing,
    Closing
};

struct PlayerColor {
    int colors[3];
};
struct MenuState {
    int playerCount;
    bool useKM;
    int controllerIndex[4];
    int teamIndex[4];
    PlayerColor playerColors[4];
};

struct State {
    Menu menu;
    int pageIndex;
    int optionIndex;
    int subOptionIndex;
    std::deque<int> subOptionWindow;
    float time;
    float duration;
    Phase phase;
    MenuState menuState;
};

class StateMachine {
public:
    StateMachine(const Menu& menu);

    void handleLeft();
    void handleRight();
    void handleUp();
    void handleDown();
    void handleSubOption();

    void handleEvent(const SDL_Event& event);
    void update(float dt);
    void render(SDL_Renderer* renderer, TTF_Font* font, int WINDOW_WIDTH, int WINDOW_HEIGHT);

    bool isRunning() const;
    State& getState();

private:
    State currentState;
    bool running;
};