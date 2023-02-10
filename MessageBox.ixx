export module FLib.MessageBox;

import <SFML/Graphics/RectangleShape.hpp>;
import <SFML/Graphics/RenderWindow.hpp>;
import <SFML/Graphics/Shader.hpp>;
import <SFML/Graphics/Text.hpp>;
import <SFML/System/Thread.hpp>;
import <SFML/Window/Event.hpp>;

import <algorithm>;
import <numeric>;
import <stdexcept>;
import <string>;

import FLib.Application;
import FLib.Button;
import FLib.TextButton;

namespace flib
{
    export class MessageBox final
    {
    public:
        explicit MessageBox(const sf::Font& font);
        explicit MessageBox(const std::string& title, const sf::Text& message);

        std::shared_ptr<TextButton> addButton(const std::string& text);
        void removeButton(const std::string& text);

        void show(Application& app);
        void hide();

        [[nodiscard]] std::string title() const { return m_title; }
        [[nodiscard]] sf::Text message() const { return m_message; }

        void setTitle(const std::string& title) { m_title = title; }
        void setMessage(const std::string& message) { m_message = sf::Text(message, m_font); }

    private:
        sf::Font m_font;
        std::string m_title;
        sf::Text m_message;
        std::vector<std::shared_ptr<TextButton>> m_buttons;

        bool m_isShown = false;
    };
}

module: private;

namespace flib
{
    MessageBox::MessageBox(const sf::Font& font)
        : m_font(font) { }

    MessageBox::MessageBox(const std::string& title, const sf::Text& message)
        : m_title(title), m_message(message)
    {
        m_font = *m_message.getFont();
    }

    std::shared_ptr<TextButton> MessageBox::addButton(const std::string& text)
    {
        const auto button_text = sf::Text(text, m_font);
        const auto button_size = sf::Vector2f(button_text.getLocalBounds().width + 5,
                                              button_text.getLocalBounds().height + 10);
        m_buttons.emplace_back(std::make_shared<TextButton>(sf::Vector2f(0, 0), button_size, button_text));
        return m_buttons.back();
    }

    void MessageBox::removeButton(const std::string& text)
    {
        std::erase_if(m_buttons, [&text](const auto& button)
        {
            return button->text().getString() == text;
        });
    }

    void MessageBox::show(Application& app)
    {
        m_isShown = true;

        sf::RectangleShape background(app.window().getView().getSize());
        background.setFillColor(sf::Color(0, 0, 0, 3));

        const sf::Vector2u& window_size = app.window().getSize();
        const sf::FloatRect& message_size = m_message.getGlobalBounds();
        m_message.setPosition(sf::Vector2f(window_size.x / 2 - message_size.width / 2,
                                           window_size.y / 2 - message_size.height / 2 - window_size.y / 6));

        const float space_size = (window_size.x - std::accumulate(m_buttons.cbegin(), m_buttons.cend(), 0.f,
                                                                  [](float acc, const auto& button)
                                                                  {
                                                                      return acc + button->size().x;
                                                                  })) / (m_buttons.size() + 1);

        float next_pos = space_size;
        for (auto& button : m_buttons)
        {
            const sf::FloatRect& button_size = button->text().getGlobalBounds();
            const float height = window_size.y / 2 - button_size.height / 2 + window_size.y / 6;
            button->setPosition(sf::Vector2f(next_pos, height));
            next_pos += (button_size.width + space_size);
        }

        sf::RenderWindow& window = app.window();
        while (m_isShown)
        {
            sf::Event event {};
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    for (auto& button : m_buttons)
                    {
                        const auto mouse_pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                        if (button->shape().getGlobalBounds().contains(mouse_pos))
                            button->onClick.emit(button);
                    }
                }
            }
            window.draw(background);
            window.draw(m_message);
            std::ranges::for_each(m_buttons, [&window](auto& button) { window.draw(*button); });
            window.display();
        }
    }

    void MessageBox::hide()
    {
        m_isShown = false;
    }
}
