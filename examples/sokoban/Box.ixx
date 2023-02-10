export module Box;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;

import FLib.DrawableImage;

namespace sokoban
{
    export class Box final : public sf::Drawable
    {
    public:
        explicit Box(const sf::Vector2f& position, const std::shared_ptr<sf::Texture>& image)
            : m_image(image) {}

        flib::DrawableImage& image() noexcept { return m_image; }
        sf::FloatRect getGlobalBounds() const { return m_image.sprite().getGlobalBounds(); }

    protected:
        void draw(sf::RenderTarget& target, const sf::RenderStates states) const override
        {
            target.draw(m_image, states);
        }

    private:
        flib::DrawableImage m_image;
    };
}
