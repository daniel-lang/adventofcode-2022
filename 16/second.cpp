#include <vector>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>

struct valve
{
    uint16_t flow;
    std::vector<std::string> tunnels;
};

std::map<std::string, valve> valves;
std::vector<std::string> neededValves;
std::vector<uint16_t> flow;
uint32_t closedValves;
uint16_t neededValveCount;
uint16_t startPosition;
std::map<uint16_t, std::map<uint16_t, uint16_t>> directions;

uint8_t getValveIndex(std::string const &valve)
{
    auto it = std::find(neededValves.cbegin(), neededValves.cend(), valve);
    return it - neededValves.cbegin();
}

uint16_t run(uint16_t position, uint16_t minute, uint16_t minute2 = 0, bool first = false)
{
    uint16_t best = 0;

    if (!closedValves)
        return 0;

    if (minute2 > 0)
        best = run(startPosition, minute2);

    auto mapping = directions.at(position);

    for (uint16_t i = 0; i < neededValveCount; ++i)
    {
        if ((closedValves & (1 << i)) == 0)
            continue;

        if (position == i)
            continue;

        auto distance = mapping.at(i);

        if (minute - distance <= 0)
            continue;

        closedValves &= ~(1 << i);
        best = std::max(best, static_cast<uint16_t>(flow[i] * (minute - distance - 1) + run(i, minute - distance - 1, minute2)));
        closedValves |= (1 << i);

        if (first)
            std::cout << "finished permutation " << neededValves[i] << ": " << best << "\n";
    }

    return best;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;

    neededValveCount = 0;

    while (std::getline(input, line))
    {
        char id[3];
        valve tmp;
        sscanf(line.c_str(), "Valve %s has flow rate=%hu", id, &(tmp.flow));

        std::string stringId(id);
        if (tmp.flow > 0 || stringId == "AA")
        {
            neededValves.push_back(stringId);
            flow.push_back(tmp.flow);
            closedValves |= 1 << neededValveCount;
            neededValveCount++;
            std::cout << id << " ";
        }

        std::string tunnels;
        if (line.find("to valves ") != std::string::npos)
            tunnels = line.substr(line.find("to valves ") + 10);
        else
            tunnels = line.substr(line.find("to valve ") + 9);

        int offset = 0;
        int count = std::count(tunnels.cbegin(), tunnels.cend(), ',') + 1;
        for (int i = 0; i < count; ++i)
        {
            tmp.tunnels.push_back(tunnels.substr(offset, 2));
            offset += 4;
        }

        valves.insert({stringId, tmp});
    }

    std::cout << "\n";

    for (auto const &valve : neededValves)
    {
        std::map<uint16_t, uint16_t> mapping;
        std::queue<std::string> queue;
        std::vector<std::string> visited;
        unsigned int steps = 0;
        int nodesInNextLayer = 0;
        int nodesLeftInLayer = 1;
        queue.push(valve);
        visited.push_back(valve);

        while (queue.size() > 0)
        {
            auto v = queue.front();
            queue.pop();

            if (v != valve && std::find(neededValves.cbegin(), neededValves.cend(), v) != neededValves.cend())
            {
                std::cout << valve << " -> " << v << ": " << steps << "\n";
                mapping.insert({getValveIndex(v), steps});
            }

            for (auto const &t : valves[v].tunnels)
            {
                if (std::find(visited.cbegin(), visited.cend(), t) != visited.cend())
                    continue;

                queue.push(t);
                visited.push_back(t);
                nodesInNextLayer++;
            }

            nodesLeftInLayer--;
            if (nodesLeftInLayer == 0)
            {
                nodesLeftInLayer = nodesInNextLayer;
                nodesInNextLayer = 0;
                steps++;
            }
        }
        directions.insert({getValveIndex(valve), mapping});
    }

    startPosition = getValveIndex("AA");
    closedValves |= (1 << startPosition);
    uint16_t maxFlown = run(startPosition, 26, 26, true);
    std::cout << maxFlown << std::endl;

    return 0;
}