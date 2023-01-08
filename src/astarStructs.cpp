#include "astarStructs.h"


// SEARCH TREE
SearchTree::SearchTree() {
    _open = std::vector<Node>();
    _closed = std::unordered_map<std::tuple<int, int, int>, Node, hashTuple3>();
}
    
size_t SearchTree::getSize() {
    return _open.size() + _closed.size();
}

bool SearchTree::openIsEmpty() {
    return _open.size() == 0;
}

void SearchTree::addToOpen(Node const& item) {
    _open.push_back(item);
    std::push_heap<std::vector<Node>::iterator, InversedComparator>(_open.begin(), _open.end(), InversedComparator());
}

Node SearchTree::getBestNodeFromOpen() {
    std::pop_heap<std::vector<Node>::iterator, InversedComparator>(_open.begin(), _open.end(), InversedComparator());
    auto bestNode = _open[_open.size() - 1];
    _open.pop_back();
    while (wasExpanded(bestNode) and _open.size() > 0) {
        std::pop_heap<std::vector<Node>::iterator, InversedComparator>(_open.begin(), _open.end(), InversedComparator());
        bestNode = _open[_open.size() - 1];
        _open.pop_back(); 
    }
    return bestNode;
}
    
void SearchTree::addToClosed(Node& item) {
    _closed.insert(std::pair<std::tuple<int, int, int>, Node>(item.getTuple(), item));
}

bool SearchTree::wasExpanded(Node& item) {
    return _closed.contains(item.getTuple());
}

std::vector<Node> SearchTree::OPEN() {
    return _open;
}

std::unordered_map<std::tuple<int, int, int>, Node, hashTuple3> SearchTree::CLOSED() {
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
