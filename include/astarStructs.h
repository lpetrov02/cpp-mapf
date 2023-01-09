#pragma once

#include <algorithm>
#include <cmath>
#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "astarNodes.h"


int manhattan(int i1, int j1, int i2, int j2);


// SEARCH TREE
class SearchTree {
public:
    SearchTree();
    size_t getSize();
    bool openIsEmpty();
    void addToOpen(Node const& item);
    Node getBestNodeFromOpen();    
    void addToClosed(Node& item);
    bool wasExpanded(Node& item);
    std::vector<Node> OPEN();
    std::unordered_map<std::tuple<int, int, int>, Node, hashTuple3> CLOSED();

private:
    std::vector<Node> _open;
    std::unordered_map<std::tuple<int, int, int>, Node, hashTuple3> _closed;
};


class Map {
public:
    Map();
    void readFromString(std::string cellStr, int width, int height);    
    void setGridCells(int width, int height, std::vector<std::vector<int>> grid_cells);
    bool inBounds(int i, int j);
    bool isTraversable(int i, int j);
    std::vector<std::pair<int, int>> getNeighbors(int i, int j);
    std::pair<int, int> getSize();

private:
    int _width;
    int _height;
    std::vector<std::vector<int>> _cells;
};


class Agent {
public:
    Agent(int index, std::pair<int, int> start, std::pair<int, int> finish);
    BaseNode getStart() const;
    BaseNode getGoal() const;
    int getIndex() const;

private:
    int _index;
    BaseNode _start;
    BaseNode _goal;
};

bool operator==(Agent const& first, Agent const& second);
bool operator<(Agent const& first, Agent const& second);
std::ostream& operator<<(std::ostream& os, Agent const& node);

struct hashAgent {
    typedef Agent argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct hashAgentInt {
    typedef std::tuple<Agent, int> argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};
