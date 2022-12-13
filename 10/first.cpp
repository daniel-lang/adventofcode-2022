#include <vector>
#include <fstream>
#include <iostream>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::string> instructions;
    std::string line;
    int cycles = 0;
    int x = 1;
    int addCycles = 0;
    int addValue = 0;
    int sum = 0;

    while (std::getline(input, line))
        instructions.push_back(line);

    size_t pr = 0;
    while (pr < instructions.size() && cycles < 220)
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

        cycles++;

        if (cycles == 20 || (cycles - 20) % 40 == 0)
        {
            std::cout << "cycle " << cycles << " x is " << x << std::endl;
            sum += x * cycles;
        }

        if (addCycles == 1)
        {
            x += addValue;
            addCycles = 0;
        }
    }

    std::cout << sum << std::endl;

    return 0;
}