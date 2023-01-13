export module FLib.Scene;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderWindow.hpp>;

import <vector>;

import Sling;

namespace flib
{
    export class Scene final : public sf::Drawable
    {
    public:
        Scene() = default;

        Scene(const Scene& other) = delete;
        Scene(Scene&& other) noexcept = delete;
        Scene& operator=(const Scene& other) = delete;
        Scene& operator=(Scene&& other) noexcept = delete;

        void addObject(const std::shared_ptr<sf::Drawable>& obj);
        void removeObject(const std::shared_ptr<sf::Drawable>& obj);

        std::vector<std::shared_ptr<sf::Drawable>> getObjects() noexcept { return m_objects; }

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::vector<std::shared_ptr<sf::Drawable>> m_objects;
    };
}

module: private;

namespace flib
{
    void Scene::addObject(const std::shared_ptr<sf::Drawable>& obj)
    {
        m_objects.push_back(obj);
    }

    void Scene::removeObject(const std::shared_ptr<sf::Drawable>& obj)
    {
        m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), obj));
    }

    void Scene::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        std::ranges::for_each(m_objects, [&](const std::shared_ptr<sf::Drawable>& obj)
        {
            target.draw(*obj.get(), states);
        });
    }
}
