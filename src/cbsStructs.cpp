#include "cbsStructs.h"


// SOLUTION
Solution::Solution() {
    _solutions = std::unordered_map<Agent, std::vector<BaseNode>, hashAgent>();
    _cost = 0;
}

void Solution::addAgentPath(Agent agent, std::vector<BaseNode> path) {
    if (_solutions.contains(agent)) {
        _cost -= _solutions[agent].size();
    }
    _solutions[agent] = path;
    _cost += path.size();
}

void Solution::updateCost() {
    _cost = 0;
    for (auto& agentAndPath : _solutions) {
        _cost += agentAndPath.second.size();
    }
}

int Solution::getCost() const {
    return _cost;
}

std::unordered_map<Agent, std::vector<BaseNode>, hashAgent> Solution::getSolutions() {
    return _solutions;
}


// HIGH LEVEL NODE
HighLevelNode::HighLevelNode() {
    _constraints = NegativeConstraints();
    _solution = Solution();
}

void HighLevelNode::resolve(Agent agent, std::vector<BaseNode> path) {
    std::reverse(path.begin(), path.end());
    _solution.addAgentPath(agent, std::move(path));
}

int HighLevelNode::getCost() const {
    return _solution.getCost();
}

Solution HighLevelNode::getSolution() {
    return _solution;
}

NegativeConstraints* HighLevelNode::getConstraints() {
    return &_constraints;
}

std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int> HighLevelNode::findVertexConflicts() {
    auto paths = _solution.getSolutions();

    int maxLen = 0;
    for (auto& [agent, path] : paths) {
        if (path.size() > static_cast<std::size_t>(maxLen)) {
            maxLen = path.size();
        }
    }

    bool conflictsFound = false;
    BaseNode conflictNode = BaseNode(0, 0);
    int time;

    for (int t = 0; t < maxLen; t++) {
        std::unordered_map<BaseNode, std::unordered_set<Agent, hashAgent>, hashBaseNode> points;
        for (auto& [agent, path] : paths) {
            time = path.size() > static_cast<std::size_t>(t) ? t : path.size() - 1;
            if (conflictsFound) {
                if (path[time] == conflictNode) {
                    points[conflictNode].insert(agent);
                }
            }
            else {
                if (points.contains(path[time])) {
                    conflictsFound = true;
                    conflictNode = path[time];
                    points[path[time]].insert(agent);
                } 
                else {
                    points[path[time]] = std::unordered_set<Agent, hashAgent>({ agent });
                }
            }
        }
        if (conflictsFound) {
            return std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int>(true, points[conflictNode], conflictNode, t);
        }
    }
    return std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int>(false, std::unordered_set<Agent, hashAgent>(), BaseNode(0, 0), 0);
}

std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int> HighLevelNode::findEdgeConflicts() {
    auto paths = _solution.getSolutions();

    int maxLen = 0;
    for (auto& [agent, path] : paths) {
        if (path.size() > static_cast<std::size_t>(maxLen)) {
            maxLen = path.size();
        }
    }

    for (int t = 1; t < maxLen; t++) {
        std::unordered_map<std::tuple<BaseNode, BaseNode>, Agent, hashBaseNode_x2> edges;
        for (auto& [agent, path] : paths) {
            if (static_cast<std::size_t>(t) < path.size()) {
                if (edges.contains(std::tuple<BaseNode, BaseNode>(path[t], path[t - 1]))) {
                    return std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int>(
                        true, path[t - 1], path[t],
                        agent, edges[std::tuple<BaseNode, BaseNode>(path[t], path[t - 1])],
                        t - 1, t
                    );
                } 
                else {
                    edges[std::tuple<BaseNode, BaseNode>(path[t - 1], path[t])] = agent;
                }
            }
        }
    }

    return std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int>(
        false, BaseNode(0, 0), BaseNode(0, 0), Agent(), Agent(), 0, 0
    );
}

bool operator<(HighLevelNode const& first, HighLevelNode const& second) {
    return first.getCost() < second.getCost();
}

std::size_t hashHighLevelNode::operator()(HighLevelNode const& s) const {
    return hashConstraints{}(s._constraints);
}

bool InversedComparator_HL::operator()(HighLevelNode const& first, HighLevelNode const& second) const { 
    return first.getCost() >= second.getCost();
}

bool operator==(HighLevelNode const& first, HighLevelNode const& second) {
    return first._constraints == second._constraints;
}


// CONFLICT TREE
ConflictTree::ConflictTree() {
    _open = std::vector<HighLevelNode>();
    _closed = std::unordered_set<HighLevelNode, hashHighLevelNode>();
}
    
size_t ConflictTree::getSize() {
    return _open.size() + _closed.size();
}

bool ConflictTree::openIsEmpty() {
    return _open.size() == 0;
}

void ConflictTree::addToOpen(HighLevelNode const& item) {
    _open.push_back(item);
    std::push_heap<std::vector<HighLevelNode>::iterator, InversedComparator_HL>(_open.begin(), _open.end(), InversedComparator_HL());
}

HighLevelNode ConflictTree::getBestNodeFromOpen() {
    std::pop_heap<std::vector<HighLevelNode>::iterator, InversedComparator_HL>(_open.begin(), _open.end(), InversedComparator_HL());
    auto bestNode = _open[_open.size() - 1];
    _open.pop_back();
    while (wasExpanded(bestNode) and _open.size() > 0) {
        std::pop_heap<std::vector<HighLevelNode>::iterator, InversedComparator_HL>(_open.begin(), _open.end(), InversedComparator_HL());
        bestNode = _open[_open.size() - 1];
        _open.pop_back(); 
    }
    return bestNode;
}
    
void ConflictTree::addToClosed(HighLevelNode& item) {
    _closed.insert(item);
}

bool ConflictTree::wasExpanded(HighLevelNode& item) {
    return _closed.contains(item);
}

std::vector<HighLevelNode> ConflictTree::OPEN() {
    return _open;
}

std::unordered_set<HighLevelNode, hashHighLevelNode> ConflictTree::CLOSED() {
    return _closed;
}