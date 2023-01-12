#pragma once

#include "astar.h"
#include "forTesting.h"
#include "cbsStructs.h"
#include "cbsDsStructs.h"


std::tuple<bool, double, int, Solution> cbs(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::function<int(int, int, int, int)> heuristicFunc = manhattan
);

std::tuple<bool, double, int, Solution> cbsDS(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::function<int(int, int, int, int)> heuristicFunc = manhattan
);