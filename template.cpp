#include <vector>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    unsigned int count = 0;

    while (input >> line)
    {
    }

    return 0;
}