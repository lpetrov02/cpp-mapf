#include "astarStructs.h"

int mamhattan(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}


// BASE NODE
BaseNode::BaseNode(int i, int j) : _i(i), _j(j) {}

std::tuple<int, int> BaseNode::getTuple() {
    return std::tuple<int, int>(_i, _j);
}

bool operator==(BaseNode const& other) {
    return _i == other->_i && _j == other->_j;
}

bool operator<(BaseNode const& other) { 
    if (_f == other->_f)
        return _g < other->_g;
    return _f < other->_f;
}

std::ostream& operator<<(std::ostream& os, BaseNode const& baseNode) {
    os << baseNode->_i << " " << baseNode->_j;
    return os;
}

template<>
size_t hash<BaseNode>::operator()(BaseNode const& s) const {
    return std::hash(s->getTuple());
}


// NODE
Node::Node(
    int i, int j, int g = 0, int h = 0, int f = -1, 
    Node* parent = nullptr, std::function<int(int, int, int, int)> tie_breaking_func = NULL
) : _i(i), _j(j), _g(g), _h(h) {
    _node = BaseNode(i, j);
    _time = 0;
    _f = 0;
    if (parent == nullptr):
        _time = parent->_time + 1;
        _f = _time + _h;
    _parent = parent;
}

std::tuple<int, int, int> Node::getTuple() const {
    return std::tuple<int, int, int>(_i, _j, _time);
}

bool operator==(Node const& other) {
    return _i == other->_i && _j == other->_j && _time == other->_time;
}

bool operator<(Node const& other) { 
    if (_f == other->_f)
        return _g < other->_g;
    return _f < other->_f;
}

std::ostream& operator<<(std::ostream& os, Node const& node) {
    os << node->_i << " " << node->_j << " " << node->_time;
    return os;
}

template<>
size_t hash<Node>::operator()(Node const& s) const {
    return std::hash(s->getTuple());
}

// SEARCH TREE
SearchTree::SearchTree() {
    _open = std::vector<Node>();
    _closed = std::unordered_set<Node>();
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

Node& SearchTree::getBestNodeFromOpen() {
    std::pop_heap(_open.begin(), _open.end());
    auto bestNode = _open.pop_back();
    while (was_expanded(best_node) and _open.size() > 0) {
        std::pop_heap(_open.begin(), _open.end());
        bestNode = _open.pop_back(); 
    }
    return bestNode;
}
    
void SearchTree::addToClosed(Node& item) {
    _closed.insert(item);
}

bool SearchTree::wasExpanded(Node& item) {
    return std::find(_closed.begin(), _closed.end(), item);
}

    std::vector<Node> SearchTree::OPEN() {
        return _open;
    }
    
    std::unordered_set<Node> SearchTree::CLOSED() {
        return _closed;
    }


// MAP
Map::Map() {
    _width = 0;
    _height = 0;
    _cells = nullptr;
}

Map::~Map() {
    delete _cells;
}

void Map::readFromString(std::string cellStr, int width, int height) {
    _width = width;
    _height = height;
    _cells = new int[height][width];
    if (!_cells) {
        delete this;
        return;
    }

    std::string line;
    std::stringstream input(cellStr);
    int i = 0, j = 0;

    while (std::getline(input, line, '\n')) {
        if (line.size() != 0) {
            i = 0
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
                throw std::exception("Size Error. Map width = " + std::to_string(i) + ", but must be" + std::to_string(width));
            }
            j += 1;
        }
    }

    if j != height:
        throw std::exception("Size Error. Map height = " + std::to_string(j) + ", but must be" + std::to_string(height));
}

    
def Map::setGridCells(int width, int height, int** grid_cells) {
    _width = width;
    _height = height;
    _cells = grid_cells;
}


def Map::inBounds(int i, int j) {
    return (0 <= j && j < self._width) && (0 <= i && i < self._height);
}


def Map::isTraversable(i, j) {
    return !_cells[j][i];
}


std::vector<std::pair<int, int> Map::getNeighbors(i, j) {  
    auto neighbors = std::vector<std::pair<int, int>>();
    std::vector<int*> delta = { {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {0, 0} };
    for (auto& d : delta) {
        if self.in_bounds(i + d[0], j + d[1]) and self.traversable(i + d[0], j + d[1]):
            neighbors.push_back({i + d[0], j + d[1]});
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

BaseNode Agent::getStart() {
    return _start;
}

BaseNode Agent::getGoal() {
    return _goal;
}

BaseNode Agent::getIndex() {
    return _index;
}

bool operator==(Agent const& other) {
    return _index == other->_index;    
}

bool operator<(Agent const& other) {
    return _index < other->_index;
}

std::ostream& operator<<(std::ostream& os, Agent const& agent) {
    os << "Agent #" << agent->_index;
    return os;
}

template<>
struct hash<Agent>
{
    typedef Agent argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

template<>
size_t hash<Agent>::operator()(Agent const& s) const {
    return std::hash(_index);
}
