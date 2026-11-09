//
// Created by Ziyang Zhang on 2019-05-03.
//

#include "Utils.h"

// A board square: x = row, y = column.
Position::Position(int x,int y)
    :x(x),y(y)
{
}

// pos[0] = row, pos[1] = column; any other index throws IndexOutOfBoundError.
int& Position::operator[] (const int& index)
{
    if (index == 0)
    {
        return this->x;
    }
    else if (index == 1)
    {
        return this->y;
    }
    else
    {
        throw IndexOutOfBoundError();
    }
}

// Two positions are equal if both row and column match.
bool Position::operator== (const Position& lp) const
{
    return lp.x == x && lp.y == y;
}

// Orders positions by row, then column (lets Position be used in sets/maps).
bool Position::operator< (const Position& rp) const
{
    if (x < rp.x)
    {
        return true;
    }
    else if (x == rp.x)
    {
        if (y <  rp.y)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }



}


// Forward directions for a color: B moves down (+1 row), W moves up (-1 row), each to the left or right.
std::vector<Position> Direction::operator[] (const std::string& index)
{
    return list.at(index);
}
