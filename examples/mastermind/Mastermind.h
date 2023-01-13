#pragma once

#include <SFML/Graphics.hpp>

#include <array>
#include <random>

import FLib.Button;
import Sling;

namespace sf::extensions
{
    class Button;
    class ColoredButton;
    class TextButton;
}

namespace mastermind
{
    static const std::array g_colors = {
        sf::Color::White, sf::Color::Red, sf::Color::Green, sf::Color::Blue,
        sf::Color::Yellow, sf::Color::Magenta, sf::Color::Cyan
    };

    std::vector<std::shared_ptr<sf::Drawable>> get_ui_elements(const sf::Font& font);
    void restart(std::array<sf::Color, 4>& win_colors, sf::Text* tries_text, int& remaining_tries);
    std::array<sf::Color, 4> generate_win_colors();

    template <typename... Args>
    sf::Color get_random_color(Args... args)
    {
        static std::random_device rd;
        static std::uniform_int_distribution<size_t> dis(0, g_colors.size() - 1);

        const sf::Color color = g_colors[dis(rd)];
        if constexpr (sizeof...(args) == 0)
            return color;
        else
        {
            const std::array<sf::Color, sizeof...(args)> colors {{std::forward<Args>(args)...}};
            if (std::ranges::find(colors, color) != colors.cend())
                return get_random_color(args...);
            return color;
        }
    }

    template <std::size_t Nb>
    std::vector<std::shared_ptr<flib::Button>> get_buttons(sling::Slot<flib::Button*> slot)
    {
        const static std::array<sf::Color, Nb> colors = {
            sf::Color::Blue, sf::Color::Yellow, sf::Color::Green, sf::Color::Magenta
        };

        std::vector<std::shared_ptr<flib::Button>> buttons;
        buttons.reserve(Nb);
        for (std::size_t i = 0; i < Nb; i++)
        {
            buttons.emplace_back(std::make_shared<flib::Button>(sf::Vector2f(static_cast<float>(50 + i * 140), 100),
                                                                sf::Vector2f(125, 300),
                                                                colors[i]));
            buttons.back()->onClick.connect(slot);
        }
        return buttons;
    }
}
