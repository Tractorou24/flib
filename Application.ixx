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

        void addScene(const std::string& name, const std::shared_ptr<Scene>&);
        void removeScene(const std::string& name);

        void setActiveScene(const std::string& name);

        [[nodiscard]] const std::shared_ptr<Scene>& activeScene() { return m_activeScene; }
        [[nodiscard]] sf::RenderWindow& window() { return m_renderWindow; }
        [[nodiscard]] sf::Font& font() { return m_font; }

    private:
        sf::RenderWindow m_renderWindow;
        sf::Font m_font;

        std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
        std::shared_ptr<Scene> m_activeScene;
    };
}

module: private;

namespace flib
{
    Application::Application(const sf::VideoMode mode, const sf::String& title, const sf::Uint32 style,
                             const sf::ContextSettings& settings)
        : m_renderWindow(mode, title, style, settings)
    {
        m_font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    }

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
                    m_activeScene->handleButtons(sf::Mouse::getPosition(m_renderWindow));
                m_activeScene->onEvent.emit(event);
            }

            m_renderWindow.clear();
            if (m_activeScene)
                m_renderWindow.draw(*m_activeScene);
            m_activeScene->onDraw.emit();
            m_renderWindow.display();
        }
    }

    void Application::close()
    {
        m_renderWindow.close();
    }

    void Application::addScene(const std::string& name, const std::shared_ptr<Scene>& scene)
    {
        m_scenes.insert_or_assign(name, scene);
    }

    void Application::removeScene(const std::string& name)
    {
        m_scenes.erase(name);
    }

    void Application::setActiveScene(const std::string& name)
    {
        if (!m_scenes.contains(name))
            throw std::runtime_error("Scene is not added in the application !");
        m_activeScene = m_scenes[name];
    }
}
