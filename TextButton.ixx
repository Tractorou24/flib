export module FLib.TextButton;

import <SFML/Graphics.hpp>;

import FLib.Button;

namespace flib
{
    export class TextButton final : public Button
    {
    public:
        explicit TextButton(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Text& text,
                            const sf::Color& background_color = sf::Color::Transparent,
                            const sf::Color& outline_color = sf::Color::Transparent,
                            const float& outline_thickness = 0.0f);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void setSize(const sf::Vector2f& new_size) noexcept override;
        void setPosition(const sf::Vector2f& new_position) noexcept override;

        void setText(const sf::Text& new_text) { m_text = new_text; }
        [[nodiscard]] const sf::Text& text() const noexcept { return m_text; }

    private:
        void setTextPosition();

    private:
        sf::Text m_text;
    };
}

module: private;

namespace flib
{
    TextButton::TextButton(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Text& text,
                           const sf::Color& background_color,
                           const sf::Color& outline_color,
                           const float& outline_thickness)
        : Button(position, size, background_color, outline_color, outline_thickness), m_text(text)
    {
        setTextPosition();
        m_shape.setOutlineColor(sf::Color::White);
        m_shape.setOutlineThickness(2);
    }

    void TextButton::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        Button::draw(target, states);
        target.draw(m_text);
    }

    void TextButton::setSize(const sf::Vector2f& new_size) noexcept
    {
        Button::setSize(new_size);
        setTextPosition();
    }

    void TextButton::setPosition(const sf::Vector2f& new_position) noexcept
    {
        Button::setPosition(new_position);
        setTextPosition();
    }

    void TextButton::setTextPosition()
    {
        m_text.setPosition(sf::Vector2f(
            m_shape.getPosition().x + m_shape.getSize().x / 2 - m_text.getGlobalBounds().width / 2,
            m_shape.getPosition().y + m_shape.getSize().y / 2 - m_text.getGlobalBounds().height));
    }
}
