#pragma once

#include <algorithm>
#include <cmath>
#include <vector>
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


int manhattan(int i1, int j1, int i2, int j2);


struct BaseNode {
    friend bool operator==(BaseNode const& first, BaseNode const& second);
    friend bool operator<(BaseNode const& first, BaseNode const& second);
    friend std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode);
public:
    BaseNode(int i = 0, int j = 0);
    std::tuple<int, int> getTuple() const;

public:
    int _i;
    int _j;
};

bool operator==(BaseNode const& first, BaseNode const& second);
bool operator<(BaseNode const& first, BaseNode const& second);
std::ostream& operator<<(std::ostream& os, BaseNode const& node);

struct hashBaseNode {
    typedef BaseNode argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


class Node {
    friend bool operator==(Node const& first, Node const& second);
    friend bool operator<(Node const& first, Node const& second);
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
public:
    Node(int i, int j, int g = 0, int h = 0, std::tuple<int, int, int> parentTuple = std::tuple<int, int, int>(-1, -1, -1));
    std::tuple<int, int, int> getTuple() const;
    std::tuple<int, int, int> getParentTuple() const;

public:
    int _i;
    int _j;
    int _g;
    int _h;
    int _time;
    int _f;

private:
    BaseNode _node;
    std::tuple<int, int, int> _parentTuple;
};

bool operator==(Node const& first, Node const& second);
bool operator<(Node const& first, Node const& second);
std::ostream& operator<<(std::ostream& os, Node const& node);

struct hashNode {
    typedef Node argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct hashTuple3 {
    typedef std::tuple<int, int, int> argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


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