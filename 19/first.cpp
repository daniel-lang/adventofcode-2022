#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>

struct blueprint
{
    int index = 0;
    int oreCostOre = 0;
    int clayCostOre = 0;
    int obsidianCostOre = 0;
    int obsidianCostClay = 0;
    int geodeCostOre = 0;
    int geodeCostObsidian = 0;
};

struct state
{
    int ore = 0;
    int clay = 0;
    int obsidian = 0;
    int geode = 0;

    int oreRobots = 1;
    int clayRobots = 0;
    int obsidianRobots = 0;
    int geodeRobots = 0;

    int minute = 0;
};

std::string stateToString(state s)
{
    return std::to_string(s.minute) + "," +
           std::to_string(s.ore) + "," +
           std::to_string(s.clay) + "," +
           std::to_string(s.obsidian) + "," +
           std::to_string(s.geode) + "," +
           std::to_string(s.oreRobots) + "," +
           std::to_string(s.clayRobots) + "," +
           std::to_string(s.obsidianRobots) + "," +
           std::to_string(s.geodeRobots);
}

std::vector<blueprint> blueprints;

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    int count = 0;

    while (std::getline(input, line))
    {
        blueprint bp;
        std::sscanf(line.c_str(), "Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian.",
                    &bp.index, &bp.oreCostOre, &bp.clayCostOre, &bp.obsidianCostOre, &bp.obsidianCostClay, &bp.geodeCostOre, &bp.geodeCostObsidian);

        blueprints.push_back(bp);
    }

    for (auto const &bp : blueprints)
    {
        int maxGeode = 0;
        std::queue<state> queue;
        std::set<std::string> visited;
        state start;
        start.minute = 24;
        queue.push(start);

        int maxOre = std::max(bp.oreCostOre, std::max(bp.clayCostOre, std::max(bp.obsidianCostOre, bp.geodeCostOre)));
        int maxClay = bp.obsidianCostClay;
        int maxObsidian = bp.geodeCostObsidian;

        while (queue.size() > 0)
        {
            state s = queue.front();
            queue.pop();

            /* capping the number of robots, we don't need more than the maximum amount we can spend per minute */
            s.oreRobots = std::min(s.oreRobots, maxOre);
            s.clayRobots = std::min(s.clayRobots, maxClay);
            s.obsidianRobots = std::min(s.obsidianRobots, maxObsidian);

            /* capping the number of minerals */
            s.ore = std::min(s.ore, maxOre * s.minute - s.oreRobots * (s.minute - 1));
            s.clay = std::min(s.clay, maxClay * s.minute - s.clayRobots * (s.minute - 1));
            s.obsidian = std::min(s.obsidian, maxObsidian * s.minute - s.obsidianRobots * (s.minute - 1));

            if (visited.count(stateToString(s)) > 0)
                continue;

            visited.insert(stateToString(s));

            bool buildOre = s.ore >= bp.oreCostOre;
            bool buildClay = s.ore >= bp.clayCostOre;
            bool buildObsidian = s.ore >= bp.obsidianCostOre && s.clay >= bp.obsidianCostClay;
            bool buildGeode = s.ore >= bp.geodeCostOre && s.obsidian >= bp.geodeCostObsidian;

            s.ore += s.oreRobots;
            s.clay += s.clayRobots;
            s.obsidian += s.obsidianRobots;
            s.geode += s.geodeRobots;
            s.minute--;

            if (s.geode > maxGeode)
                maxGeode = s.geode;

            if (s.minute == 0)
                continue;

            if (buildGeode)
            {
                state sNew = s;
                sNew.ore -= bp.geodeCostOre;
                sNew.obsidian -= bp.geodeCostObsidian;
                sNew.geodeRobots++;
                queue.push(sNew);
            }

            if (buildObsidian)
            {
                state sNew = s;
                sNew.ore -= bp.obsidianCostOre;
                sNew.clay -= bp.obsidianCostClay;
                sNew.obsidianRobots++;
                queue.push(sNew);
            }

            if (buildClay)
            {
                state sNew = s;
                sNew.ore -= bp.clayCostOre;
                sNew.clayRobots++;
                queue.push(sNew);
            }

            if (buildOre)
            {
                state sNew = s;
                sNew.ore -= bp.oreCostOre;
                sNew.oreRobots++;
                queue.push(sNew);
            }

            /* build nothing */
            queue.push(s);
        }

        std::cout << bp.index << ": " << maxGeode << "\n";

        count += bp.index * maxGeode;
    }

    std::cout << count << std::endl;

    return 0;
}