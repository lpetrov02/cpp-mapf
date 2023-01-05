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


int mamhattan(int i1, int j1, int i2, int j2);


struct BaseNode {
    template<>
    friend struct hash<BaseNode>;
    friend bool operator==(BaseNode const& other);
    friend bool operator<(NaseNode const& other);
    friend std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode);
public:
    BaseNode(int i, int j);
    std::tuple<int, int> getTuple();

public:
    int _i, _j;
};

bool operator==(BaseNode const& other);
bool operator<(BaseNode const& other);
std::ostream& operator<<(std::ostream& os, BaseNode const& node);

template<>
struct hash<BaseNode>
{
    typedef BaseNode argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


class Node {
    template<>
    friend struct hash<Node>;
    friend bool operator==(Node const& other);
    friend bool operator<(Node const& other);
    friend std::ostream& operator<<(std::ostream& os, const Node& node);
public:
    Node(int i, int j, int g = 0, int h = 0, int f = None, 
        Node* parent = None, std::function<int(int, int, int, int)> tie_breaking_func = None);
    std::tuple<int, int, int> getTuple() const;

private:
    BaseNode _node;
    Node* parent;
public:
    int _i, _j, _time, _g, _h, _f;

};

bool operator==(Node const& other);
bool operator<(Node const& other);
std::ostream& operator<<(std::ostream& os, Node const& node);

template<>
struct hash<Node>
{
    typedef Node argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


class SearchTree {
public:
    SearchTree();
    size_t getSize();
    bool openIsEmpty();
    void addToOpen(Node const& item);
    Node& getBestNodeFromOpen();    
    void addToClosed(Node& item);
    bool wasExpanded(Node& item);
    std::vector<Node> OPEN();
    std::unordered_set<Node> CLOSED();

private:
    std::vector<Node> _open;
    std::unordered_set<Node> _closed;
};


class Map {
public:
    Map();
    ~Map();    
    void readFromString(std::string cellStr, int width, int height);    
    def setGridCells(int width, int height, int** grid_cells);
    def inBounds(int i, int j);
    def isTraversable(i, j);
    std::vector<std::pair<int, int> getNeighbors(i, j);
    std::pair<int, int> getSize();

private:
    int _width;
    int _heigt;
    int** _cells;
};


class Agent {
public:
    Agent(int index, std::pair<int, int> start = {0, 0}, std::pair<int, int> finish = {0, 0});
    BaseNode getStart();
    BaseNode getGoal();
    BaseNode getIndex();

private:
    int _index;
    BaseNode _start;
    BaseNode _goal;
};

bool operator==(Agent const& other);
bool operator<(Agent const& other);
std::ostream& operator<<(std::ostream& os, Agent const& node);

template<>
struct hash<Agent>
{
    typedef Agent argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};