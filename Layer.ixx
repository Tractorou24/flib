export module FLib.Layer;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <vector>;

namespace flib
{
    export class Layer final : sf::Drawable
    {
    public:
        Layer() = default;
        ~Layer() override = default;

        void addDrawable(const std::shared_ptr<sf::Drawable>& obj);
        void removeDrawable(const std::shared_ptr<sf::Drawable>& obj);

        // @formatter:off
        template <typename T>
        std::shared_ptr<T> getDrawable(const std::size_t& index) const { return std::dynamic_pointer_cast<T>(m_drawables[index]); }
        // @formatter:on

        std::vector<std::shared_ptr<sf::Drawable>> getDrawables() { return m_drawables; }

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::vector<std::shared_ptr<sf::Drawable>> m_drawables;
    };
}

module: private;

namespace flib
{
    void Layer::addDrawable(const std::shared_ptr<sf::Drawable>& obj)
    {
        m_drawables.push_back(obj);
    }

    void Layer::removeDrawable(const std::shared_ptr<sf::Drawable>& obj)
    {
        m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), obj));
    }

    void Layer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        std::ranges::for_each(m_drawables, [&](const std::shared_ptr<sf::Drawable>& obj)
        {
            target.draw(*obj, states);
        });
    }
}
