export module FLib.TextButton;

import <SFML/Graphics.hpp>;

import FLib.Button;

namespace flib
{
    /**
    * \brief This class represents a clickable button with a text inside it
    * \code
    * Example:
    *       const sf::Font font("arial.ttf");
    *       auto button = std::make_shared<flib::Button>(sf::Vector2f(0, 0), sf::Vector2f(100, 100), sf::Text("Hello World", font));
    *       button->onClick.connect(sling::Slot<Button*>([](Button* button) { std::cout << "Button clicked!" << std::endl; }));
    * \endcode
    */
    export class TextButton final : public Button
    {
    public:
        /**
         * \brief Creates a new text button
         * \param position The position in the scene in pixels from the top left corner of the scene
         * \param size The size of the button in pixels
         * \param text The text to display in the button
         * \param background_color The color of the background (defaulted: Transparent)
         * \param outline_color The color of the button's outline (default: Transparent)
         * \param outline_thickness The size of the button's outline (default: 0)
         */
        explicit TextButton(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Text& text,
                            const sf::Color& background_color = sf::Color::Transparent,
                            const sf::Color& outline_color = sf::Color::Transparent,
                            const float& outline_thickness = 0.0f);

        /**
         * \brief Draws the buttons into the target
         * \param target The target to draw the button into
         * \param states The states used to render the button
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        /**
         * \return The text displayed in the button
         */
        [[nodiscard]] const sf::Text& text() const noexcept { return m_text; }


        /**
         * \brief Sets the size of the button to the new size
         * \param new_size The new size
         */
        void setSize(const sf::Vector2f& new_size) noexcept override;

        /**
         * \brief Moves the button to the new position
         * \param new_position The new position
         */
        void setPosition(const sf::Vector2f& new_position) noexcept override;

        /**
         * \brief Sets the text of the button
         * \param new_text The new text
         */
        void setText(const sf::Text& new_text) { m_text = new_text; }

    private:
        /**
         * \brief Recalculates the text position to the center and move it.
         */
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
