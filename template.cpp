#include <vector>
#include <fstream>
#include <iostream>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    unsigned int count = 0;

    while (std::getline(input, line))
    {
    }

    std::cout << count << std::endl;

    return 0;
}