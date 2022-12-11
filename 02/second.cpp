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
        // lose
        if (choice == 'X')
        {
            if (choiceOther == 'A') // rock -> scissor
                points += 3;
            else if (choiceOther == 'B') // paper -> rock
                points += 1;
            else // scissor -> paper
                points += 2;
        }
        // draw
        else if (choice == 'Y')
        {
            points += 3;

            if (choiceOther == 'A') // rock -> rock
                points += 1;
            else if (choiceOther == 'B') // paper -> paper
                points += 2;
            else // scissor -> scissor
                points += 3;
        }
        // win
        else if (choice == 'Z')
        {
            points += 6;

            if (choiceOther == 'A') // rock -> paper
                points += 2;
            else if (choiceOther == 'B') // paper -> scissor
                points += 3;
            else // scissor -> rock
                points += 1;
        }
    }

    std::cout << points << std::endl;

    return 0;
}