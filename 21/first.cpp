#include <vector>
#include <fstream>
#include <iostream>

enum class operation
{
    NONE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
};

struct monkey
{
    std::string name;
    std::string leftMonkey;
    std::string rightMonkey;
    long long number = INT64_MIN;
    long long leftNumber = INT64_MIN;
    long long rightNumber = INT64_MIN;
    operation op = operation::NONE;
    bool done = false;
};

std::vector<monkey> monkeys;

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;

    while (std::getline(input, line))
    {
        monkey m;
        char name[5];
        char left[5];
        char right[5];
        char operation;
        int number;

        if (std::sscanf(line.c_str(), "%4s: %4s %c %4s", name, left, &operation, right) == 4)
        {
            m.leftMonkey = std::string(left);
            m.rightMonkey = std::string(right);
            if (operation == '+')
                m.op = operation::PLUS;
            else if (operation == '-')
                m.op = operation::MINUS;
            else if (operation == '*')
                m.op = operation::MULTIPLY;
            else if (operation == '/')
                m.op = operation::DIVIDE;
        }
        else if (std::sscanf(line.c_str(), "%4s: %d", name, &number) == 2)
        {
            m.number = number;
        }

        m.name = std::string(name);

        monkeys.push_back(m);
    }

    bool rootKnown = false;
    long long rootValue = 0;

    while (!rootKnown)
    {
        for (auto &monkey : monkeys)
        {
            if (monkey.done)
                continue;

            if (monkey.number == INT64_MIN)
                continue;

            for (auto &monkey2 : monkeys)
            {
                if (monkey2.leftMonkey == monkey.name)
                    monkey2.leftNumber = monkey.number;
                if (monkey2.rightMonkey == monkey.name)
                    monkey2.rightNumber = monkey.number;

                if (monkey2.leftNumber != INT64_MIN && monkey2.rightNumber != INT64_MIN)
                {
                    if (monkey2.op == operation::PLUS)
                        monkey2.number = monkey2.leftNumber + monkey2.rightNumber;
                    if (monkey2.op == operation::MINUS)
                        monkey2.number = monkey2.leftNumber - monkey2.rightNumber;
                    if (monkey2.op == operation::MULTIPLY)
                        monkey2.number = monkey2.leftNumber * monkey2.rightNumber;
                    if (monkey2.op == operation::DIVIDE)
                        monkey2.number = monkey2.leftNumber / monkey2.rightNumber;
                }
            }
            monkey.done = true;

            std::cout << monkey.name << ": " << monkey.number << "\n";

            if (monkey.name == "root")
            {
                rootValue = monkey.number;
                rootKnown = true;
            }
        }
    }

    std::cout << rootValue << std::endl;

    return 0;
}