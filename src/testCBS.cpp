#include <vector>
#include <string>
#include <filesystem>
#include <iostream>

#include "timeException.h"
#include "tasksFromFile.h"
#include "cbsFamily.h"


int main() {
    std::vector<std::string> directories = { 
        "../tests/Tests/brc-scen-even",
        "../tests/Tests/brc-scen-random",
         "../tests/Tests/den-scen-even",
         "../tests/Tests/den-scen-random",
         "../tests/Tests/ost-scen-even",
         "../tests/Tests/ost-scen-random" 
    };

    std::vector<std::tuple<bool, double, int, Solution>> results;

    for(auto directory: directories) 
    {
        auto mapInfo = getMap("../tests/Tests/brc202d.map");

        Map map = Map();
        map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));
        for (const auto &file : std::filesystem::directory_iterator(directories[0])) 
        {
            std::vector<task> tasks = readTasksFromFile(file.path());
            std::vector<task> tasksToDo;
            tasksToDo.push_back(tasks[0]);
            try 
            {
                std::vector<std::pair<int, int>> starts, goals;
                for(std::size_t i = 0; i < tasks.size() - 1; i++) 
                {
                    starts.push_back(std::pair<int, int>(tasksToDo[tasksToDo.size() - 1].start_x, tasksToDo[tasksToDo.size() - 1].start_y));
                    goals.push_back(std::pair<int, int>(tasksToDo[tasksToDo.size() - 1].goal_x, tasksToDo[tasksToDo.size() - 1].goal_y));
                    auto result = cbs(map, starts, goals);
                    std::cout << std::get<0>(result) << " " << std::get<1>(result) << " " << std::get<0>(result) << std::endl;
                    results.push_back(result);
                    tasksToDo.push_back(tasks[i + 1]);
                }
                starts.push_back(std::pair<int, int>(tasksToDo[tasksToDo.size() - 1].start_x, tasksToDo[tasksToDo.size() - 1].start_y));
                goals.push_back(std::pair<int, int>(tasksToDo[tasksToDo.size() - 1].goal_x, tasksToDo[tasksToDo.size() - 1].goal_y));
                auto result = cbs(map, starts, goals);
                results.push_back(result);
            }
            catch (TimeException &e)
            {
                std::cout << e.what() << std::endl;
            }
        }
    }
    return 0;
}