export module FLib.Scene;

import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderWindow.hpp>;

import <memory>;
import <ranges>;
import <unordered_map>;
import <vector>;

import FLib.Layer;
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

        void addLayer(const std::string& name, const std::shared_ptr<Layer>& obj);
        void removeLayer(const std::string& name);

        void handleButtons(const sf::Vector2i& mouse_position) const;

        std::shared_ptr<Layer> getLayer(const std::string& name) { return m_layers.at(name); }

    public:
        sling::Signal<sf::Event> onEvent;
        sling::Signal<> onDraw;

    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::unordered_map<std::string, std::shared_ptr<Layer>> m_layers;
    };
}

module: private;

namespace flib
{
    void Scene::addLayer(const std::string& name, const std::shared_ptr<Layer>& obj)
    {
        m_layers.insert_or_assign(name, obj);
    }

    void Scene::removeLayer(const std::string& name)
    {
        m_layers.erase(name);
    }

    void Scene::handleButtons(const sf::Vector2i& mouse_position) const
    {
        for (const auto layer : m_layers | std::views::values)
            layer->handleButtons(mouse_position);
    }

    void Scene::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        std::ranges::for_each(m_layers, [&](const auto& pair)
        {
            target.draw(*std::reinterpret_pointer_cast<sf::Drawable>(pair.second), states);
        });
    }
}
