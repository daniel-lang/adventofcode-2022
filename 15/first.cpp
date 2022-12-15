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
int filledMin = INT32_MAX;
int filledMax = INT32_MIN;

void check(int x)
{
    if (x < filledMin)
    {
        filledMin = x;
    }
    if (x > filledMax)
    {
        filledMax = x;
    }
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int y = atoi(argv[2]);

    while (std::getline(input, line))
    {
        point sensor, beacon;
        std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensor.x, &sensor.y, &beacon.x, &beacon.y);
        couples.push_back({sensor, beacon});
    }

    for (auto const &c : couples)
    {
        auto distance = std::abs(c.sensor.x - c.beacon.x) + std::abs(c.sensor.y - c.beacon.y);
        if ((c.sensor.y <= y && c.sensor.y + distance >= y) || (c.sensor.y >= y && c.sensor.y - distance <= y))
        {
            auto diff = distance - std::abs(c.sensor.y - y);
            std::printf("sensor (%d,%d), beacon (%d, %d) hits y=%d because distance is %d -> diff %d\n", c.sensor.x, c.sensor.y, c.beacon.x, c.beacon.y, y, distance, diff);

            check(c.sensor.x);
            for (int i = 1; i <= diff; ++i)
            {
                check(c.sensor.x + i);
                check(c.sensor.x - i);
            }
        }
    }

    std::vector<int> excludedX;

    for (auto const &c : couples)
    {
        if (c.sensor.y == y && c.sensor.x >= filledMin && c.sensor.x <= filledMax && std::find(excludedX.cbegin(), excludedX.cend(), c.sensor.x) == excludedX.cend())
            excludedX.push_back(c.sensor.x);

        if (c.beacon.y == y && c.beacon.x >= filledMin && c.beacon.x <= filledMax && std::find(excludedX.cbegin(), excludedX.cend(), c.beacon.x) == excludedX.cend())
            excludedX.push_back(c.beacon.x);
    }

    std::printf("-> min %d\n-> max %d\n->count %ld\n%d\n", filledMin, filledMax, excludedX.size(), (filledMax - filledMin - static_cast<int>(excludedX.size()) + 1));

    return 0;
}