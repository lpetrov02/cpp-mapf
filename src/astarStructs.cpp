#include "astarStructs.h"


int manhattan(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}


// BASE NODE
BaseNode::BaseNode(int i, int j) : _i{i}, _j{j} {}

std::tuple<int, int> BaseNode::getTuple() const {
    return std::tuple<int, int>(_i, _j);
}

bool operator==(BaseNode const& first, BaseNode const& second) {
    return first._i == second._i && first._j == second._j;
}

bool operator<(BaseNode const& first, BaseNode const& second) { 
    if (first._i == second._i)
        return first._j < second._j;
    return first._i < second._i;
}

std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode) {
    os << baseNode._i << " " << baseNode._j;
    return os;
}

std::size_t hashBaseNode::operator()(BaseNode const& s) const {
     auto ij = s.getTuple();
     return std::hash<int>{}(std::get<0>(ij)) ^ std::hash<int>{}(std::get<1>(ij));
}


// NODE
Node::Node(int i, int j, int g, int h, Node* parent) 
    : _i{i}, _j{j}, _g{g}, _h{h}, _time{0}, _node{BaseNode(i, j)}, _parent(parent) {
    if (parent != nullptr) {
        _time = parent->_time + 1;
        _f = _time + _h;
    }
}

std::tuple<int, int, int> Node::getTuple() const {
    return std::tuple<int, int, int>(_i, _j, _time);
}

bool operator==(Node const& first, Node const& second) {
    return first._i == second._i && first._j == second._j && first._time == second._time;
}

bool operator<(Node const& first, Node const& second) { 
    if (first._f == second._f)
        return first._g < second._g;
    return first._f < second._f;
}

std::ostream& operator<<(std::ostream& os, Node const& node) {
    os << node._i << " " << node._j << " " << node._time;
    return os;
}

std::size_t hashNode::operator()(Node const& s) const {
    auto ijt = s.getTuple();
    return std::hash<int>{}(std::get<0>(ijt)) ^ std::hash<int>{}(std::get<1>(ijt)) ^ std::hash<int>{}(std::get<2>(ijt));
}


// SEARCH TREE
SearchTree::SearchTree() {
    _open = std::vector<Node>();
    _closed = std::unordered_set<Node, hashNode>();
}
    
size_t SearchTree::getSize() {
    return _open.size() + _closed.size();
}

bool SearchTree::openIsEmpty() {
    return _open.size() == 0;
}

void SearchTree::addToOpen(Node const& item) {
    _open.push_back(item);
    std::push_heap(_open.begin(), _open.end());
}

Node SearchTree::getBestNodeFromOpen() {
    auto bestNode = _open[0];
    std::pop_heap(_open.begin(), _open.end());
    _open.pop_back();
    while (wasExpanded(bestNode) and _open.size() > 0) {
        bestNode = _open[0];
        std::pop_heap(_open.begin(), _open.end());
        _open.pop_back(); 
    }
    return bestNode;
}
    
void SearchTree::addToClosed(Node& item) {
    _closed.insert(item);
}

bool SearchTree::wasExpanded(Node& item) {
    return _closed.contains(item);
}

    std::vector<Node> SearchTree::OPEN() {
        return _open;
    }
    
    std::unordered_set<Node, hashNode> SearchTree::CLOSED() {
        return _closed;
    }


// MAP
Map::Map() {
    _width = 10;
    _height = 10;
    _cells = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
}

void Map::readFromString(std::string cellStr, int width, int height) {
    _width = width;
    _height = height;
    _cells = std::vector<std::vector<int>>(height, std::vector<int>(width, 0));

    std::string line;
    std::stringstream input(cellStr);
    int i = 0, j = 0;

    while (std::getline(input, line, '\n')) {
        if (line.size() != 0) {
            i = 0;
            for (auto& c : line) {
                if (c == '.')
                    _cells[j][i] = 0;
                else if (c == '#')
                    _cells[j][i] = 1;
                else
                    continue;
                i += 1;
            }
            if (i != width) {
                throw std::out_of_range("Size Error. Map width = " + std::to_string(i) + ", but must be" + std::to_string(width));
            }
            j += 1;
        }
    }

    if (j != height) {
        throw std::out_of_range("Size Error. Map height = " + std::to_string(j) + ", but must be" + std::to_string(height));
    }
}

    
void Map::setGridCells(int width, int height, std::vector<std::vector<int>> grid_cells) {
    _width = width;
    _height = height;
    _cells = grid_cells;
}


bool Map::inBounds(int i, int j) {
    return (0 <= j && j < _width) && (0 <= i && i < _height);
}


bool Map::isTraversable(int i, int j) {
    return !_cells[j][i];
}


std::vector<std::pair<int, int>> Map::getNeighbors(int i, int j) {  
    auto neighbors = std::vector<std::pair<int, int>>();
    std::vector<std::pair<int, int>> delta = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0} };
    for (auto& d : delta) {
        if (inBounds(i + d.first, j + d.second) and isTraversable(i + d.first, j + d.second)) {
            neighbors.push_back({i + d.first, j + d.second});
        }
    }
    return neighbors;
}

std::pair<int, int> Map::getSize() {
    return std::pair<int, int>(_height, _width);
}


// AGENT
Agent::Agent(int index, std::pair<int, int> start, std::pair<int, int> finish) {
    _index = index;
    _start = BaseNode(start.first, start.second);
    _goal = BaseNode(finish.first, finish.second);
}

BaseNode Agent::getStart() const {
    return _start;
}

BaseNode Agent::getGoal() const {
    return _goal;
}

int Agent::getIndex() const {
    return _index;
}

bool operator==(Agent const& first, Agent const& second) {
    return first.getIndex() == second.getIndex();    
}

bool operator<(Agent const& first, Agent const& second) {
    return first.getIndex() < second.getIndex();
}

std::ostream& operator<<(std::ostream& os, Agent const& agent) {
    os << "Agent #" << agent.getIndex();
    return os;
}

std::size_t hashAgent::operator()(Agent const& s) const {
    return std::hash<int>{}(s.getIndex());
}
