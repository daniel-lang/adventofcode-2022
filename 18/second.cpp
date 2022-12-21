#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <queue>

enum class content
{
    ROCK,
    WATER,
    AIR
};

struct point
{
    int x;
    int y;
    int z;
    content c;
};

std::unordered_map<std::string, point> map;

int dx[] = {1, -1, 0, 0, 0, 0};
int dy[] = {0, 0, 1, -1, 0, 0};
int dz[] = {0, 0, 0, 0, 1, -1};

int minX = INT32_MAX, minY = INT32_MAX, minZ = INT32_MAX, maxX = 0, maxY = 0, maxZ = 0;

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z);
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    unsigned int count = 0;

    while (std::getline(input, line))
    {
        point p;
        p.c = content::ROCK;
        std::sscanf(line.c_str(), "%d,%d,%d", &p.x, &p.y, &p.z);
        map.insert({pointToString(p), p});

        if (p.x < minX)
            minX = p.x;
        if (p.x > maxX)
            maxX = p.x;
        if (p.y < minY)
            minY = p.y;
        if (p.y > maxY)
            maxY = p.y;
        if (p.z < minZ)
            minZ = p.z;
        if (p.z > maxZ)
            maxZ = p.z;
    }

    std::queue<point> queue;
    point start = {minX - 1, minY - 1, minZ - 1, content::WATER};
    map.insert({pointToString(start), start});
    queue.push(start);

    while (queue.size() > 0)
    {
        auto p = queue.front();
        queue.pop();

        for (int i = 0; i < 6; ++i)
        {
            auto newP = p;
            newP.x += dx[i];
            newP.y += dy[i];
            newP.z += dz[i];

            if (newP.x > maxX + 1 || newP.y > maxY + 1 || newP.z > maxZ + 1)
                continue;
            if (newP.x < minX - 1 || newP.y < minY - 1 || newP.z < minZ - 1)
                continue;

            auto exists = map.count(pointToString(newP));
            if (exists == 0)
            {
                newP.c = content::WATER;
                queue.push(newP);
                map.insert({pointToString(newP), newP});
            }
            else if (map.at(pointToString(newP)).c == content::ROCK)
                count++;
        }
    }

    std::cout << count << std::endl;

    return 0;
}