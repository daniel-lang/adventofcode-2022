#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <list>
#include <algorithm>

struct point
{
    int x;
    int y;
};

struct elve
{
    int id;
    point position;
    point proposedPosition;
};

struct facing
{
    int direction;
    int x[3];
    int y[3];
};

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void print(point min, point max, std::unordered_map<std::string, elve> const &elves)
{
    for (int y = min.y; y <= max.y; ++y)
    {
        for (int x = min.x; x <= max.x; ++x)
        {
            if (elves.count(pointToString({x, y})) != 0)
                std::cout << "#";
            else
                std::cout << ".";
        }

        std::cout << "\n";
    }

    std::cout << "\n";
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;

    std::unordered_map<std::string, elve> elves;
    int y = 0;
    int id = 0;
    point min = {INT32_MAX, INT32_MAX};
    point max = {INT32_MIN, INT32_MIN};

    while (std::getline(input, line))
    {
        int x = 0;
        for (char c : line)
        {
            if (c == '#')
            {
                elves.insert({pointToString({x, y}), {id, {x, y}, {INT32_MIN, INT32_MIN}}});
                id++;

                if (x > max.x)
                    max.x = x;
                if (x < min.x)
                    min.x = x;
                if (y > max.y)
                    max.y = y;
                if (y < min.y)
                    min.y = y;
            }

            x++;
        }

        y++;
    }

    int dx[] = {-1, 0, 1, -1, 0, 1, -1, 1};
    int dy[] = {-1, -1, -1, 1, 1, 1, 0, 0};

    facing north = {
        0,
        {-1, 0, 1},
        {-1, -1, -1}};

    facing south = {
        1,
        {-1, 0, 1},
        {1, 1, 1}};

    facing west = {
        2,
        {-1, -1, -1},
        {-1, 0, 1}};

    facing east = {
        3,
        {1, 1, 1},
        {-1, 0, 1}};

    std::list<facing> facings;
    facings.push_back(north);
    facings.push_back(south);
    facings.push_back(west);
    facings.push_back(east);

    int round = 0;
    bool oneElvesMoved = true;
    while (oneElvesMoved)
    {
        oneElvesMoved = false;
        std::vector<std::string> elvesProposed;

        for (auto &[key, elve] : elves)
        {
            point proposed = {INT32_MIN, INT32_MIN};

            bool needsMove = false;

            for (int p = 0; !needsMove && p < 8; ++p)
            {
                point pNew = elve.position;
                pNew.x += dx[p];
                pNew.y += dy[p];
                if (elves.find(pointToString(pNew)) != 0)
                    needsMove = true;
            }

            if (needsMove)
            {
                oneElvesMoved = true;

                for (auto const &f : facings)
                {
                    bool moveDirection = true;

                    for (int p = 0; moveDirection && p < 3; ++p)
                    {
                        point pNew = elve.position;
                        pNew.x += f.x[p];
                        pNew.y += f.y[p];
                        if (elves.find(pointToString(pNew)) != 0)
                            moveDirection = false;
                    }

                    if (!moveDirection)
                        continue;

                    switch (f.direction)
                    {
                    case 0: // north
                        proposed = {elve.position.x, elve.position.y - 1};
                        break;
                    case 1: // south
                        proposed = {elve.position.x, elve.position.y + 1};
                        break;
                    case 2: // west
                        proposed = {elve.position.x - 1, elve.position.y};
                        break;
                    case 3: // east
                        proposed = {elve.position.x + 1, elve.position.y};
                        break;
                    }

                    break;
                }
            }

            elvesProposed.push_back(pointToString(proposed));
            elve.proposedPosition = proposed;
        }

        facings.push_back(facings.front());
        facings.pop_front();

        int i = 0;
        std::unordered_map<std::string, elve> tmp;
        for (auto &[key, elve] : elves)
        {
            // std::cout << elve.id << " (" << pointToString(elve.position) << ") proposed " << elvesProposed[i];

            if (elve.proposedPosition.x == INT32_MIN && elve.proposedPosition.y == INT32_MIN)
            {
                // no move
                tmp.insert({key, elve});
                // std::cout << " not moving because no proposed\n";
            }
            else if (std::count(elvesProposed.cbegin(), elvesProposed.cend(), elvesProposed[i]) > 1)
            {
                // no move
                tmp.insert({key, elve});
                // std::cout << " not moving because occupied\n";
            }
            else
            {
                // std::cout << " moving to " << pointToString(elve.proposedPosition) << "\n";

                elve.position = elve.proposedPosition;
                tmp.insert({pointToString(elve.position), elve});

                if (elve.position.x > max.x)
                    max.x = elve.position.x;
                if (elve.position.x < min.x)
                    min.x = elve.position.x;
                if (elve.position.y > max.y)
                    max.y = elve.position.y;
                if (elve.position.y < min.y)
                    min.y = elve.position.y;
            }

            i++;
        }

        elves = tmp;
        round++;
        // print(min, max, elves);
    }

    std::cout << round << std::endl;

    return 0;
}