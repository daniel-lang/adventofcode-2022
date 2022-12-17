#include <vector>
#include <fstream>
#include <iostream>
#include <array>
#include <unordered_map>

struct point
{
    int x;
    int y;
};

struct form
{
    std::vector<point> points;
    int height;
    int width;
};

enum class content
{
    AIR,
    ROCK,
    ROCK_FALLING
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

int maxY = 0;
form currentForm;
point currentFormPosition;
std::unordered_map<std::string, content> grid;
std::vector<int> gases;

std::string pointToString(point p)
{
    return std::to_string(p.x) + "," + std::to_string(p.y);
}

void print()
{
    int height = currentFormPosition.y + currentForm.height;

    for (int y = height - 1; y >= 0; y--)
    {
        std::cout << "|";

        for (int x = 0; x < 7; x++)
        {
            std::string coord = pointToString({x, y});
            content c = content::AIR;
            if (grid.count(coord) == 1)
                c = grid.at(coord);

            for (auto const &p : currentForm.points)
                if (currentFormPosition.x + p.x == x && currentFormPosition.y + p.y == y)
                    c = content::ROCK_FALLING;

            if (c == content::ROCK)
                std::cout << "#";
            else if (c == content::ROCK_FALLING)
                std::cout << "@";
            else
                std::cout << ".";
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

    int rocks = 0;
    size_t gas = 0;
    currentForm = forms[0];
    currentFormPosition = {2, 3};

    // print();

    while (rocks < 2022)
    {
        bool gasPossible = true;
        for (auto const &p : currentForm.points)
        {
            auto newX = currentFormPosition.x + p.x + gases[gas];
            auto coord = pointToString({newX, currentFormPosition.y + p.y});
            if (newX < 0 || newX > 6 || (grid.count(coord) == 1 && grid.at(coord) != content::AIR))
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
            auto coord = pointToString({currentFormPosition.x + p.x, newY});
            if (newY < 0 || (grid.count(coord) == 1 && grid.at(coord) != content::AIR))
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
                grid.insert({pointToString({p.x + currentFormPosition.x, p.y + currentFormPosition.y}), content::ROCK});

            auto tmp = currentFormPosition.y + currentForm.height;
            if (tmp > maxY)
                maxY = tmp;

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