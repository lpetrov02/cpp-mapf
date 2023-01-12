#pragma once

#include <string>
#include <vector>

typedef struct {
    int start_x;
    int start_y;
    int goal_x;
    int goal_y;
} task;

std::vector<task> readTasksFromFile(std::string fileName);
