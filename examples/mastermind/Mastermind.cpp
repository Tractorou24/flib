#include "Mastermind.h"

#include <format>

namespace mastermind
{
    std::vector<std::shared_ptr<sf::Drawable>> get_ui_elements(const sf::Font& font)
    {
        auto title = std::make_shared<sf::Text>("MasterMind 2000", font, 50);
        title->setPosition(150, 10);
        title->setFillColor(sf::Color::Cyan);

        auto remaining_tries = std::make_shared<sf::Text>("10 tries remaining !", font, 20);
        remaining_tries->setPosition(15, 445);

        return {title, remaining_tries};
    }

    void restart(std::array<sf::Color, 4>& win_colors, sf::Text* tries_text, int& remaining_tries)
    {
        std::ranges::generate(win_colors, [&win_colors]
        {
            return get_random_color(win_colors[0],
                                    win_colors[1],
                                    win_colors[2],
                                    win_colors[3]);
        });
        remaining_tries = 10;
        tries_text->setString(std::format("{} tries remaining", remaining_tries));
    }

    std::array<sf::Color, 4> generate_win_colors()
    {
        std::array<sf::Color, 4> res;
        std::ranges::generate(res, [&res]
        {
            return get_random_color(res[0],
                                    res[1],
                                    res[2],
                                    res[3]);
        });
        return res;
    }
}
