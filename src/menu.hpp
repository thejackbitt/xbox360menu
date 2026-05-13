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
    Toggle,
    Subpage,
    SubpageOption,
    Increment,
    Decrement,
    PointerDisplay
};

class Option {
public:
    std::string label;
    OptionType type;
    std::vector<Option> subOptions;
    int subOptionType;
    int value = 0;

    std::optional<int*> pointer;
    
    Option(std::string lbl, OptionType t)
        : label(std::move(lbl)), type(t) {}
    
    Option(std::string lbl, OptionType t, int s)
    : label(std::move(lbl)), type(t), subOptionType(s) {}

    // Option(std::string lbl, std::vector<std::string> subs)
    //     : label(std::move(lbl)),
    //       type(OptionType::Subpage),
    //       subOptions(std::move(subs)) {}

    // Option(std::string lbl, int* ptr)
    //     : label(std::move(lbl)),
    //       type(OptionType::PointerDisplay),
    //       pointer(ptr) {}

    void addOption(const Option& opt) {
        subOptions.push_back(opt);
    }
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
            Option("Controller", OptionType::Subpage));
        pages[index].options[0].addOption(Option("Controller 1", OptionType::SubpageOption, 0));
        pages[index].options[0].addOption(Option("Controller 2", OptionType::SubpageOption, 0));
        pages[index].options[0].addOption(Option("Controller 3", OptionType::SubpageOption, 0));
        pages[index].options[0].addOption(Option("Controller 4", OptionType::SubpageOption, 0));

        pages[index].addOption(Option("Team", OptionType::Toggle));

        //
        // adding primary colors
        //
        pages[index].addOption(Option("Primary Color", OptionType::Subpage));
        pages[index].options[2].addOption(Option("Steel", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Silver", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("White", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Brown", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Tan", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Khaki", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Sage", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Olive", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Drab", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Forest", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Green", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Sea Foam", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Teal", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Aqua", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Cyan", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Blue", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Cobalt", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Ice", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Violet", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Orchid", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Lavender", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Maroon", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Brick", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Rose", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Rust", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Coral", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Peach", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Gold", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Yellow", OptionType::SubpageOption, 1));
        pages[index].options[2].addOption(Option("Pale", OptionType::SubpageOption, 1));
        //
        // adding secondary colors
        //
        pages[index].addOption(Option("Secondary Color", OptionType::Subpage));
        pages[index].options[3].addOption(Option("Steel", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Silver", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("White", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Brown", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Tan", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Khaki", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Sage", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Olive", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Drab", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Forest", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Green", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Sea Foam", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Teal", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Aqua", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Cyan", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Blue", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Cobalt", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Ice", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Violet", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Orchid", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Lavender", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Maroon", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Brick", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Rose", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Rust", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Coral", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Peach", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Gold", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Yellow", OptionType::SubpageOption, 2));
        pages[index].options[3].addOption(Option("Pale", OptionType::SubpageOption, 2));
        //
        // adding tertiary colors
        //
        pages[index].addOption(Option("Tertiary Color", OptionType::Subpage));
        pages[index].options[4].addOption(Option("Steel", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Silver", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("White", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Brown", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Tan", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Khaki", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Sage", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Olive", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Drab", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Forest", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Green", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Sea Foam", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Teal", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Aqua", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Cyan", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Blue", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Cobalt", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Ice", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Violet", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Orchid", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Lavender", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Maroon", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Brick", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Rose", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Rust", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Coral", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Peach", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Gold", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Yellow", OptionType::SubpageOption, 3));
        pages[index].options[4].addOption(Option("Pale", OptionType::SubpageOption, 3));
    }

    void init() {
        pages[0] = Page("Splitscreen", true);

        pages[0].addOption(Option("Remove Player", OptionType::Decrement));
        pages[0].addOption(Option("Display", OptionType::PointerDisplay));
        pages[0].addOption(Option("Add Player", OptionType::Increment));
        pages[0].addOption(Option("Use K/M for Player 1", OptionType::Boolean));

        for (int i = 1; i <= 4; i++) {
            initPlayer(i);
        }
    }
};