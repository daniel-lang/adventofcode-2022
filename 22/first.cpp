#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct point
{
    int x;
    int y;
};

struct instruction
{
    bool move;
    int distance;
    int turn;
};

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void print(int maxX, int maxY, std::unordered_map<std::string, bool> &map, point position, int facing)
{
    for (int y = 1; y < maxY; y++)
    {
        for (int x = 1; x < maxX; x++)
        {
            if (position.x == x && position.y == y)
            {
                if (facing == 0)
                    std::cout << ">";
                else if (facing == 1)
                    std::cout << "v";
                else if (facing == 2)
                    std::cout << "<";
                else
                    std::cout << "^";
            }
            else if (map.count(pointToString({x, y})) > 0)
            {
                if (map[pointToString({x, y})])
                    std::cout << ".";
                else
                    std::cout << "#";
            }
            else
                std::cout << " ";
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;

    int y = 1;
    int x = 1;
    point position = {-1, -1};
    int facing = 0;
    std::unordered_map<std::string, bool> grid;
    std::vector<instruction> instructions;
    int maxX = 1;

    while (std::getline(input, line))
    {
        if (line.find(".") != std::string::npos)
        {
            x = 1;

            for (char c : line)
            {
                if (c == '.')
                {
                    if (y == 1 && position.x == -1 && position.y == -1)
                    {
                        position.x = x;
                        position.y = y;
                    }

                    grid.insert({pointToString({x, y}), true});
                }
                else if (c == '#')
                {
                    grid.insert({pointToString({x, y}), false});
                }

                x++;
            }

            if (x > maxX)
                maxX = x;

            y++;
        }
        else if (!line.empty())
        {
            for (size_t i = 0; i < line.size(); ++i)
            {
                if (line.at(i) == 'R')
                    instructions.push_back({false, 0, 1});
                else if (line.at(i) == 'L')
                    instructions.push_back({false, 0, -1});
                else
                {
                    int tmp = 0;
                    std::sscanf(line.substr(i).c_str(), "%d", &tmp);
                    instructions.push_back({true, tmp, 0});

                    if (tmp >= 100)
                        i += 2;
                    else if (tmp >= 10)
                        i++;
                }
            }
        }
    }

    int maxY = y;

    for (instruction const &i : instructions)
    {
        if (!i.move)
        {
            facing += i.turn;
            if (facing == -1)
                facing = 3;
            if (facing == 4)
                facing = 0;

            continue;
        }

        bool movePossible = true;
        for (int m = 0; movePossible && m < i.distance; ++m)
        {
            point newPosition = position;
            if (facing == 0)
                newPosition.x++;
            else if (facing == 1)
                newPosition.y++;
            else if (facing == 2)
                newPosition.x--;
            else
                newPosition.y--;

            std::string p = pointToString(newPosition);
            if (grid.count(p) == 1)
            {
                if (!grid[p])
                    movePossible = false;
                else
                    position = newPosition;
            }
            else
            {
                bool nextFound = false;
                if (facing == 0)
                    newPosition = {0, position.y};
                else if (facing == 1)
                    newPosition = {position.x, 0};
                else if (facing == 2)
                    newPosition = {maxX, position.y};
                else
                    newPosition = {position.x, maxY};

                while (!nextFound)
                {
                    std::string p = pointToString(newPosition);
                    if (grid.count(p) == 1)
                    {
                        nextFound = true;
                        if (!grid[p])
                            movePossible = false;
                        else
                            position = newPosition;
                    }
                    else
                    {
                        if (facing == 0)
                            newPosition.x++;
                        else if (facing == 1)
                            newPosition.y++;
                        else if (facing == 2)
                            newPosition.x--;
                        else
                            newPosition.y--;
                    }
                }
            }
        }

        // print(maxX, maxY, grid, position, facing);
    }

    std::cout << "facing: " << facing << " x: " << position.x << " y: " << position.y << "\n";
    std::cout << (1000 * position.y + 4 * position.x + facing) << std::endl;

    return 0;
}