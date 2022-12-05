#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <deque>
#include <vector>

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    unsigned int count = 0;
    int stackCount = atoi(argv[2]);
    std::vector<std::deque<char>> stacks(stackCount);

    while (std::getline(input, line))
    {
        // cranes
        if (line.find('[') != std::string::npos)
        {
            int offset = 0;
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

            for (int i = 0; i < amount; ++i)
            {
                char c = stacks[from - 1].front();
                stacks[from - 1].pop_front();
                stacks[to - 1].push_front(c);
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