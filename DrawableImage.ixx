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
        explicit DrawableImage(const std::shared_ptr<sf::Texture>& texture);
        ~DrawableImage() override = default;

        const sf::Vector2f& position() const noexcept { return m_sprite->getPosition(); }
        const sf::Vector2f& scale() const noexcept { return m_sprite->getScale(); }
        const sf::Texture& texture() const noexcept { return *m_sprite->getTexture(); }
        const sf::Sprite& sprite() const noexcept { return *m_sprite; }
        void setPosition(const sf::Vector2f& position) noexcept { m_sprite->setPosition(position); }
        void setScale(const sf::Vector2f& scale) noexcept { m_sprite->setScale(scale); }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::shared_ptr<sf::Texture> m_texture;
        std::shared_ptr<sf::Sprite> m_sprite;
    };
}

module: private;

namespace flib
{
    DrawableImage::DrawableImage(const std::string& file_path)
    {
        m_texture->loadFromFile(file_path);
        m_sprite = std::make_shared<sf::Sprite>(*m_texture);
    }

    DrawableImage::DrawableImage(const std::shared_ptr<sf::Texture>& texture)
        : m_texture(texture)
    {
        m_sprite = std::make_shared<sf::Sprite>(*m_texture);
    }

    void DrawableImage::draw(sf::RenderTarget& target, sf::RenderStates) const
    {
        target.draw(*m_sprite);
    }
}
