export module FLib.FunctionTracer;

import <SFML/Graphics/Color.hpp>;
import <SFML/Graphics/Drawable.hpp>;
import <SFML/Graphics/RenderTarget.hpp>;
import <SFML/Graphics/VertexArray.hpp>;

import <functional>;

namespace flib
{
    /**
     * @brief Draws a function on a render target
     */
    export class FunctionTracer final : public sf::Drawable
    {
    public:
        /**
         * @brief Creates an empty function preview
         */
        FunctionTracer() = default;

        /**
         * @brief Creates a function preview with the given function and color
         * @param function The function to draw
         * @param viewport The viewport for the function [form:(xMin, xMax)(yMin, yMax)]
         * @param color The color of the function
         */
        FunctionTracer(std::function<long double(long double)> function, std::pair<sf::Vector2f, sf::Vector2f> viewport, sf::Color color);
        ~FunctionTracer() override = default;

        FunctionTracer(const FunctionTracer& other);
        FunctionTracer(FunctionTracer&& other) noexcept;
        FunctionTracer& operator=(const FunctionTracer& other);
        FunctionTracer& operator=(FunctionTracer&& other) noexcept;

        /**
         * \return The function we are currently drawing
         */
        std::function<long double(long double)> function() const noexcept { return m_function; }

        /**
         * \return The viewport we are drawing to
         */
        std::pair<sf::Vector2f, sf::Vector2f> viewport() const noexcept { return m_viewport; }

        /**
         * \return The color of the function we are drawing
         */
        sf::Color color() const noexcept { return m_color; }


        /**
         * \brief Sets the function we will draw
         * \param function The function to use
         */
        void setFunction(std::function<long double(long double)> function) { m_function = std::move(function); }

        /**
         * @brief Sets the viewport we will draw to
         * @param viewport The viewport to use
         */
        void setViewport(std::pair<sf::Vector2f, sf::Vector2f> viewport);

        /**
         * \brief Sets the color of the function we will draw
         * \param color The color to use
         */
        void setColor(sf::Color color) { m_color = std::move(color); }

        /**
         * \brief Draws the function into the target
         * \param target The target to draw the function into
         * \param states The states used to render the function
         */
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        std::function<long double(long double)> m_function;
        std::pair<sf::Vector2f, sf::Vector2f> m_viewport;
        sf::View m_view;
        sf::Color m_color;
    };
}

module: private;

namespace flib
{
    FunctionTracer::FunctionTracer(std::function<long double(long double)> function, std::pair<sf::Vector2f, sf::Vector2f> viewport,
                                   sf::Color color)
        : m_function(std::move(function)), m_color(std::move(color))
    {
        setViewport(std::move(viewport));
    }

    FunctionTracer::FunctionTracer(const FunctionTracer& other)
        : m_function {other.m_function},
          m_viewport(other.m_viewport),
          m_view(other.m_view),
          m_color {other.m_color} { }

    FunctionTracer::FunctionTracer(FunctionTracer&& other) noexcept
        : m_function {std::move(other.m_function)},
          m_viewport(other.m_viewport),
          m_view(other.m_view),
          m_color {std::move(other.m_color)} { }

    FunctionTracer& FunctionTracer::operator=(const FunctionTracer& other)
    {
        if (this == &other)
            return *this;
        m_function = other.m_function;
        m_viewport = other.m_viewport;
        m_view = other.m_view;
        m_color = other.m_color;
        return *this;
    }


    FunctionTracer& FunctionTracer::operator=(FunctionTracer&& other) noexcept
    {
        if (this == &other)
            return *this;
        m_function = std::move(other.m_function);
        m_viewport = other.m_viewport;
        m_view = other.m_view;
        m_color = std::move(other.m_color);
        return *this;
    }

    void FunctionTracer::setViewport(const std::pair<sf::Vector2f, sf::Vector2f> viewport)
    {
        m_viewport = std::move(viewport);
        m_view = sf::View(sf::FloatRect(m_viewport.first.x, m_viewport.second.x,
                                        std::abs(m_viewport.first.x) + std::abs(m_viewport.first.y),
                                        std::abs(m_viewport.second.x) + std::abs(m_viewport.second.y)));
    }

    void FunctionTracer::draw(sf::RenderTarget& target, const sf::RenderStates states) const
    {
        const std::size_t vertex_count = target.getSize().x;
        const float step = (m_viewport.first.y - m_viewport.first.x) / static_cast<float>(target.getSize().x);

        sf::VertexArray line(sf::LinesStrip, vertex_count);
        for (unsigned int i = 0; i < vertex_count; i++)
        {
            const float x = m_viewport.first.x + static_cast<float>(i) * step;
            line[i].position = sf::Vector2f(x, static_cast<float>(std::invoke(m_function, x) * -1));
            line[i].color = m_color;
        }

        const sf::View old_view = target.getView();
        target.setView(m_view);
        target.draw(line, states);
        target.setView(old_view);
    }
}
