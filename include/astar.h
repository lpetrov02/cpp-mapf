#pragma once


#include "astarStructs.h"
#include "constraints.h"


struct AstarRes {
public:
    AstarRes(bool found, std::vector<BaseNode> path, int steps);

public:
    bool _found;
    int _steps;
    std::vector<BaseNode> _path;
};


int computeCost(int i1, int j1, int i2, int j2);
int manhattan(int i1, int j1, int i2, int j2);

AstarRes astar(Map gridMap, Agent agent, Constraints* constraints, std::function<int(int, int, int, int)> heuristicFunc = manhattan);