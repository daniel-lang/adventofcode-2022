#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <deque>
#include <vector>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int stackCount = atoi(argv[2]);
    std::vector<std::deque<char>> stacks(stackCount);

    while (std::getline(input, line))
    {
        // cranes
        if (line.find('[') != std::string::npos)
        {
            size_t offset = 0;
            while ((offset = line.find('[', offset)) != std::string::npos)
            {
                stacks[offset / 4].push_back(line.at(offset + 1));
                offset++;
            }
        }
        // move instructions
        else
        {
            std::istringstream iss(line);
            int amount, from, to;
            if (sscanf(line.c_str(), "move %d from %d to %d", &amount, &from, &to) != 3)
                continue;

            std::vector<char> tmp;

            for (int i = 0; i < amount; ++i)
            {
                tmp.push_back(stacks[from - 1].front());
                stacks[from - 1].pop_front();
            }

            for (int i = amount - 1; i >= 0; --i)
            {
                stacks[to - 1].push_front(tmp.at(i));
            }
        }
    }

    for (int i = 0; i < stackCount; ++i)
    {
        std::cout << stacks[i].front();
    }

    std::cout << std::endl;

    return 0;
}