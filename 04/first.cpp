#include <stdio.h>
#include <iostream>

int main(int, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");

    int firstStart, firstEnd, secondStart, secondEnd;
    int count = 0;

    while (fscanf(fp, "%d-%d,%d-%d", &firstStart, &firstEnd, &secondStart, &secondEnd) == 4)
    {
        if ((firstStart >= secondStart && firstEnd <= secondEnd) || (secondStart >= firstStart && secondEnd <= firstEnd))
            count++;
    }

    std::cout << count << std::endl;

    return 0;
}