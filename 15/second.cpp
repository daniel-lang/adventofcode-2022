#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

struct point
{
    int x = 0;
    int y = 0;
};

struct sensor
{
    point position;
    int distance;
};

std::vector<sensor> sensors;

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int max = atoi(argv[2]);

    while (std::getline(input, line))
    {
        sensor s;
        point b;
        std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &s.position.x, &s.position.y, &b.x, &b.y);
        s.distance = std::abs(s.position.x - b.x) + std::abs(s.position.y - b.y);
        sensors.push_back(s);
    }

    int vx[] = {1, 1, -1, -1};
    int vy[] = {1, -1, 1, -1};

    /* The beacon needs to be on the outside (distance + 1) of one of the sensors. */
    for (auto const &s1 : sensors)
    {
        int distance = s1.distance;
        for (int dx = 0; dx < distance + 2; dx++)
        {
            int dy = (distance + 1) - dx;

            for (int i = 0; i < 4; i++)
            {
                int x = s1.position.x + dx * vx[i];
                int y = s1.position.y + dy * vy[i];

                if (x < 0 || x > max || y < 0 || y > max)
                    continue;

                bool overlap = false;
                for (auto const &s2 : sensors)
                {
                    auto distance = std::abs(s2.position.x - x) + std::abs(s2.position.y - y);
                    if (s2.distance >= distance)
                        overlap = true;
                }

                if (!overlap)
                {
                    std::printf("%d,%d", x, y);
                    return 0;
                }
            }
        }
    }

    return 1;
}