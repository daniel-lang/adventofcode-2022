#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

struct point
{
    int x = 0;
    int y = 0;
};

struct couple
{
    point sensor;
    point beacon;
};

std::vector<couple> couples;

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int max = atoi(argv[2]);

    while (std::getline(input, line))
    {
        point sensor, beacon;
        std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor.x, &sensor.y, &beacon.x, &beacon.y);
        couples.push_back({sensor, beacon});
    }

    bool found = false;
    point p;
    for (int y = 0; y < max; y++)
    {
        for (int x = 0; x < max; x++)
        {
            std::printf("%d,%d\n", x, y);
            bool hit = false;
            for (auto const &c : couples)
            {
                if ((c.sensor.x == x && c.sensor.y == y) || (c.beacon.x == x && c.beacon.y == y))
                {
                    hit = true;
                    break;
                }

                auto distance = std::abs(c.sensor.x - c.beacon.x) + std::abs(c.sensor.y - c.beacon.y);
                auto distanceSensor = std::abs(c.sensor.x - x) + std::abs(c.sensor.y - y);
                if (distanceSensor <= distance)
                {
                    hit = true;
                    break;
                }
            }

            if (!hit)
            {
                found = true;
                p.x = x;
                p.y = y;
                break;
            }
        }

        if (found)
            break;
    }

    std::printf("%d,%d -> %d\n", p.x, p.y, (p.x * max + p.y));
    return 0;
}