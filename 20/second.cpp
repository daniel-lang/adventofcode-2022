#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

struct node
{
    std::shared_ptr<node> prev = nullptr;
    long long value = 0;
    int originalIndex = 0;
    bool mixed = false;
    std::shared_ptr<node> next = nullptr;
};

std::shared_ptr<node> head = nullptr;
std::shared_ptr<node> tail = nullptr;
long long multiplier = 811589153;

void print()
{
    auto node = head;
    while (node != nullptr)
    {
        std::cout << node->value << " ";
        node = node->next;
    }

    std::cout << std::endl
              << std::endl;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    int value = 0;
    int index = 0;

    while (input >> value)
    {
        auto tmp = std::make_shared<node>();
        tmp->value = value * multiplier;
        tmp->originalIndex = index;

        if (head == nullptr)
            head = tmp;

        if (tail != nullptr)
        {
            tmp->prev = tail;
            tail->next = tmp;
        }

        tail = tmp;
        index++;
    }

    // print();

    for (int m = 0; m < 10; ++m)
    {
        for (int i = 0; i < index; ++i)
        {
            auto node = head;
            while (node != nullptr)
            {
                if (node->originalIndex == i)
                    break;

                node = node->next;
            }

            auto target = node;
            auto moves = node->value % (index - 1);
            if (moves == 0)
            {
                continue;
            }
            if (moves > 0)
            {
                for (int j = moves; j > 0; --j)
                {
                    target = target->next;
                    if (target == nullptr) // wrap to beginning
                        target = head;
                }
            }
            else
            {
                for (int j = moves; j <= 0; ++j)
                {
                    target = target->prev;
                    if (target == nullptr) // wrap to end
                        target = tail;
                }
            }

            if (node == head)
            {
                head = node->next;
                head->prev = nullptr;
            }
            else if (node == tail)
            {
                tail = node->prev;
                tail->next = nullptr;
            }
            else
            {
                node->next->prev = node->prev;
                node->prev->next = node->next;
            }

            auto tmp = target->next;
            target->next = node;
            node->next = tmp;
            node->prev = target;

            if (tmp != nullptr)
                tmp->prev = node;
            else
                tail = node;
        }

        // print();
    }

    int i0 = 0;
    auto node = head;
    while (node != nullptr)
    {
        if (node->value == 0)
            break;

        node = node->next;
        i0++;
    }

    int i1 = (i0 + 1000) % index;
    int i2 = (i0 + 2000) % index;
    int i3 = (i0 + 3000) % index;
    long long v1 = 0;
    long long v2 = 0;
    long long v3 = 0;

    node = head;
    for (int i = 0; i <= std::max(i1, std::max(i2, i3)); ++i)
    {
        if (i == i1)
            v1 = node->value;
        if (i == i2)
            v2 = node->value;
        if (i == i3)
            v3 = node->value;

        node = node->next;
    }

    std::cout << index << " -> " << i0 << ", " << i1 << ", " << i2 << ", " << i3 << "\n";
    std::cout << v1 << ", " << v2 << ", " << v3 << " -> " << (v1 + v2 + v3) << "\n";
    return 0;
}