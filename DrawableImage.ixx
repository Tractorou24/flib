﻿export module FLib.DrawableImage;

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
        /**
         * \brief Creates a drawable image from a file
         * \param file_path The file path of the base image
         */
        explicit DrawableImage(const std::string& file_path);

        /**
         * \brief Creates a drawable image from a texture loaded in memory
         * \param texture A shared pointer to the texture to use
         */
        explicit DrawableImage(const std::shared_ptr<sf::Texture>& texture);
        ~DrawableImage() override = default;

        /**
         * \brief Draws the buttons into the target
         * \param target The target to draw the button into
         * \param states The states used to render the button
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        /**
         * \return The position of the image from the top left corner of the scene
         */
        const sf::Vector2f& position() const noexcept { return m_sprite->getPosition(); }

        /**
         * \return The scale factor of the image
         */
        const sf::Vector2f& scale() const noexcept { return m_sprite->getScale(); }

        /**
         * \return The underlying SFML texture
         */
        const sf::Texture& texture() const noexcept { return *m_sprite->getTexture(); }

        /**
         * \return The underlying SFML sprite
         */
        const sf::Sprite& sprite() const noexcept { return *m_sprite; }


        /**
         * \brief Moves the image to the given position
         * \param position The new position of the image
         */
        void setPosition(const sf::Vector2f& position) noexcept { m_sprite->setPosition(position); }

        /**
         * \brief Scales the image by the given factor
         * \param scale The new factor to scale the image to
         */
        void setScale(const sf::Vector2f& scale) noexcept { m_sprite->setScale(scale); }

    private:
        std::shared_ptr<sf::Texture> m_texture;
        std::unique_ptr<sf::Sprite> m_sprite;
    };
}

module: private;

namespace flib
{
    DrawableImage::DrawableImage(const std::string& file_path)
    {
        m_texture->loadFromFile(file_path);
        m_sprite = std::make_unique<sf::Sprite>(*m_texture);
    }

    DrawableImage::DrawableImage(const std::shared_ptr<sf::Texture>& texture)
        : m_texture(texture)
    {
        m_sprite = std::make_unique<sf::Sprite>(*m_texture);
    }

    void DrawableImage::draw(sf::RenderTarget& target, sf::RenderStates) const
    {
        target.draw(*m_sprite);
    }
}
