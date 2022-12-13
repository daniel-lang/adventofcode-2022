#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <array>

struct position
{
    int x = 0;
    int y = 0;
};

void print(std::array<position, 10> const &knots)
{
    for (int y = 15; y >= -5; y--)
    {
        for (int x = -11; x < 15; x++)
        {
            if (knots[0].x == x && knots[0].y == y)
                std::cout << "H";
            else if (knots[1].x == x && knots[1].y == y)
                std::cout << "1";
            else if (knots[2].x == x && knots[2].y == y)
                std::cout << "2";
            else if (knots[3].x == x && knots[3].y == y)
                std::cout << "3";
            else if (knots[4].x == x && knots[4].y == y)
                std::cout << "4";
            else if (knots[5].x == x && knots[5].y == y)
                std::cout << "5";
            else if (knots[6].x == x && knots[6].y == y)
                std::cout << "6";
            else if (knots[7].x == x && knots[7].y == y)
                std::cout << "7";
            else if (knots[8].x == x && knots[8].y == y)
                std::cout << "8";
            else if (knots[9].x == x && knots[9].y == y)
                std::cout << "9";
            else if (x == 0 && y == 0)
                std::cout << "s";
            else
                std::cout << ".";
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    char direction;
    unsigned int steps;
    std::vector<position> visits;
    std::array<position, 10> knots;

    visits.push_back(knots[9]);

    while (input >> direction >> steps)
    {
        // print(knots);
        // std::cout << "== " << direction << " " << steps << " ==\n\n";

        for (int i = steps; i > 0; i--)
        {
            if (direction == 'U')
                knots[0].y++;
            else if (direction == 'D')
                knots[0].y--;
            else if (direction == 'R')
                knots[0].x++;
            else if (direction == 'L')
                knots[0].x--;

            for (size_t k = 1; k < knots.size(); k++)
            {
                int diffX = knots[k - 1].x - knots[k].x;
                int diffY = knots[k - 1].y - knots[k].y;
                int product = std::abs(diffX * diffY);

                if (diffX > 1 || (product >= 2 && diffX > 0))
                    knots[k].x++;
                else if (diffX < -1 || (product >= 2 && diffX < 0))
                    knots[k].x--;

                if (diffY > 1 || (product >= 2 && diffY > 0))
                    knots[k].y++;
                else if (diffY < -1 || (product >= 2 && diffY < 0))
                    knots[k].y--;

                if (k != 9)
                    continue;

                bool visited = false;
                for (auto const &visit : visits)
                {
                    if (visit.x == knots[k].x && visit.y == knots[k].y)
                        visited = true;
                }

                if (!visited)
                    visits.push_back(knots[k]);
            }
        }
    }

    // print(knots);
    std::cout << visits.size() << std::endl;

    return 0;
}