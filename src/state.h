#include <deque>
#include <array>
#include <SDL.h>
#include <SDL_mixer.h>
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
    std::array<Mix_Chunk*, 6> sounds;
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
    StateMachine(const Menu& menu, const std::array<Mix_Chunk*, 6>& sounds);

    void handleLeft();
    void handleRight();
    void handleUp();
    void handleDown();
    void handleOption();
    void handleSubOption();
    void handleClose();

    void handleEvent(const SDL_Event& event);
    void update(float dt);
    void render(SDL_Renderer* renderer, TTF_Font* font, int WINDOW_WIDTH, int WINDOW_HEIGHT);

    bool isRunning() const;
    State& getState();

private:
    State currentState;
    bool running;
};