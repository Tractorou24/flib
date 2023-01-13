export module FLib.Button;

import <SFML/Graphics.hpp>;

import <functional>;

import Sling;

namespace flib
{
    export class Button : public sf::Drawable
    {
    public:
        static void HandleButtons(const sf::Vector2i& mouse_position);

    public:
        explicit Button(const sf::Vector2f& position, const sf::Vector2f& size,
                        const sf::Color& background_color = sf::Color::Transparent,
                        const sf::Color& outline_color = sf::Color::Transparent, const float& outline_thickness = 0.0f);
        ~Button() override;

        Button(const Button& other) = delete;
        Button(Button&& other) noexcept;
        Button& operator=(const Button& other) = delete;
        Button& operator=(Button&& other) noexcept;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        [[nodiscard]] virtual sf::Vector2f position() const noexcept { return m_shape.getPosition(); }
        [[nodiscard]] virtual sf::Vector2f size() const noexcept { return m_shape.getSize(); }
        [[nodiscard]] virtual sf::RectangleShape shape() const noexcept { return m_shape; }
        [[nodiscard]] virtual sf::Color backgroundColor() const noexcept { return m_shape.getFillColor(); }
        [[nodiscard]] virtual sf::Color outlineColor() const noexcept { return m_shape.getOutlineColor(); }
        [[nodiscard]] virtual float outlineThickness() const noexcept { return m_shape.getOutlineThickness(); }

        void virtual setPosition(const sf::Vector2f& new_position) noexcept { m_shape.setPosition(new_position); }
        void virtual setSize(const sf::Vector2f& new_size) noexcept { m_shape.setSize(new_size); }
        void virtual setBackgroundColor(const sf::Color& new_color) noexcept { m_shape.setFillColor(new_color); }
        void virtual setOutlineColor(const sf::Color& new_color) noexcept { m_shape.setOutlineColor(new_color); }

        void virtual setOutlineThickness(const float& new_thickness) noexcept
        {
            m_shape.setOutlineThickness(new_thickness);
        }

        sling::Signal<Button*> onClick;

    protected:
        sf::RectangleShape m_shape;
        static std::vector<Button*> s_buttons;
    };
}

module: private;

namespace flib
{
    std::vector<Button*> Button::s_buttons;

    void Button::HandleButtons(const sf::Vector2i& mouse_position)
    {
        for (auto* button : s_buttons)
        {
            if (button->m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position)))
                button->onClick.emit(button);
        }
    }

    Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& background_color,
                   const sf::Color& outline_color, const float& outline_thickness)
    {
        m_shape.setPosition(position);
        m_shape.setSize(size);
        m_shape.setFillColor(background_color);
        m_shape.setOutlineColor(outline_color);
        m_shape.setOutlineThickness(outline_thickness);

        s_buttons.push_back(this);
    }

    Button::~Button()
    {
        std::erase(s_buttons, this);
    }

    Button::Button(Button&& other) noexcept
        : onClick {std::move(other.onClick)},
          m_shape {std::move(other.m_shape)} {}

    Button& Button::operator=(Button&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_shape = std::move(other.m_shape);
        onClick = std::move(other.onClick);
        return *this;
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates) const
    {
        target.draw(m_shape);
    }
}
