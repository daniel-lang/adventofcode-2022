#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

int main(int argc, char *argv[])
{
    std::ifstream input(argv[1]);

    char choice, choiceOther;
    bool other = true;
    unsigned int points = 0;

    while (input >> choice)
    {
        if (other)
        {
            choiceOther = choice;
            other = false;
        }
        else
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

            other = true;
        }
    }

    std::cout << points << std::endl;

    return 0;
}