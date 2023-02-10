export module Sokoban.TileType;

export enum class TileType : char
{
    Empty = ' ',
    Wall = '#',
    Box = '$',
    Player = '@',
    Goal = '.',
    BoxOnGoal = '*',
};
