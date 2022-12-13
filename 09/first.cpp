#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>

struct position
{
    int x = 0;
    int y = 0;
};

void print(position head, position tail)
{
    for (int y = 4; y >= 0; y--)
    {
        for (int x = 0; x < 6; x++)
        {
            if (head.x == x && head.y == y)
                std::cout << "H";
            else if (tail.x == x && tail.y == y)
                std::cout << "T";
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
    position head, tail;
    std::vector<position> visits;

    visits.push_back(tail);
    // print(head, tail);

    while (input >> direction >> steps)
    {
        // std::cout << "== " << direction << " " << steps << " ==\n\n";

        for (int i = steps; i > 0; i--)
        {
            if (direction == 'U')
                head.y++;
            else if (direction == 'D')
                head.y--;
            else if (direction == 'R')
                head.x++;
            else if (direction == 'L')
                head.x--;

            int diffX = head.x - tail.x;
            int diffY = head.y - tail.y;
            int product = std::abs(diffX * diffY);

            if (diffX > 1 || (product >= 2 && diffX > 0))
                tail.x++;
            else if (diffX < -1 || (product >= 2 && diffX < 0))
                tail.x--;

            if (diffY > 1 || (product >= 2 && diffY > 0))
                tail.y++;
            else if (diffY < -1 || (product >= 2 && diffY < 0))
                tail.y--;

            bool visited = false;
            for (auto const &visit : visits)
            {
                if (visit.x == tail.x && visit.y == tail.y)
                    visited = true;
            }

            if (!visited)
                visits.push_back(tail);

            // print(head, tail);
        }
    }

    std::cout << visits.size() << std::endl;

    return 0;
}