export module Sokoban.Textures;

import <SFML/Graphics/Texture.hpp>;

import <map>;
import <memory>;
import <stdexcept>;

import Sokoban.TileType;

export class Textures
{
public:
    using Map = std::map<TileType, std::shared_ptr<sf::Texture>>;

public:
    static const Map& GetTextureMap();

private:
    static std::shared_ptr<sf::Texture> GetTextureFromTile(const TileType& tile);
};

module: private;

const Textures::Map& Textures::GetTextureMap()
{
    static const Map texture_map = {
        {TileType::Empty, GetTextureFromTile(TileType::Empty)},
        {TileType::Wall, GetTextureFromTile(TileType::Wall)},
        {TileType::Box, GetTextureFromTile(TileType::Box)},
        {TileType::Player, GetTextureFromTile(TileType::Player)},
        {TileType::Goal, GetTextureFromTile(TileType::Goal)},
        {TileType::BoxOnGoal, GetTextureFromTile(TileType::BoxOnGoal)},
    };
    return texture_map;
}

std::shared_ptr<sf::Texture> Textures::GetTextureFromTile(const TileType& tile)
{
    auto texture = std::make_shared<sf::Texture>();
    switch (tile)
    {
    case TileType::Empty:
        texture->loadFromFile("Assets/ground.png");
        break;
    case TileType::Wall:
        texture->loadFromFile("Assets/wall.png");
        break;
    case TileType::Box:
        texture->loadFromFile("Assets/box.png");
        break;
    case TileType::Goal:
        texture->loadFromFile("Assets/goal.png");
        break;
    case TileType::Player:
        texture->loadFromFile("Assets/player.png");
        break;
    case TileType::BoxOnGoal:
        texture->loadFromFile("Assets/box_on_goal.png");
        break;
    }

    if (!texture)
        throw std::domain_error("Unknown tile type");
    return texture;
}
