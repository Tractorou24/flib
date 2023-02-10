#include <SFML/Graphics.hpp>

#include <format>
#include "Mastermind.h"

import FLib.Application;
import FLib.Button;
import FLib.TextButton;
import FLib.MessageBox;
import FLib.Layer;
import FLib.Scene;
import Sling;

int main()
{
    flib::Application app(sf::VideoMode(640, 480), "THE MasterMind 2000");

    // Get application font
    sf::Font font;
    font.loadFromFile(R"(C:\Windows\Fonts\times.ttf)");

    // Init game variables
    int remainingTries = 10;
    auto win_colors = mastermind::generate_win_colors();
    const auto ui_elements = mastermind::get_ui_elements(font);
    const auto buttons = mastermind::get_buttons<4>();
    std::ranges::for_each(buttons, [](auto btn)
    {
        btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>(
            [](const std::shared_ptr<flib::Button> button)
            {
                const auto& color_it = std::ranges::find(mastermind::g_colors, button->backgroundColor());
                button->setBackgroundColor(
                    mastermind::g_colors[(std::distance(mastermind::g_colors.cbegin(),
                                                        color_it) + 1) % (mastermind::g_colors.size() - 1)]);
            }));
    });

    sf::Text order_button_text("Check Order", font, 20);
    order_button_text.setFillColor(sf::Color::White);

    const auto order_button = std::make_shared<flib::TextButton>(sf::Vector2f(430, 440), sf::Vector2f(200, 30),
                                                                 order_button_text);
    order_button->setOutlineColor(sf::Color::White);
    order_button->setOutlineThickness(2);
    order_button->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([&](std::shared_ptr<flib::Button>)
    {
        // Update tries number
        auto* tries_text = reinterpret_cast<sf::Text*>(ui_elements[1].get());
        tries_text->setString(std::format("{} tries remaining", --remainingTries));

        // Check win condition
        bool local_win = true;
        for (int i = 0; i < 4; i++)
            if (win_colors[i] != buttons[i]->backgroundColor())
                local_win = false;

        // Show MessageBox game is over
        if (local_win || remainingTries < 1)
        {
            flib::MessageBox msg(font);
            msg.setTitle(local_win ? "Win!" : "Loose!");
            msg.setMessage(local_win ? "You win!" : "You lose!");
            const auto restart_btn = msg.addButton("Restart");
            restart_btn->setOutlineColor(sf::Color::White);
            restart_btn->setOutlineThickness(2);
            restart_btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([&](std::shared_ptr<flib::Button>)
            {
                mastermind::restart(win_colors, tries_text, remainingTries);
                msg.hide();
            }));

            const auto quit_btn = msg.addButton("Quit");
            quit_btn->setOutlineColor(sf::Color::White);
            quit_btn->setOutlineThickness(2);
            quit_btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>([&](std::shared_ptr<flib::Button>)
            {
                msg.hide();
                app.close();
            }));
            msg.show(app);
        }
    }));

    const auto scene = std::make_shared<flib::Scene>();
    const auto layer = std::make_shared<flib::Layer>();
    std::ranges::for_each(ui_elements, [&layer](const auto& element) { layer->addDrawable(element); });
    std::ranges::for_each(buttons, [&layer](const auto button) { layer->addDrawable(button); });
    layer->addDrawable(order_button);

    scene->addLayer("base", layer);
    app.addScene("main", scene);
    app.setActiveScene("main");
    app.run();
    return 0;
}
