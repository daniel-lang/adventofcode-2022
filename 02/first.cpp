#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    char choice, choiceOther;
    unsigned int points = 0;

    while (input >> choiceOther >> choice)
    {
        // rock
        if (choice == 'X')
        {
            points += 1;

            if (choiceOther == 'A') // rock - draw
                points += 3;
            else if (choiceOther == 'C') // scissor - win
                points += 6;
        }
        // paper
        else if (choice == 'Y')
        {
            points += 2;

            if (choiceOther == 'A') // rock - win
                points += 6;
            else if (choiceOther == 'B') // paper - draw
                points += 3;
        }
        // scissor
        else if (choice == 'Z')
        {
            points += 3;

            if (choiceOther == 'B') // paper - win
                points += 6;
            else if (choiceOther == 'C') // scissor - draw
                points += 3;
        }
    }

    std::cout << points << std::endl;

    return 0;
}