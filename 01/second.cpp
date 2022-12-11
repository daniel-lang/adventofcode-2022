#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::vector<int> elves;
    int elve = 0;
    char line[10];
    while (input.getline(line, 10))
    {
        if (!strlen(line) && elve > 0)
        {
            elves.push_back(elve);
            elve = 0;
        }
        else
        {
            elve += atoi(line);
        }
    }

    if (elve > 0)
        elves.push_back(elve);

    int one = 0;
    int two = 0;
    int three = 0;

    for (int elve : elves)
    {
        if (elve > one)
        {
            three = two;
            two = one;
            one = elve;
        }
        else if (elve > two)
        {
            three = two;
            two = elve;
        }
        else if (elve > three)
        {
            three = elve;
        }
    }

    std::cout << (one + two + three) << std::endl;

    return 0;
}