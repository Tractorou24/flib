export module Sokoban;

import <SFML/Graphics/Texture.hpp>;
import <SFML/System/Vector2.hpp>;
import <SFML/Window/Event.hpp>;

import <format>;
import <fstream>;
import <map>;
import <stdexcept>;
import <string>;
import <vector>;

import Sling;

import FLib.Scene;
import FLib.DrawableImage;
import FLib.Layer;

import Sokoban.TileType;
import Sokoban.Textures;
import Sokoban.Level;

export class Sokoban
{
public:
    Sokoban();
    ~Sokoban() = default;

    Sokoban(const Sokoban& other) = delete;
    Sokoban(Sokoban&& other) noexcept = delete;
    Sokoban& operator=(const Sokoban& other) = delete;
    Sokoban& operator=(Sokoban&& other) noexcept = delete;

    void movePlayer(const int& delta_x, const int& delta_y) noexcept;
    std::shared_ptr<flib::Scene> scene() const noexcept { return m_scene; }

    sling::Signal<> onGameWon;

private:
    void replace_drawable_on_layer(const std::shared_ptr<flib::Layer>& layer,
                                   const std::shared_ptr<flib::DrawableImage>& old_drawable,
                                   const TileType& tile_type) noexcept;

private:
    Level m_level;
    std::shared_ptr<flib::Scene> m_scene;
};

module: private;

Sokoban::Sokoban()
    : m_level(Level::GetFromFile(1))
{
    m_scene = m_level.generateScene();
    m_scene->onEvent.connect(sling::Slot<sf::Event>([&](const sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Left:
                movePlayer(-1, 0);
                break;
            case sf::Keyboard::Right:
                movePlayer(1, 0);
                break;
            case sf::Keyboard::Up:
                movePlayer(0, -1);
                break;
            case sf::Keyboard::Down:
                movePlayer(0, 1);
                break;
            default:
                break;
            }
        }
    }));
}

void Sokoban::movePlayer(const int& delta_x, const int& delta_y) noexcept
{
    const std::shared_ptr<flib::Layer> box_layer = m_scene->getLayer("boxes");
    const std::shared_ptr<flib::Layer> player_layer = m_scene->getLayer("player");

    // Get the player and its position
    const auto player = std::dynamic_pointer_cast<flib::DrawableImage>(player_layer->getDrawables().front());
    const sf::Vector2f player_pos = player->position();

    // Get the tile at the next player position
    const TileType next_pos_tile = m_level.tileAt(static_cast<std::size_t>(player_pos.y / 64) + delta_y,
                                                  static_cast<std::size_t>(player_pos.x / 64) + delta_x);

    // Check if the player can move
    if (next_pos_tile == TileType::Wall)
        return;
    if (next_pos_tile == TileType::Box || next_pos_tile == TileType::BoxOnGoal)
    {
        /*
         * The player is trying to push a box.
         * Get the tile at the next possible box position.
         */
        const TileType next_box_pos_tile = m_level.tileAt(
            static_cast<std::size_t>(player_pos.y / 64) + delta_y * 2,
            static_cast<std::size_t>(player_pos.x / 64) + delta_x * 2);

        // If the next box position is a wall or another box, the player can't move
        if (next_box_pos_tile == TileType::Wall || next_box_pos_tile == TileType::Box || next_box_pos_tile ==
            TileType::BoxOnGoal)
            return;

        // Get the box
        const std::size_t box_col = static_cast<std::size_t>(player_pos.x / 64) + delta_x;
        const std::size_t box_line = static_cast<std::size_t>(player_pos.y / 64) + delta_y;

        const auto box = box_layer->getDrawableAtPosition<flib::DrawableImage>(
            sf::Vector2f(static_cast<float>(box_col * 64), static_cast<float>(box_line * 64)));

        // Move the box
        box->setPosition(sf::Vector2f(static_cast<float>(box_col + delta_x) * 64,
                                      static_cast<float>(box_line + delta_y) * 64));

        // If the next position is a goal, replace the box with a box on goal.
        if (next_box_pos_tile == TileType::Goal || next_box_pos_tile == TileType::BoxOnGoal)
        {
            replace_drawable_on_layer(box_layer, box, TileType::BoxOnGoal);

            m_level.setTileAt(box_line, box_col,
                              m_level.tileAt(box_line, box_col) != TileType::BoxOnGoal
                                  ? TileType::Empty
                                  : TileType::Goal);
            m_level.setTileAt(box_line + delta_y, box_col + delta_x, TileType::BoxOnGoal);
        } else
        {
            // If the box is already on a goal and is exiting it, replace it with a simple box.
            if (m_level.tileAt(box_line, box_col) == TileType::BoxOnGoal)
            {
                replace_drawable_on_layer(box_layer, box, TileType::Box);

                m_level.setTileAt(box_line, box_col, TileType::Goal);
                m_level.setTileAt(box_line + delta_y, box_col + delta_x, TileType::Box);
            }
            // Else, move the box in the level by swapping it with it's next position
            else
                m_level.swapTiles(box_line, box_col, box_line + delta_y, box_col + delta_x);
        }

        // If the game is over, emit the onGameWon signal.
        if (m_level.isGameWin())
            onGameWon.emit();
    }

    // Move the player
    player->setPosition(sf::Vector2f(player_pos.x + (delta_x * 64), player_pos.y + (delta_y * 64)));
}

void Sokoban::replace_drawable_on_layer(const std::shared_ptr<flib::Layer>& layer,
                                        const std::shared_ptr<flib::DrawableImage>& old_drawable,
                                        const TileType& tile_type) noexcept
{
    static const Textures::Map& textures = Textures::GetTextureMap();

    const auto elem = std::make_shared<flib::DrawableImage>(textures.at(tile_type));
    elem->setScale(sf::Vector2f(0.5, 0.5));
    elem->setPosition(old_drawable->position());

    layer->addDrawable(elem);
    layer->removeDrawable(old_drawable);
}
