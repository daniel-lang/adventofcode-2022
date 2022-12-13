#include <vector>
#include <fstream>
#include <iostream>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::string> instructions;
    std::string line;
    int x = 1;
    int addCycles = 0;
    int addValue = 0;

    int crtX = 0;

    while (std::getline(input, line))
        instructions.push_back(line);

    size_t pr = 0;
    while (pr < instructions.size())
    {
        if (addCycles == 0 && pr < instructions.size())
        {
            std::string inst = instructions[pr];
            pr++;
            if (inst != "noop")
            {
                addCycles = 2;
                addValue = atoi(inst.substr(5).c_str());
            }
        }
        else
        {
            addCycles--;
        }

        if (x - 2 < crtX && x + 2 > crtX)
            std::cout << "#";
        else
            std::cout << ".";

        crtX++;

        if (addCycles == 1)
        {
            x += addValue;
            addCycles = 0;
        }

        if (crtX == 40)
        {
            std::cout << "\n";
            crtX = 0;
        }
    }

    return 0;
}