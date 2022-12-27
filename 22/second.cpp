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

void print(int maxX, int maxY, std::unordered_map<std::string, bool> &map, std::unordered_map<std::string, int> &path)
{
    for (int y = 1; y < maxY; y++)
    {
        for (int x = 1; x < maxX; x++)
        {
            auto p = pointToString({x, y});
            auto it = path.find(p);
            if (it != path.cend())
            {
                if (it->second == 0)
                    std::cout << ">";
                else if (it->second == 1)
                    std::cout << "v";
                else if (it->second == 2)
                    std::cout << "<";
                else
                    std::cout << "^";
            }
            else if (map.count(p) > 0)
            {
                if (map[p])
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

    int cubeSize = atoi(argv[2]);
    int y = 1;
    int x = 1;
    point position = {-1, -1};
    int facing = 0;
    std::unordered_map<std::string, bool> grid;
    std::unordered_map<std::string, int> path;
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
    std::cout << maxX << "/" << maxY << "\n";
    path.insert({pointToString(position), facing});

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
                else {
                    position = newPosition;
                    path.insert({pointToString(position), facing});
                }
            }
            else
            {
                point side;
                point relativePosition;
                side.x = (position.x -1) / cubeSize + 1;
                side.y = (position.y -1) / cubeSize + 1;
                relativePosition.x = ((position.x - 1) % cubeSize) + 1;
                relativePosition.y = ((position.y - 1) % cubeSize) + 1;

                /*
                * example:
                *         3/1
                * 1/2 2/2 3/2
                *         3/3 4/3
                * 
                * input:
                *     2/1 3/1
                *     2/2
                * 1/3 2/3
                * 1/4
                */

                std::cout << position.x << "/" << position.y << " facing " << facing
                << " edge on side " << side.x << "/" << side.y
                << ", relative " << relativePosition.x << "/" << relativePosition.y << "\n";

                int newFacing = -1;
                point newPosition;

                if (cubeSize == 4) // example
                {
                    if (facing == 0 && side.x == 3 && side.y == 2)
                    {
                        /* 3/2 -> 4/3 */
                        newPosition.x = 3 * cubeSize + 1 + (cubeSize - relativePosition.y);
                        newPosition.y = 2 * cubeSize + 1;
                        newFacing = 1; // down
                    }
                    else if (facing == 1 && side.x == 3 && side.y == 3)
                    {
                        /* 3/3 -> 1/2 */
                        newPosition.x = 1 + cubeSize - relativePosition.x;
                        newPosition.y = 2 * cubeSize;
                        newFacing = 3; // up
                    }
                    else if (facing == 3 && side.x == 2 && side.y == 2)
                    {
                        /* 2/2 -> 3/1 */
                        newPosition.y = relativePosition.x;
                        newPosition.x = cubeSize * 2 + 1;
                        newFacing = 0; // right
                    }
                }
                else // input
                {
                    if (side.x == 2 && side.y == 1)
                    {
                        if (facing == 2) // left
                        {
                            // 1/3, 180°
                            newPosition.x = 1;
                            newPosition.y = cubeSize * 2 + (cubeSize - relativePosition.y) + 1;
                            newFacing = 0;
                        }
                        else if (facing == 3) // up
                        {
                            // 1/4, -90°
                            newPosition.x = 1;
                            newPosition.y = cubeSize * 3 + relativePosition.x;
                            newFacing = 0; // right
                        }
                    }
                    else if (side.x == 3 && side.y == 1)
                    {
                        if (facing == 0) // right
                        {
                            // 2/3, 180°
                            newPosition.x = cubeSize * 2;
                            newPosition.y = cubeSize * 2 + (cubeSize - relativePosition.y) + 1;
                            newFacing = 2;
                        }
                        else if (facing == 1) // down
                        {
                            // 2/2, 90°
                            newPosition.x = cubeSize * 2;
                            newPosition.y = cubeSize + relativePosition.x;
                            newFacing = 2; // left
                        }
                        else if (facing == 3) // up
                        {
                            // 1/4, 0°
                            newPosition.x = relativePosition.x;
                            newPosition.y = cubeSize * 4;
                            newFacing = 3; // up
                        }
                    }
                    else if (side.x == 2 && side.y == 2)
                    {
                        if (facing == 0) // right
                        {
                            // 3/1, 90°
                            newPosition.x = cubeSize * 2 + relativePosition.y;
                            newPosition.y = cubeSize;
                            newFacing = 3; // up
                        }
                        else if (facing == 2) // left
                        {
                            // 1/3, 90°
                            newPosition.x = relativePosition.y;
                            newPosition.y = cubeSize * 2 + 1;
                            newFacing = 1; // down
                        }
                    }
                    else if (side.x == 2 && side.y == 3)
                    {
                        if (facing == 0) // right
                        {
                            // 3/1, 180°
                            newPosition.x = cubeSize * 3;
                            newPosition.y = cubeSize - relativePosition.y + 1;
                            newFacing = 2; // left
                        }
                        else if (facing == 1) // down
                        {
                            // 1/4, -90°
                            newPosition.x = cubeSize;
                            newPosition.y = cubeSize * 3 + relativePosition.x;
                            newFacing = 2; // left
                        }
                    }
                    else if (side.x == 1 && side.y == 3)
                    {
                        if (facing == 2) // left
                        {
                            // 2/1, 180°
                            newPosition.x = cubeSize + 1;
                            newPosition.y = cubeSize - relativePosition.y + 1;
                            newFacing = 0; // right
                        }
                        else if (facing == 3) // up
                        {
                            // 2/2, 90°
                            newPosition.x = cubeSize + 1;
                            newPosition.y = cubeSize + relativePosition.x;
                            newFacing = 0; // right
                        }
                    }
                    else if (side.x == 1 && side.y == 4)
                    {
                        if (facing == 0) // right
                        {
                            // 2/3, 90°
                            newPosition.x = cubeSize + relativePosition.y;
                            newPosition.y = cubeSize * 3;
                            newFacing = 3; // up

                        }
                        else if (facing == 1) // down
                        {
                            // 3/1, 0°
                            newPosition.x = cubeSize * 2 + relativePosition.x;
                            newPosition.y = 1;
                            newFacing = 1; // down
                        }
                        else if (facing == 2) // left
                        {
                            // 2/1, 90°
                            newPosition.x = cubeSize + relativePosition.y;
                            newPosition.y = 1;
                            newFacing = 1; // down
                        }
                    }
                }

                if (newFacing == -1) {
                    print(maxX, maxY, grid, path);
                    return 0;
                }

                std::string p = pointToString(newPosition);
                if (!grid[p])
                {
                    movePossible = false;
                }
                else
                {
                    position = newPosition;
                    facing = newFacing;
                    path.insert({pointToString(position), facing});
                    print(maxX, maxY, grid, path);
                }
            }
        }
    }

    std::cout << "facing: " << facing << " x: " << position.x << " y: " << position.y << "\n";
    std::cout << (1000 * position.y + 4 * position.x + facing) << std::endl;

    return 0;
}