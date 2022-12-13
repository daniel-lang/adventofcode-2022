#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

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
            size_t leftComma = left.find(',');
            size_t rightComma = right.find(',');
            if (leftComma == std::string::npos)
                leftComma = left.size();
            if (rightComma == std::string::npos)
                rightComma = right.size();

            int leftNumber = atoi(left.substr(l, leftComma - l).c_str());
            int rightNumber = atoi(right.substr(r, rightComma - r).c_str());

            if (leftNumber < rightNumber) /* left value is lower */
                return 1;
            else if (leftNumber > rightNumber) /* right value is lower */
                return -1;
        }

        l++;
        r++;
    }
}

bool sort(std::string const &left, std::string &right)
{
    return isListInOrder(left, right) == 1;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line))
    {
        if (!line.empty())
            lines.push_back(line);
    }

    lines.push_back("[[[2]]]");
    lines.push_back("[[[6]]]");

    std::sort(lines.begin(), lines.end(), sort);

    int index = 1;
    int product = 1;
    for (auto const &line : lines)
    {
        if (line == "[[[2]]]" || line == "[[[6]]]")
            product *= index;

        index++;
    }

    std::cout << product << std::endl;

    return 0;
}