#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

struct point
{
    int x = 0;
    int y = 0;
};

struct path
{
    point start;
    point end;
};

std::vector<path> paths;
std::vector<point> sand;

bool empty(int x, int y)
{
    for (auto const &s : sand)
        if (s.x == x && s.y == y)
            return false;

    for (auto const &p : paths)
    {
        auto dxc = x - p.start.x;
        auto dyc = y - p.start.y;
        auto dxl = p.end.x - p.start.x;
        auto dyl = p.end.y - p.start.y;
        auto cross = dxc * dyl - dyc * dxl;

        if (cross != 0)
            continue;

        if (std::abs(dxl) >= std::abs(dyl))
        {
            if (dxl > 0 && p.start.x <= x && x <= p.end.x)
                return false;
            else if (dxl < 0 && p.end.x <= x && x <= p.start.x)
                return false;
        }
        else
        {
            if (dyl > 0 && p.start.y <= y && y <= p.end.y)
                return false;
            else if (dyl < 0 && p.end.y <= y && y <= p.start.y)
                return false;
        }
    }

    return true;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int maxY = 0;

    while (std::getline(input, line))
    {
        point start, end;
        start.x = atoi(line.substr(0, line.find(',')).c_str());
        start.y = atoi(line.substr(line.find(',') + 1, line.find('-') - line.find(',')).c_str());
        size_t offset = line.find('-');

        while (offset != std::string::npos)
        {
            offset += 2;
            end.x = atoi(line.substr(offset, line.find(',', offset)).c_str());
            end.y = atoi(line.substr(line.find(',', offset) + 1, line.find('-', offset) - line.find(',', offset)).c_str());

            offset = line.find('-', offset);
            paths.push_back({start, end});

            if (start.y > maxY)
                maxY = start.y;
            if (end.y > maxY)
                maxY = end.y;

            start = end;
        }
    }

    bool overflow = false;
    while (!overflow)
    {
        point current = {500, 0};
        bool rest = false;

        while (!rest)
        {
            if (empty(current.x, current.y + 1))
            {
                current.y++;
            }
            else if (empty(current.x - 1, current.y + 1))
            {
                current.x--;
                current.y++;
            }
            else if (empty(current.x + 1, current.y + 1))
            {
                current.x++;
                current.y++;
            }
            else
            {
                rest = true;
                sand.push_back(current);
            }

            if (current.y > maxY)
            {
                overflow = true;
                break;
            }
        }
    }

    std::cout << sand.size() << std::endl;

    return 0;
}