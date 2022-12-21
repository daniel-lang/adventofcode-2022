#include <vector>
#include <fstream>
#include <iostream>
#include <queue>

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
    bool numberKnown = false;
    long long number = 0;
    long long leftNumber = 0;
    long long rightNumber = 0;
    bool leftNumberKnown = false;
    bool rightNumberKnown = false;
    operation op = operation::NONE;
};

std::vector<monkey> monkeys;
std::queue<monkey> queue;

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
            m.numberKnown = true;
        }

        m.name = std::string(name);

        if (m.name == "humn")
            continue;

        if (m.numberKnown)
        {
            std::cout << m.name << ": " << m.number << "\n";
            queue.push(m);
        }

        monkeys.push_back(m);
    }

    long long targetValue = 0;
    std::string targetName;

    while (queue.size() > 0)
    {
        auto m = queue.front();
        queue.pop();

        for (auto &monkey : monkeys)
        {
            if (monkey.numberKnown || (monkey.leftMonkey != m.name && monkey.rightMonkey != m.name))
                continue;

            if (monkey.leftMonkey == m.name)
            {
                monkey.leftNumber = m.number;
                monkey.leftNumberKnown = true;

                if (monkey.name == "root")
                {
                    targetValue = m.number;
                    targetName = monkey.rightMonkey;
                }
            }
            if (monkey.rightMonkey == m.name)
            {
                monkey.rightNumber = m.number;
                monkey.rightNumberKnown = true;

                if (monkey.name == "root")
                {
                    targetValue = m.number;
                    targetName = monkey.leftMonkey;
                }
            }

            if (monkey.leftNumberKnown && monkey.rightNumberKnown)
            {
                if (monkey.op == operation::PLUS)
                    monkey.number = monkey.leftNumber + monkey.rightNumber;
                if (monkey.op == operation::MINUS)
                    monkey.number = monkey.leftNumber - monkey.rightNumber;
                if (monkey.op == operation::MULTIPLY)
                    monkey.number = monkey.leftNumber * monkey.rightNumber;
                if (monkey.op == operation::DIVIDE)
                    monkey.number = monkey.leftNumber / monkey.rightNumber;

                monkey.numberKnown = true;
                queue.push(monkey);

                std::cout << monkey.name << ": " << monkey.number << "\n";
            }
        }
    }

    std::cout << targetName << " -> " << targetValue << std::endl;

    while (targetName != "humn")
    {
        for (auto &monkey : monkeys)
        {
            if (monkey.name != targetName)
                continue;

            if (monkey.leftNumberKnown)
            {
                targetName = monkey.rightMonkey;

                if (monkey.op == operation::PLUS)
                    targetValue = targetValue - monkey.leftNumber;
                if (monkey.op == operation::MINUS)
                    targetValue = (targetValue - monkey.leftNumber) * -1;
                if (monkey.op == operation::MULTIPLY)
                    targetValue = targetValue / monkey.leftNumber;
                if (monkey.op == operation::DIVIDE)
                    targetValue = monkey.leftNumber / targetValue;
            }
            else
            {
                targetName = monkey.leftMonkey;

                if (monkey.op == operation::PLUS)
                    targetValue = targetValue - monkey.rightNumber;
                if (monkey.op == operation::MINUS)
                    targetValue = (targetValue + monkey.rightNumber);
                if (monkey.op == operation::MULTIPLY)
                    targetValue = targetValue / monkey.rightNumber;
                if (monkey.op == operation::DIVIDE)
                    targetValue = targetValue * monkey.rightNumber;
            }

            std::cout << targetName << ": " << targetValue << "\n";
            break;
        }
    }

    std::cout << targetName << " -> " << targetValue << std::endl;

    return 0;
}