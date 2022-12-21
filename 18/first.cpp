#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

struct point
{
    int x;
    int y;
    int z;
};

std::unordered_map<std::string, point> map;

int dx[] = {1, -1, 0, 0, 0, 0};
int dy[] = {0, 0, 1, -1, 0, 0};
int dz[] = {0, 0, 0, 0, 1, -1};

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
        std::sscanf(line.c_str(), "%d,%d,%d", &p.x, &p.y, &p.z);
        map.insert({pointToString(p), p});
    }

    for (auto const &p : map)
    {
        for (int i = 0; i < 6; ++i)
        {
            point newP = p.second;
            newP.x += dx[i];
            newP.y += dy[i];
            newP.z += dz[i];
            if (map.count(pointToString(newP)) == 0)
                count++;
        }
        std::cout << pointToString(p.second) << " " << count << "\n";
    }

    std::cout << count << std::endl;

    return 0;
}