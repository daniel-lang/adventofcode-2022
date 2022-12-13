#include <vector>
#include <fstream>
#include <iostream>

size_t findMatch(std::string const &input)
{
    size_t position = 1;
    size_t count = 1;

    while (count > 0)
    {
        char c = input.at(position);
        if (c == '[')
            count++;
        else if (c == ']')
            count--;

        if (count != 0)
            position++;
    }

    return position;
}

size_t getList(std::string const &input, std::string &output)
{
    if (input.at(0) == '[') /* need to find the end */
    {
        size_t end = findMatch(input);
        output = input.substr(1, end - 1);
        return output.size() + 2;
    }

    /* generate new list */
    output = input.substr(0, input.find(','));
    return 1;
}

int isListInOrder(std::string const &left, std::string const &right)
{
    size_t l = 0, r = 0;
    while (true)
    {
        if (l < left.size() && r >= right.size()) /* right ran out of values first */
            return -1;
        else if (l >= left.size() && r < right.size()) /* left ran out of values */
            return 1;
        else if (l >= left.size() && r >= right.size()) /* both ran out of values */
            return 0;
        else if (left.at(l) == '[' || right.at(r) == '[')
        {
            std::string newLeft, newRight;
            l += getList(left.substr(l), newLeft);
            r += getList(right.substr(r), newRight);
            int ret = isListInOrder(newLeft, newRight);
            if (ret != 0)
                return ret;

            continue; /* do not increment counters */
        }
        else if (left.at(l) != ',' && right.at(r) != ',')
        {
            int leftNumber = atoi(left.substr(l, left.find(',') - l).c_str());
            int rightNumber = atoi(right.substr(r, right.find(',') - r).c_str());

            if (leftNumber < rightNumber) /* left value is lower */
                return 1;
            else if (leftNumber > rightNumber) /* right value is lower */
                return -1;
        }

        l++;
        r++;
    }
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line, left;
    unsigned int sum = 0;
    unsigned int pair = 1;

    while (std::getline(input, line))
    {
        if (line.empty())
            continue;

        if (left.empty())
            left = line;
        else
        {
            if (isListInOrder(left.substr(1, left.find_last_of(']') - 1), line.substr(1, line.find_last_of(']') - 1)) == 1)
                sum += pair;

            pair++;
            left = "";
        }
    }

    std::cout << sum << std::endl;

    return 0;
}