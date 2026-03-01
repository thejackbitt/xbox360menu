#include <string>
#include <vector>
#include <optional>

// option types:
// - 0 = boolean
// - 1 = subpages
// - 2 = positive increment
// - 3 = negative increment
// - 4 = pointer display

struct Option {
    std::string label;
    int type;
    int value;
    std::optional<int*> pointer;
    std::optional<std::vector<std::string>> subOptions;
};

struct Page {
    std::string label;
    std::vector<Option> options;
    bool visible;
};