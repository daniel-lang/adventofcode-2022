#include <vector>
#include <fstream>
#include <iostream>

void to_snafu_digit(std::vector<int64_t> const &limit, int64_t number, int digit)
{
    int i = 0;
    long power = 1;
    while (limit[i] < std::abs(number))
    {
        i++;
        power *= 5;
    }

    if (digit == 0)
    {
        std::cout << number << "\n";
        return;
    }

    if (i != digit)
    {
        std::cout << "0";
        to_snafu_digit(limit, number, --digit);
        return;
    }

    for (int j = -2; j <= 2; j++)
    {
        long remaining = number - power * j;
        if (remaining < -limit[i - 1] || remaining > limit[i - 1])
            continue;

        if (j == -2)
            std::cout << "=";
        else if (j == -1)
            std::cout << "-";
        else
            std::cout << j;

        to_snafu_digit(limit, remaining, --digit);
        return;
    }
}

void to_snafu(std::vector<int64_t> const &limit, int64_t number)
{
    int digits = 0;
    while (limit[digits] < number)
        digits++;

    std::cout << "result has " << (digits + 1) << " digits\n";
    to_snafu_digit(limit, number, digits);
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    uint64_t count = 0;

    while (std::getline(input, line))
    {
        uint64_t power = 1;
        int64_t number = 0;

        for (auto it = line.rbegin(); it != line.rend(); ++it)
        {
            char c = *it;
            int digit = 0;

            if (c == '-')
                digit = -1;
            else if (c == '=')
                digit = -2;
            else
                digit = c - '0';

            number += digit * power;

            power *= 5;
        }

        std::cout << "line: " << line << " -> " << number << "\n";

        count += number;
    }

    std::cout << "Sum: " << count << "\n";

    std::vector<int64_t> limit;
    int64_t n = 2;
    int64_t p = 1;
    for (int i = 0; i < 28; ++i)
    {
        limit.push_back(n);
        p *= 5;
        n += p * 2;
    }

    to_snafu(limit, count);

    return 0;
}