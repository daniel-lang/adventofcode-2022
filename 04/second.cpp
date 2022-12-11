#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

int main(int, char *argv[])
{
    FILE *fp = fopen(argv[1], "r");

    int firstStart, firstEnd, secondStart, secondEnd;
    int count = 0;

    while (fscanf(fp, "%d-%d,%d-%d", &firstStart, &firstEnd, &secondStart, &secondEnd) == 4)
    {
        std::vector<unsigned int> first(firstEnd - firstStart + 1);
        std::vector<unsigned int> second(secondEnd - secondStart + 1);
        std::vector<unsigned int> result(first.size() + second.size());

        std::iota(first.begin(), first.end(), firstStart);
        std::iota(second.begin(), second.end(), secondStart);

        auto it = std::set_intersection(first.cbegin(), first.cend(), second.cbegin(), second.cend(), result.begin());
        if (it != result.cbegin())
            count++;
    }

    std::cout << count << std::endl;

    return 0;
}