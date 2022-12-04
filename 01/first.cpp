#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

int main(int argc, char *argv[])
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

    std::cout << *(std::max_element(elves.cbegin(), elves.cend())) << std::endl;

    return 0;
}