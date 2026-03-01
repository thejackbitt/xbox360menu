#include <./menu.hpp>
#include <iostream>

int initPlayer(Page *page, int player) {
    page[player].label = "Player " + std::to_string(player);
    page[player].options.push_back({"Controller", 1, 0, std::nullopt, std::vector<std::string>{"Controller 1", "Controller 2", "Controller 3", "Controller 4"}});
    page[player].options.push_back({"Primary Color", 1, 0, std::nullopt, std::vector<std::string>{"Red", "Green", "Blue", "Yellow"}});
    page[player].options.push_back({"Secondary Color", 1, 0, std::nullopt, std::vector<std::string>{"Red", "Green", "Blue", "Yellow"}});
    page[player].options.push_back({"Tertiary Color", 1, 0, std::nullopt, std::vector<std::string>{"Red", "Green", "Blue", "Yellow"}});
    return 0;
}

int main() {
    Page page[5];
    page[0].label = "Splitscreen";
    page[0].options.push_back({"Add Player", 2, 0, std::nullopt, std::nullopt});
    page[0].options.push_back({"Display", 4, 0, std::nullopt, std::nullopt});
    page[0].options.push_back({"Remove Player", 3, 0, std::nullopt, std::nullopt});
    page[0].options.push_back({"Use K/M for Player 1", 0, 0, std::nullopt, std::nullopt});
    page[0].visible = true;
    initPlayer(page, 1);
    initPlayer(page, 2);
    initPlayer(page, 3);
    initPlayer(page, 4);

    std::cout << page[2].label << std::endl;
    return 0;
}