#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    unsigned int count = 0;
    std::vector<char> list;
    char in;

    while (input >> in)
    {
        list.push_back(in);
        count++;

        if (list.size() >= 14)
        {
            bool unique = true;

            for (auto it = list.cend() - 14; it < list.cend(); ++it)
            {
                for (auto it2 = it + 1; it2 < list.cend(); ++it2)
                {
                    if (*it == *it2)
                        unique = false;
                }
            }

            if (unique)
            {
                std::cout << count << std::endl;
                return 0;
            }
        }
    }

    return 0;
}