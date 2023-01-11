#pragma once

#include <fstream>
#include <vector>

#include "astarNodes.h"
#include "cbsStructs.h"


std::tuple<int, int, std::string> getMap(std::string filename);
void writePathToFile(std::vector<BaseNode> path, std::string filename);
void writeCbsSolution(Solution solution, std::string filename);