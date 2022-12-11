#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

struct entry
{
    bool dir;
    unsigned int size;
    std::string name;
};

std::map<std::string, std::vector<entry>> content;

unsigned int getFolderSize(std::string path, std::vector<entry> entries)
{
    unsigned int size = 0;

    for (auto const &entry : entries)
    {
        if (!entry.dir)
            size += entry.size;
        else
            size += getFolderSize(path + "/" + entry.name, content[path + "/" + entry.name]);
    }

    return size;
}

int main(int, char *argv[])
{
    std::ifstream input(argv[1]);

    std::string line;
    std::vector<std::string> dirs;
    std::string currentPath = "";
    unsigned int count = 0;

    while (std::getline(input, line))
    {
        if (line.at(0) == '$' && line.substr(2, 2) == "cd")
        {
            std::string path = line.substr(5);
            if (path == "/")
                currentPath = std::string("/");
            else if (path == "..")
                currentPath = currentPath.substr(0, currentPath.find_last_of("/"));
            else
                currentPath += "/" + path;

            content.insert({currentPath, std::vector<entry>()});
        }
        else if (line.at(0) != '$')
        {
            entry t;
            if (line.substr(0, 3) == "dir")
            {
                t = {true, 0, line.substr(4)};
            }
            else
            {
                t.dir = false;
                std::stringstream ss(line);
                ss >> t.size >> t.name;
            }

            content[currentPath].push_back(t);
        }
    }

    for (auto it = content.cbegin(); it != content.cend(); ++it)
    {
        unsigned int size = getFolderSize(it->first, it->second);
        if (size <= 100000)
            count += size;
    }

    std::cout << count << std::endl;

    return 0;
}