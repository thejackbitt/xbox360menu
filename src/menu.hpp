#include <string>
#include <vector>
#include <optional>
#include <iostream>

// option types:
// - 0 = boolean
// - 1 = subpages
// - 2 = positive increment
// - 3 = negative increment
// - 4 = pointer display

enum class OptionType {
    Boolean,
    Subpage,
    Increment,
    Decrement,
    PointerDisplay
};

class Option {
public:
    std::string label;
    OptionType type;
    int value = 0;

    std::optional<int*> pointer;
    std::optional<std::vector<std::string>> subOptions;
    
    Option(std::string lbl, OptionType t)
        : label(std::move(lbl)), type(t) {}

    Option(std::string lbl, std::vector<std::string> subs)
        : label(std::move(lbl)),
          type(OptionType::Subpage),
          subOptions(std::move(subs)) {}

    Option(std::string lbl, int* ptr)
        : label(std::move(lbl)),
          type(OptionType::PointerDisplay),
          pointer(ptr) {}
};

class Page {
public:
    std::string label;
    std::vector<Option> options;
    bool visible = false;

    Page() = default;

    explicit Page(std::string lbl, bool vis = false)
        : label(std::move(lbl)), visible(vis) {}

    void addOption(const Option& opt) {
        options.push_back(opt);
    }
};

class Menu {
public:
    static constexpr int MAX_PAGES = 5;
    Page pages[MAX_PAGES];

    Menu() {
        init();
    }

private:
    void initPlayer(int index) {
        pages[index].label = "Player " + std::to_string(index);

        pages[index].addOption(
            Option("Controller", {"Controller 1", "Controller 2", "Controller 3", "Controller 4"}));

        pages[index].addOption(
            Option("Primary Color", {"Red", "Green", "Blue", "Yellow"}));

        pages[index].addOption(
            Option("Secondary Color", {"Red", "Green", "Blue", "Yellow"}));

        pages[index].addOption(
            Option("Tertiary Color", {"Red", "Green", "Blue", "Yellow"}));
    }

    void init() {
        pages[0] = Page("Splitscreen", true);

        pages[0].addOption(Option("Add Player", OptionType::Increment));
        pages[0].addOption(Option("Display", OptionType::PointerDisplay));
        pages[0].addOption(Option("Remove Player", OptionType::Decrement));
        pages[0].addOption(Option("Use K/M for Player 1", OptionType::Boolean));

        for (int i = 1; i <= 4; i++) {
            initPlayer(i);
        }
    }
};