#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line = "";
    std::string first = "";
    std::string second = "";
    unsigned int score = 0;

    while (input >> line)
    {
        if (first.empty() && second.empty())
        {
            second = line;
        }
        else if (first.empty())
        {
            first = line;
        }
        else
        {
            for (char c : line)
            {
                if (first.find(c) != std::string::npos && second.find(c) != std::string::npos)
                {
                    if (c >= 'a' && c <= 'z')
                        score += c - 'a' + 1;
                    else
                        score += c - 'A' + 27;

                    break;
                }
            }

            first.clear();
            second.clear();
        }
    }

    std::cout << score << std::endl;

    return 0;
}