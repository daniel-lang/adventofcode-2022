#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>

struct position
{
    int x;
    int y;
};

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::vector<char>> grid;
    std::string line;
    position start, end;

    int y = 0;
    while (std::getline(input, line))
    {
        int x = 0;
        std::stringstream sline(line);
        char c;
        std::vector<char> tmp;
        while (sline >> c)
        {
            if (c == 'S')
            {
                start.x = x;
                start.y = y;
                c = 'a';
            }
            else if (c == 'E')
            {
                end.x = x;
                end.y = y;
                c = 'z';
            }
            tmp.push_back(c);
            x++;
        }

        grid.push_back(tmp);
        y++;
    }

    size_t height = grid.size();
    size_t width = grid[0].size();
    unsigned int steps = 0;
    unsigned int nodesLeftInLayer = 1;
    unsigned int nodesInNextLayer = 0;
    bool found = false;
    std::vector<std::vector<bool>> visited(height);
    std::queue<position> queue;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (size_t i = 0; i < visited.size(); ++i)
    {
        std::vector<bool> tmp(width);
        std::fill(tmp.begin(), tmp.end(), false);
        visited[i] = tmp;
    }

    queue.push(start);
    visited[start.y][start.x] = true;

    while (queue.size() > 0)
    {
        position p = queue.front();
        queue.pop();

        if (p.x == end.x && p.y == end.y)
        {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i)
        {
            int newX = p.x + dx[i];
            int newY = p.y + dy[i];

            if (newX < 0 || newY < 0)
                continue;
            if (static_cast<size_t>(newX) >= width || static_cast<size_t>(newY) >= height)
                continue;
            if (visited[newY][newX])
                continue;
            if (grid[newY][newX] - 1 > grid[p.y][p.x])
                continue;

            queue.push({newX, newY});
            visited[newY][newX] = true;
            nodesInNextLayer++;
        }

        nodesLeftInLayer--;
        if (nodesLeftInLayer == 0)
        {
            nodesLeftInLayer = nodesInNextLayer;
            nodesInNextLayer = 0;
            steps++;
        }
    }

    if (found)
        std::cout << steps << std::endl;
    else
        std::cout << "not found" << std::endl;

    return 0;
}