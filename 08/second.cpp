#include <vector>
#include <fstream>
#include <iostream>

enum direction_t
{
    up,
    down,
    left,
    right
};

std::vector<std::vector<uint8_t>> grid;

unsigned int checkGridVisibility(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, direction_t dir)
{
    // edge is always visible
    if (dir == up && y2 >= 1)
    {
        if (grid[y2 - 1][x2] >= grid[y1][x1])
            return 1;
        else
            return 1 + checkGridVisibility(x1, y1, x2, y2 - 1, dir);
    }
    else if (dir == down && y2 <= grid[0].size() - 2)
    {
        if (grid[y2 + 1][x2] >= grid[y1][x1])
            return 1;
        else
            return 1 + checkGridVisibility(x1, y1, x2, y2 + 1, dir);
    }
    else if (dir == left && x2 >= 1)
    {
        if (grid[y2][x2 - 1] >= grid[y1][x1])
            return 1;
        else
            return 1 + checkGridVisibility(x1, y1, x2 - 1, y2, dir);
    }
    else if (dir == right && x2 <= grid.size() - 2)
    {
        if (grid[y2][x2 + 1] >= grid[y1][x1])
            return 1;
        else
            return 1 + checkGridVisibility(x1, y1, x2 + 1, y2, dir);
    }

    return 0;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    unsigned int max = 0;

    while (input >> line)
    {
        std::vector<uint8_t> tmp;

        for (size_t j = 0; j < line.size(); ++j)
            tmp.push_back(line.at(j));

        grid.push_back(tmp);
    }

    for (size_t i = 1; i < grid.size() - 1; ++i)
    {
        for (size_t j = 1; j < grid[0].size() - 1; ++j)
        {
            std::cout << "checking " << j << ":" << i << " (" << grid[i][j] << ") ";
            unsigned int scene = checkGridVisibility(j, i, j, i, up) * checkGridVisibility(j, i, j, i, down) * checkGridVisibility(j, i, j, i, left) * checkGridVisibility(j, i, j, i, right);
            std::cout << scene;
            if (scene > max)
            {
                max = scene;
                std::cout << " -> max";
            }

            std::cout << "\n";
        }
    }

    std::cout << max << "\n";

    return 0;
}