export module FLib.Application;

import <SFML/Graphics.hpp>;

import <functional>;
import <optional>;
import <stdexcept>;
import <string>;
import <thread>;

import FLib.Button;
import FLib.Scene;
import Sling;

namespace flib
{
    export class Application final
    {
    public:
        Application(sf::VideoMode mode, const sf::String& title, sf::Uint32 style = sf::Style::Default,
                    const sf::ContextSettings& settings = sf::ContextSettings());

        void run();
        void close();

        void addScene(const std::shared_ptr<Scene>&);
        void removeScene(const std::shared_ptr<Scene>&);

        void setActiveScene(const std::shared_ptr<Scene>&);
        void setActiveScene(const int& index);

        // ReSharper disable once CppMemberFunctionMayBeConst
        [[nodiscard]] const std::shared_ptr<Scene>& activeScene() { return m_activeScene; }
        [[nodiscard]] sf::RenderWindow& window() { return m_renderWindow; }

    public:
        sling::Signal<sf::Event> onEvent;
        sling::Signal<> onDraw;

    private:
        sf::RenderWindow m_renderWindow;

        std::vector<std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Scene> m_activeScene;
    };
}

module: private;

namespace flib
{
    Application::Application(const sf::VideoMode mode, const sf::String& title, const sf::Uint32 style,
                             const sf::ContextSettings& settings)
        : m_renderWindow(mode, title, style, settings) {}

    void Application::run()
    {
        while (m_renderWindow.isOpen())
        {
            sf::Event event = {};
            while (m_renderWindow.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    m_renderWindow.close();
                    return;
                }
                if (event.type == sf::Event::MouseButtonPressed)
                    Button::HandleButtons(sf::Mouse::getPosition(m_renderWindow));
                onEvent.emit(event);
            }

            m_renderWindow.clear();
            if (m_activeScene)
                m_renderWindow.draw(*m_activeScene);
            onDraw.emit();
            m_renderWindow.display();
        }
    }

    void Application::close()
    {
        m_renderWindow.close();
    }

    void Application::addScene(const std::shared_ptr<Scene>& scene)
    {
        m_scenes.push_back(scene);
    }

    void Application::removeScene(const std::shared_ptr<Scene>& scene)
    {
        m_scenes.erase(std::remove(m_scenes.begin(), m_scenes.end(), scene));
    }

    void Application::setActiveScene(const std::shared_ptr<Scene>& scene)
    {
        if (std::ranges::find(m_scenes, scene) == m_scenes.end())
            throw std::runtime_error("Scene is not added in the application !");
        m_activeScene = scene;
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void Application::setActiveScene(const int& index)
    {
        if (index < 0 || index >= m_scenes.size())
            throw std::runtime_error("Index out of range !");
        m_activeScene = m_scenes[index];
    }
}
