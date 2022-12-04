#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line, left, right;
    std::vector<char> duplicates;
    unsigned int score = 0;

    while (input >> line)
    {
        left = line.substr(0, line.length() / 2);
        right = line.substr(line.length() / 2);

        for (char c : left)
        {
            if (right.find(c) != std::string::npos && std::find(duplicates.cbegin(), duplicates.cend(), c) == duplicates.cend())
            {
                duplicates.push_back(c);
            }
        }

        for (char c : duplicates)
        {
            if (c >= 'a' && c <= 'z')
                score += c - 'a' + 1;
            else
                score += c - 'A' + 27;
        }

        duplicates.clear();
    }

    std::cout << score << std::endl;

    return 0;
}