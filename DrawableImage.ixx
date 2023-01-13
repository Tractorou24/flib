export module FLib.DrawableImage;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;
import <SFML/Graphics/Sprite.hpp>;
import <SFML/Graphics/Texture.hpp>;

import <format>;
import <string>;

namespace flib
{
    export class DrawableImage final : public sf::Drawable
    {
    public:
        explicit DrawableImage(const std::string& file_path);
        ~DrawableImage() override = default;

        const sf::Vector2f& position() const noexcept { return m_sprite.getPosition(); }
        const sf::Vector2f& scale() const noexcept { return m_sprite.getScale(); }
        void setPosition(const sf::Vector2f& position) noexcept { m_sprite.setPosition(position); }
        void setScale(const sf::Vector2f& scale) noexcept { m_sprite.setScale(scale); }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        sf::Texture m_texture;
        sf::Sprite m_sprite;
    };
}

module: private;

namespace flib
{
    DrawableImage::DrawableImage(const std::string& file_path)
    {
        m_texture.loadFromFile(file_path);
        m_sprite.setTexture(m_texture);
    }

    void DrawableImage::draw(sf::RenderTarget& target, sf::RenderStates) const
    {
        target.draw(m_sprite);
    }
}
