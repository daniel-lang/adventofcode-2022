#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>

enum class operant_e : unsigned int
{
    plus,
    multiply
};

struct monkey_t
{
    unsigned int inspections = 0;
    std::vector<unsigned int> items;
    operant_e operant;
    bool operationOld = false;
    unsigned int operationNumber;
    unsigned int test;
    int idTestFalse;
    int idTestTrue;
};

void print(const std::vector<monkey_t> &monkeys)
{
    int i = 0;
    for (auto const &monkey : monkeys)
    {
        std::cout << "monkey " << i << ": " << monkey.inspections << " inspects, " << monkey.items.size() << " elements, divisible by " << monkey.test << " true " << monkey.idTestTrue << ", false " << monkey.idTestFalse;
        std::cout << ", operant " << (unsigned int)monkey.operant << ", right old " << monkey.operationOld << ", right number " << monkey.operationNumber << "\n";
        i++;
    }
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    std::vector<monkey_t> monkeys;
    int id = 0;

    while (std::getline(input, line))
    {
        if (line.find("Monkey") != std::string::npos)
        {
            id = atoi(line.substr(7, 1).c_str());
            monkeys.push_back(monkey_t());
        }

        else if (line.find("Starting items") != std::string::npos)
        {
            std::stringstream tmp(line.substr(18));
            std::string number;
            while (std::getline(tmp, number, ','))
            {
                monkeys[id].items.push_back(atoi(number.c_str()));
            }
        }
        else if (line.find("Operation") != std::string::npos)
        {
            std::string tmp = line.substr(13);
            char operant;
            int right;

            if (sscanf(tmp.c_str(), "new = old %c %d", &operant, &right) == 2)
            {
                monkeys[id].operationNumber = right;
            }
            else if (sscanf(tmp.c_str(), "new = old %c old", &operant) == 1)
            {
                monkeys[id].operationOld = true;
            }

            switch (operant)
            {
            case '+':
                monkeys[id].operant = operant_e::plus;
                break;

            case '*':
                monkeys[id].operant = operant_e::multiply;
                break;

            default:
                break;
            }
        }
        else if (line.find("Test") != std::string::npos)
        {
            monkeys[id].test = atoi(line.substr(21).c_str());
        }
        else if (line.find("true") != std::string::npos)
        {
            monkeys[id].idTestTrue = atoi(line.substr(29).c_str());
        }
        else if (line.find("false") != std::string::npos)
        {
            monkeys[id].idTestFalse = atoi(line.substr(30).c_str());
        }
    }

    for (int i = 0; i < 20; ++i)
    {
        for (auto &monkey : monkeys)
        {
            if (monkey.items.empty())
                continue;

            for (size_t j = 0; j < monkey.items.size(); ++j)
            {
                monkey.inspections++;
                unsigned int item = monkey.items[j];
                unsigned int number = item;
                if (!monkey.operationOld)
                    number = monkey.operationNumber;

                switch (monkey.operant)
                {
                case operant_e::plus:
                    item += number;
                    break;

                case operant_e::multiply:
                    item *= number;
                    break;
                }

                item /= 3;

                if (item % monkey.test == 0)
                    monkeys[monkey.idTestTrue].items.push_back(item);
                else
                    monkeys[monkey.idTestFalse].items.push_back(item);
            }
            monkey.items.clear();
        }
    }

    print(monkeys);

    unsigned int max1 = 0, max2 = 0;
    for (auto const &monkey : monkeys)
    {
        if (monkey.inspections > max1)
        {
            max2 = max1;
            max1 = monkey.inspections;
        }
        else if (monkey.inspections > max2)
        {
            max2 = monkey.inspections;
        }
    }

    std::cout << "1: " << max1 << ", 2: " << max2 << " -> " << (max1 * max2) << std::endl;

    return 0;
}