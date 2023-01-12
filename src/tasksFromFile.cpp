#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "tasksFromFile.h"

std::vector<task> readTasksFromFile(std::string fileName) {
    std::ifstream infile(fileName);
    std::vector<task> tasks;
    std::string version;
    int type;
    infile >> version >> type;
    int level;
    int w;
    int h;
    std::string mapFile;
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
    float len;
    while (infile >> level >> mapFile >> w >> h >> start_x >> start_y >> goal_x >> goal_y >> len)
    {
        task taskFromFile = {
            start_x,
            start_y,
            goal_x,
            goal_y
        };
        tasks.push_back(taskFromFile);
    }

    return tasks;
}

// int main() {
//     std::vector<task> tasks = readTasksFromFile("brc-scen-even/brc202d-even-1.scen");
//     for(auto t: tasks) {
//        std::cout << t.start_x << " " << t.start_y << " " << t.goal_x << " " << t.goal_y << std::endl;
//     }
//     return 0;
// }