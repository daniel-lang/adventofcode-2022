#include <vector>
#include <fstream>
#include <iostream>
#include <array>
#include <unordered_map>

struct point
{
    long x;
    long y;
};

struct form
{
    std::vector<point> points;
    int height;
    int width;
};

struct state
{
    int gasIndex;
    int formIndex;
    std::vector<uint8_t> grid;
    long height;
    long rocks;
};

std::vector<point> pointsForm1 = {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0}};

form form1 = {pointsForm1, 1, 4};

std::vector<point> pointsForm2 = {
    {1, 0},
    {0, 1},
    {1, 1},
    {2, 1},
    {1, 2}};

form form2 = {pointsForm2, 3, 3};

std::vector<point> pointsForm3 = {
    {0, 0},
    {1, 0},
    {2, 0},
    {2, 1},
    {2, 2}};

form form3 = {pointsForm3, 3, 3};

std::vector<point> pointsForm4 = {
    {0, 0},
    {0, 1},
    {0, 2},
    {0, 3}};

form form4 = {pointsForm4, 4, 1};

std::vector<point> pointsForm5 = {
    {0, 0},
    {1, 0},
    {0, 1},
    {1, 1}};

form form5 = {pointsForm5, 2, 2};

std::array<form, 5> forms = {form1, form2, form3, form4, form5};

long maxY = 0;
form currentForm;
point currentFormPosition;
std::vector<uint8_t> grid;
std::vector<int> gases;
std::vector<state> states;

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void print()
{
    long height = currentFormPosition.y + currentForm.height;

    for (long y = height - 1; y >= 0; y--)
    {
        std::cout << "|";
        uint8_t row = 0;
        if (y < static_cast<int>(grid.size()))
            row = grid[y];

        for (long x = 0; x < 7; x++)
        {
            if (row & (1 << x))
                std::cout << "#";
            else
            {
                bool falling = false;
                for (auto const &p : currentForm.points)
                    if (currentFormPosition.x + p.x == x && currentFormPosition.y + p.y == y)
                        falling = true;

                if (falling)
                 std::cout << "@";
                else
                    std::cout << ".";
            }
        }

        std::cout << "|\n";
    }

    std::cout << "+-------+\n"
              << std::endl;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    char movement;

    while (input >> movement)
    {
        if (movement == '>')
            gases.push_back(1);
        else
            gases.push_back(-1);
    }

    long rocks = 0;
    size_t gas = 0;
    bool found = false;
    currentForm = forms[0];
    currentFormPosition = {2, 3};

    // print();
    while (!found)
    {
        bool gasPossible = true;
        for (auto const &p : currentForm.points)
        {
            auto newX = currentFormPosition.x + p.x + gases[gas];
            auto mask = 1 << newX;
            auto y = currentFormPosition.y + p.y;
            if (newX < 0 || newX > 6 || (y < static_cast<long>(grid.size()) && grid[y] & mask))
            {
                gasPossible = false;
                break;
            }
        }

        if (gasPossible)
            currentFormPosition.x += gases[gas];

        bool downPossible = true;
        for (auto const &p : currentForm.points)
        {
            auto newY = currentFormPosition.y + p.y - 1;
            auto mask = 1 << (currentFormPosition.x + p.x);
            if (newY < 0 || (newY < static_cast<long>(grid.size()) && grid[newY] & mask))
            {
                downPossible = false;
                break;
            }
        }

        if (downPossible)
            currentFormPosition.y--;
        else
        {
            for (auto const &p : currentForm.points)
            {
                auto x = currentFormPosition.x + p.x;
                auto y = currentFormPosition.y + p.y;

                while (y >= static_cast<long>(grid.size()))
                    grid.push_back(0);

                grid[y] |= (1 << x);
            }

            auto tmp = currentFormPosition.y + currentForm.height;
            if (tmp > maxY)
                maxY = tmp;

            state s;
            s.formIndex = rocks % forms.size();
            s.gasIndex = gas;
            s.height = maxY;
            s.rocks = rocks + 1;
            std::copy(grid.cend() - 80, grid.cend(), std::back_inserter(s.grid));

            for (auto const &state : states)
            {
                if (s.formIndex != state.formIndex || s.gasIndex != state.gasIndex)
                    continue;

                auto it = std::mismatch(s.grid.cbegin(), s.grid.cend(), state.grid.cbegin());
                if (it.first != s.grid.cend())
                    continue;


                long rocksInSequence = s.rocks - state.rocks;
                long heightInSequence = s.height - state.height;
                long remainingRocks = 1000000000000 - s.rocks;

                std::cout << "H" << state.height << "/R" << state.rocks << " to H" << s.height << "/R" << s.rocks << " -> H" << heightInSequence << "/R" << rocksInSequence << "\n";

                if (remainingRocks % rocksInSequence != 0)
                    continue;

                long repetitions = remainingRocks / rocksInSequence;
                maxY += heightInSequence * repetitions;
                found = true;
            }

            states.push_back(s);

            rocks++;
            currentForm = forms[rocks % forms.size()];
            currentFormPosition = {2, 3 + maxY};
            // print();
        }

        gas++;
        if (gas >= gases.size())
            gas = 0;
    }

    std::cout << maxY << std::endl;

    return 0;
}