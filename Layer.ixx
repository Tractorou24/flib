export module FLib.Layer;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;

import <memory>;
import <vector>;

import FLib.Button;
import FLib.TextButton;

namespace flib
{
    template <typename T>
    concept HasGlobalBounds = requires(T t)
    {
        { t.globalBounds() };
    };

    export class Layer final : sf::Drawable
    {
    public:
        Layer() = default;
        ~Layer() override;

        template <typename T>
        void addDrawable(const std::shared_ptr<T> obj) { m_drawables.push_back(std::move(obj)); }

        void removeDrawable(const std::shared_ptr<sf::Drawable> obj);

        void handleButtons(const sf::Vector2i& mouse_position) const;

        template <typename T>
        std::shared_ptr<T> getDrawable(const std::size_t& index);

        template <typename T>
        std::shared_ptr<T> getDrawableAtPosition(const sf::Vector2f& position) requires HasGlobalBounds<T>
        {
            const auto it = std::ranges::find_if(m_drawables, [&position](auto& drawable)
            {
                return std::dynamic_pointer_cast<T>(drawable)->globalBounds().contains(position);
            });

            if (it != m_drawables.end())
                return std::dynamic_pointer_cast<T>(*it);
            return nullptr;
        }

        std::vector<std::shared_ptr<sf::Drawable>> getDrawables() { return m_drawables; }

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::vector<std::shared_ptr<sf::Drawable>> m_drawables;
        std::vector<std::shared_ptr<Button>> m_buttons;
    };
}

module: private;

namespace flib
{
    Layer::~Layer()
    {
        m_drawables.clear();
        m_buttons.clear();
    }

    template <>
    void Layer::addDrawable<Button>(const std::shared_ptr<Button> obj)
    {
        m_buttons.push_back(obj);
        m_drawables.push_back(obj);
    }

    template <>
    void Layer::addDrawable<TextButton>(const std::shared_ptr<TextButton> obj)
    {
        addDrawable<Button>(std::move(obj));
    }

    template <typename T>
    std::shared_ptr<T> Layer::getDrawable(const std::size_t& index)
    {
        return std::dynamic_pointer_cast<T>(m_drawables[index]);
    }

    void Layer::removeDrawable(const std::shared_ptr<sf::Drawable> obj)
    {
        m_drawables.erase(std::remove(m_drawables.begin(), m_drawables.end(), std::move(obj)));
    }

    void Layer::handleButtons(const sf::Vector2i& mouse_position) const
    {
        for (const auto button : m_buttons)
        {
            auto toto = static_cast<sf::Vector2f>(mouse_position);
            if (button->globalBounds().contains(toto))
                button->onClick.emit(button);
        }
    }

    void Layer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        std::ranges::for_each(m_drawables, [&](const auto& obj)
        {
            target.draw(*obj, states);
        });
    }
}
