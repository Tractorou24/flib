#include <SFML/Graphics.hpp>

import FLib.Application;
import FLib.Scene;
import FLib.Layer;
import FLib.Button;
import FLib.TextButton;
import FLib.MessageBox;
import Sling;

import Sokoban;

int main()
{
    flib::Application app(sf::VideoMode(1215, 700), "Sokoban++");

    const auto title = std::make_shared<sf::Text>("Sokoban++", app.font(), 100);
    title->setFillColor(sf::Color::White);
    title->setOutlineColor(sf::Color::Black);
    title->setOutlineThickness(2);
    title->setStyle(sf::Text::Bold);
    title->setOrigin(title->getLocalBounds().width / 2, title->getLocalBounds().height / 2);
    title->setPosition(app.window().getSize().x / 2, 100);

    const auto play_button = std::make_shared<flib::TextButton>(sf::Vector2f(510, 350),
                                                                sf::Vector2f(225, 50),
                                                                sf::Text("Play !", app.font(), 25),
                                                                sf::Color::Blue);
    play_button->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>(
        [&app](std::shared_ptr<flib::Button>)
        {
            app.setActiveScene("game");
        }));

    const auto quit_button = std::make_shared<flib::TextButton>(sf::Vector2f(510, 450),
                                                                sf::Vector2f(225, 50),
                                                                sf::Text("Quit", app.font(), 25),
                                                                sf::Color::Red);
    quit_button->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>(
        [&app](auto)
        {
            app.close();
        }));

    const auto menu_scene = std::make_shared<flib::Scene>();
    const auto menu_layer = std::make_shared<flib::Layer>();
    menu_layer->addDrawable(title);
    menu_layer->addDrawable(play_button);
    menu_layer->addDrawable(quit_button);

    menu_scene->addLayer("menu", menu_layer);

    Sokoban sokoban;
    sokoban.onGameWon.connect(sling::Slot<>([&app]
    {
        flib::MessageBox msg("You won!", sf::Text("Congratulations!", app.font()));
        const auto ok_btn = msg.addButton("Ok");
        ok_btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>(
            [&app, &msg](auto)
            {
                msg.hide();
                app.setActiveScene("menu");
            }));

        const auto quit_btn = msg.addButton("Quit");
        quit_btn->onClick.connect(sling::Slot<std::shared_ptr<flib::Button>>(
            [&app, &msg](auto)
            {
                msg.hide();
                app.close();
            }));

        msg.show(app);
    }));

    app.addScene("menu", menu_scene);
    app.addScene("game", sokoban.scene());
    app.setActiveScene("menu");
    app.run();

    return 0;
}
