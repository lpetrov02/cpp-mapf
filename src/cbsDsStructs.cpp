#include "cbsDsStructs.h"


// HIGH LEVEL NODE
HighLevelNode_DS::HighLevelNode_DS() {
    _constraints = PositiveConstraints();
    _solution = Solution();
}

void HighLevelNode_DS::resolve(Agent agent, std::vector<BaseNode> path) {
    std::reverse(path.begin(), path.end());
    _solution.addAgentPath(agent, std::move(path));
}

int HighLevelNode_DS::getCost() const {
    return _solution.getCost();
}

Solution HighLevelNode_DS::getSolution() {
    return _solution;
}

PositiveConstraints* HighLevelNode_DS::getConstraints() {
    return &_constraints;
}

std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int> HighLevelNode_DS::findVertexConflicts() {
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

std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int> HighLevelNode_DS::findEdgeConflicts() {
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

bool operator<(HighLevelNode_DS const& first, HighLevelNode_DS const& second) {
    return first.getCost() < second.getCost();
}

std::size_t hashHighLevelNode_DS::operator()(HighLevelNode_DS const& s) const {
    return hashConstraints{}(s._constraints);
}

bool InversedComparator_HLDS::operator()(HighLevelNode_DS const& first, HighLevelNode_DS const& second) const { 
    return first.getCost() >= second.getCost();
}

bool operator==(HighLevelNode_DS const& first, HighLevelNode_DS const& second) {
    return first._constraints == second._constraints;
}


// CONFLICT TREE
ConflictTree_DS::ConflictTree_DS() {
    _open = std::vector<HighLevelNode_DS>();
    _closed = std::unordered_set<HighLevelNode_DS, hashHighLevelNode_DS>();
}
    
size_t ConflictTree_DS::getSize() {
    return _open.size() + _closed.size();
}

bool ConflictTree_DS::openIsEmpty() {
    return _open.size() == 0;
}

void ConflictTree_DS::addToOpen(HighLevelNode_DS const& item) {
    _open.push_back(item);
    std::push_heap<std::vector<HighLevelNode_DS>::iterator, InversedComparator_HLDS>(_open.begin(), _open.end(), InversedComparator_HLDS());
}

HighLevelNode_DS ConflictTree_DS::getBestNodeFromOpen() {
    std::pop_heap<std::vector<HighLevelNode_DS>::iterator, InversedComparator_HLDS>(_open.begin(), _open.end(), InversedComparator_HLDS());
    auto bestNode = _open[_open.size() - 1];
    _open.pop_back();
    while (wasExpanded(bestNode) and _open.size() > 0) {
        std::pop_heap<std::vector<HighLevelNode_DS>::iterator, InversedComparator_HLDS>(_open.begin(), _open.end(), InversedComparator_HLDS());
        bestNode = _open[_open.size() - 1];
        _open.pop_back(); 
    }
    return bestNode;
}
    
void ConflictTree_DS::addToClosed(HighLevelNode_DS& item) {
    _closed.insert(item);
}

bool ConflictTree_DS::wasExpanded(HighLevelNode_DS& item) {
    return _closed.contains(item);
}

std::vector<HighLevelNode_DS> ConflictTree_DS::OPEN() {
    return _open;
}

std::unordered_set<HighLevelNode_DS, hashHighLevelNode_DS> ConflictTree_DS::CLOSED() {
    return _closed;
}