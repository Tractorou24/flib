export module FLib.Button;

import <SFML/Graphics.hpp>;

import <functional>;

import Sling;

namespace flib
{
    /**
    * \brief This class represents a clickable button
    * \code
    * Example:
    *       auto button = std::make_shared<flib::Button>(sf::Vector2f(0, 0), sf::Vector2f(100, 100));
    *       button->setBackgroundColor(sf::Color::Red);
    *       button->setOutlineColor(sf::Color::White);
    *       button->setOutlineThickness(2);
    *       button->onClick.connect(sling::Slot<Button*>([](Button* button) { std::cout << "Button clicked!" << std::endl; }));
    * \endcode
    */
    export class Button : public sf::Drawable
    {
    public:
        /**
         * \brief Creates a new button
         * \param position The position in the scene in pixels from the top left corner of the scene
         * \param size The size of the button in pixels
         * \param background_color The color of the background (defaulted: Transparent)
         * \param outline_color The color of the button's outline (default: Transparent)
         * \param outline_thickness The size of the button's outline (default: 0)
         */
        explicit Button(const sf::Vector2f& position, const sf::Vector2f& size,
                        const sf::Color& background_color = sf::Color::Transparent,
                        const sf::Color& outline_color = sf::Color::Transparent, const float& outline_thickness = 0.0f);
        ~Button() override = default;

        Button(const Button& other) = delete;
        Button(Button&& other) noexcept;
        Button& operator=(const Button& other) = delete;
        Button& operator=(Button&& other) noexcept;

        /**
         * \return The button current position in pixels from the top left corner of the scene
         */
        [[nodiscard]] virtual sf::Vector2f position() const noexcept { return m_shape.getPosition(); }

        /**
         * \return The button current size in pixels
         */
        [[nodiscard]] virtual sf::Vector2f size() const noexcept { return m_shape.getSize(); }

        /**
         * \return A copy of the button current shape
         */
        [[nodiscard]] virtual sf::RectangleShape shape() const noexcept { return m_shape; }

        /**
         * \return The button current background color
         */
        [[nodiscard]] virtual sf::Color backgroundColor() const noexcept { return m_shape.getFillColor(); }

        /**
         * \return The button current outline color
         */
        [[nodiscard]] virtual sf::Color outlineColor() const noexcept { return m_shape.getOutlineColor(); }

        /**
         * \return The button current outline thickness
         */
        [[nodiscard]] virtual float outlineThickness() const noexcept { return m_shape.getOutlineThickness(); }

        /**
         * \return The button current global bounds
         */
        [[nodiscard]] virtual sf::FloatRect globalBounds() const noexcept { return m_shape.getGlobalBounds(); }


        /**
         * \brief Moves the button to the new position
         * \param new_position The new position
         */
        void virtual setPosition(const sf::Vector2f& new_position) noexcept { m_shape.setPosition(new_position); }

        /**
         * \brief Sets the size of the button to the new size
         * \param new_size The new size
         */
        void virtual setSize(const sf::Vector2f& new_size) noexcept { m_shape.setSize(new_size); }

        /**
         * \brief Sets the background color of the button to the new color
         * \param new_color The new background color
         */
        void virtual setBackgroundColor(const sf::Color& new_color) noexcept { m_shape.setFillColor(new_color); }

        /**
         * \brief Sets the outline color of the button to the new color
         * \param new_color The new outline color
         */
        void virtual setOutlineColor(const sf::Color& new_color) noexcept { m_shape.setOutlineColor(new_color); }

        /**
         * \brief Sets the outline thickness of the button to the new thickness
         * \param new_thickness The new thickness value
         */
        void virtual setOutlineThickness(const float& new_thickness) noexcept
        {
            m_shape.setOutlineThickness(new_thickness);
        }

        /**
         * \brief Draws the buttons into the target
         * \param target The target to draw the button into
         * \param states The states used to render the button
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    public:
        /**
         * \brief The signal emitted when the button is clicked
         * \code
         * Example:
         *      button.onClick.connect(sling::Slot<Button*>([](Button* button) { std::cout << "Button clicked!" << std::endl; }));
         * \endcode
         */
        sling::Signal<std::shared_ptr<Button>> onClick;

    protected:
        sf::RectangleShape m_shape;
    };
}

module: private;

namespace flib
{
    Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& background_color,
                   const sf::Color& outline_color, const float& outline_thickness)
    {
        m_shape.setPosition(position);
        m_shape.setSize(size);
        m_shape.setFillColor(background_color);
        m_shape.setOutlineColor(outline_color);
        m_shape.setOutlineThickness(outline_thickness);
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
