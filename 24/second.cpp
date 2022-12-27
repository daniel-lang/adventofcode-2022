#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <map>
#include <queue>
#include <algorithm>

struct point
{
    int x;
    int y;
};

enum class blizzard_direction
{
    right,
    down,
    left,
    up,
    invalid
};

struct blizzard
{
    point position;
    blizzard_direction dir;
};

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

std::multimap<std::string, blizzard> moveBlizzards(std::multimap<std::string, blizzard> &blizzards, int maxX, int maxY)
{
    std::multimap<std::string, blizzard> tmp;

    for (auto &[key, b] : blizzards)
    {
        point newP = b.position;
        if (b.dir == blizzard_direction::right)
            newP.x++;
        else if (b.dir == blizzard_direction::down)
            newP.y++;
        else if (b.dir == blizzard_direction::left)
            newP.x--;
        else if (b.dir == blizzard_direction::up)
            newP.y--;

        if (newP.x >= maxX)
            newP.x = 1;
        if (newP.x <= 0)
            newP.x = maxX - 1;
        if (newP.y >= maxY)
            newP.y = 1;
        if (newP.y <= 0)
            newP.y = maxY - 1;

        b.position = newP;

        tmp.insert({pointToString(newP), b});
    }

    return tmp;
}

int find(std::unordered_set<std::string> const &grid, std::multimap<std::string, blizzard> &blizzards, int maxX, int maxY, point start, point end)
{
    std::queue<point> queue;
    std::vector<std::string> nextLayer;
    unsigned int nodesInNextLayer = 0;
    unsigned int nodesLeftInLayer = 1;
    unsigned int steps = 0;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    queue.push(start);
    blizzards = moveBlizzards(blizzards, maxX, maxY);

    while (queue.size() > 0)
    {
        point p = queue.front();
        queue.pop();

        if (p.x == end.x && p.y == end.y)
        {
            std::cout << steps << std::endl;
            return steps;
        }

        for (int i = 0; i < 4; i++)
        {
            point newP = p;
            newP.x += dx[i];
            newP.y += dy[i];
            auto s = pointToString(newP);

            if (newP.x < 0 || newP.x > maxX || newP.y < 0 || newP.y > maxY)
                continue;

            if (std::count(nextLayer.cbegin(), nextLayer.cend(), s) != 0)
                continue;

            if (grid.count(s) != 0 || blizzards.count(s) != 0)
                continue;

            queue.push(newP);
            nextLayer.push_back(s);
            nodesInNextLayer++;
        }

        std::string s = pointToString(p);
        if (blizzards.count(s) == 0 && std::count(nextLayer.cbegin(), nextLayer.cend(), s) == 0)
        {
            queue.push(p);
            nextLayer.push_back(s);
            nodesInNextLayer++;
        }

        nodesLeftInLayer--;
        if (nodesLeftInLayer == 0)
        {
            nodesLeftInLayer = nodesInNextLayer;
            nodesInNextLayer = 0;
            nextLayer.clear();
            std::cout << "step " << steps << " completed, queue size: " << queue.size() << "\n";
            steps++;
            blizzards = moveBlizzards(blizzards, maxX, maxY);
        }
    }

    std::cout << "ran out of elements in queue!\n";
    return -1;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;

    std::unordered_set<std::string> grid;
    std::multimap<std::string, blizzard> blizzards;

    int maxX = 0;
    int maxY = 0;

    int y = 0;
    while (std::getline(input, line))
    {
        int x = 0;
        for (auto const &c : line)
        {
            blizzard_direction dir = blizzard_direction::invalid;

            auto s = pointToString({x, y});
            if (c == '#')
                grid.insert(s);
            else if (c == '>')
                dir = blizzard_direction::right;
            else if (c == 'v')
                dir = blizzard_direction::down;
            else if (c == '<')
                dir = blizzard_direction::left;
            else if (c == '^')
                dir = blizzard_direction::up;

            if (dir != blizzard_direction::invalid)
            {
                blizzard b = {{x, y}, dir};
                blizzards.insert({pointToString({x, y}), b});
            }

            x++;
        }

        y++;
        maxX = x - 1;
    }

    maxY = y - 1;

    std::cout << "max x: " << maxX << ", y: " << maxY << "\n";

    int first = find(grid, blizzards, maxX, maxY, {1, 0}, {maxX - 1, maxY});
    std::cout << "start -> end: " << first << "\n";

    int second = 1 + find(grid, blizzards, maxX, maxY, {maxX - 1, maxY}, {1, 0});
    std::cout << "end -> start: " << second << "\n";

    int third = 1 + find(grid, blizzards, maxX, maxY, {1, 0}, {maxX - 1, maxY});
    std::cout << "start -> end: " << third << "\n";

    std::cout << "total: " << (first + second + third) << "\n";

    return 0;
}