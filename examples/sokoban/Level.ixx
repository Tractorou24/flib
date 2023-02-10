export module Sokoban.Level;

import <SFML/System/Vector2.hpp>;

import <algorithm>;
import <format>;
import <fstream>;
import <memory>;
import <numeric>;
import <stdexcept>;
import <string>;
import <utility>;
import <vector>;

import FLib.DrawableImage;
import FLib.Layer;
import FLib.Scene;

import Sokoban.Textures;
import Sokoban.TileType;

export class Level
{
public:
    static Level GetFromFile(const int& level_nb);

public:
    std::shared_ptr<flib::Scene> generateScene();

    TileType tileAt(const std::size_t& x, const std::size_t& y) const { return m_tiles[x][y]; }

    void setTileAt(const std::size_t& x, const std::size_t& y, const TileType& tile_type);
    void swapTiles(const std::size_t& x1, const std::size_t& y1, const std::size_t& x2, const std::size_t& y2);

    bool isGameWin() const noexcept;

private:
    Level() = default;

    void create_tile_on_layer(const std::shared_ptr<flib::Layer>& layer, const TileType& tile_type,
                              const std::pair<std::size_t, std::size_t>& grid_pos);

private:
    std::size_t m_goalsCount = 0;
    std::vector<std::vector<TileType>> m_tiles;
};

module: private;

Level Level::GetFromFile(const int& level_nb)
{
    Level level;

    std::fstream file(std::format("Assets/Levels/level_{0}.txt", level_nb), std::ios::in);
    if (!file.is_open())
        throw std::runtime_error("Could not open file");

    std::string line;
    while (std::getline(file, line))
    {
        std::vector<TileType> line_images;
        line_images.reserve(line.size());
        for (const char& c : line)
            line_images.push_back(static_cast<TileType>(c));
        level.m_tiles.push_back(line_images);
    }

    level.m_goalsCount = std::accumulate(level.m_tiles.begin(), level.m_tiles.end(), static_cast<std::size_t>(0),
                                         [](const std::size_t& count, const std::vector<TileType>& line)
                                         {
                                             return count + std::ranges::count(line, TileType::Goal);
                                         });

    return level;
}

std::shared_ptr<flib::Scene> Level::generateScene()
{
    auto scene = std::make_shared<flib::Scene>();
    const auto level_layer = std::make_shared<flib::Layer>();
    const auto box_layer = std::make_shared<flib::Layer>();
    const auto player_layer = std::make_shared<flib::Layer>();

    for (std::size_t i = 0; i < m_tiles.size(); ++i)
    {
        for (std::size_t j = 0; j < m_tiles[i].size(); ++j)
        {
            switch (m_tiles[i][j])
            {
            case TileType::Player:
                {
                    /*
                     * If the tile is the player, we create a new image and add it to the player layer.
                     * We also need to create a ground tile for the player to stand on, so we create an image in the ground layer too
                     */
                    create_tile_on_layer(player_layer, m_tiles[i][j], std::make_pair(i, j));
                    break;
                }
            case TileType::Box: // Fallthrough since we want to handle the box the same way as the box on goal
            case TileType::BoxOnGoal:
                {
                    /*
                     * Specify the box since it's in the box layer
                     */
                    create_tile_on_layer(box_layer, m_tiles[i][j], std::make_pair(i, j));
                    break;
                }
            default:
                break;
            }

            /*
             * Add the ground tile every time, since top layers can be drawn over it
             * Check if the tile was already drawn by the switch statement. if yes, draw ground below
             */
            if (m_tiles[i][j] == TileType::Player || m_tiles[i][j] == TileType::Box || m_tiles[i][j] ==
                TileType::BoxOnGoal)
                create_tile_on_layer(level_layer, TileType::Empty, std::make_pair(i, j));
            else
                create_tile_on_layer(level_layer, m_tiles[i][j], std::make_pair(i, j));
        }
    }

    scene->addLayer("level", level_layer);
    scene->addLayer("boxes", box_layer);
    scene->addLayer("player", player_layer);

    return scene;
}

void Level::setTileAt(const std::size_t& x, const std::size_t& y, const TileType& tile_type)
{
    m_tiles[x][y] = tile_type;
}

void Level::swapTiles(const std::size_t& x1, const std::size_t& y1, const std::size_t& x2, const std::size_t& y2)
{
    std::swap(m_tiles[x1][y1], m_tiles[x2][y2]);
}

bool Level::isGameWin() const noexcept
{
    return std::accumulate(m_tiles.begin(), m_tiles.end(), static_cast<std::size_t>(0),
                           [](const std::size_t& count, const std::vector<TileType>& line)
                           {
                               return count + std::ranges::count(
                                          line, TileType::BoxOnGoal);
                           }) == m_goalsCount;
}

void Level::create_tile_on_layer(const std::shared_ptr<flib::Layer>& layer, const TileType& tile_type,
                                 const std::pair<std::size_t, std::size_t>& grid_pos)
{
    static const Textures::Map& textures = Textures::GetTextureMap();

    auto object = std::make_shared<flib::DrawableImage>(textures.at(tile_type));
    object->setScale(sf::Vector2f(0.5, 0.5));
    object->setPosition(sf::Vector2f(static_cast<float>(grid_pos.second * 64),
                                     static_cast<float>(grid_pos.first * 64)));
    layer->addDrawable(object);
}
