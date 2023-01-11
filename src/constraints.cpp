#include "constraints.h"


// BASE CLASS CONSTRAINTS

std::size_t Constraints::getHash() const { return 0; }
std::string Constraints::getHashStr() const { return ""; }

bool Constraints::isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) {
    std::ignore = agent;
    std::ignore = step;
    std::ignore = nodeFrom;
    std::ignore = nodeTo;
    return true;
}

int Constraints::getLatestConstraint(Agent agent) { std::ignore = agent; return 0; }


std::size_t hashConstraints::operator()(Constraints const& constraints) const {
    return constraints.getHash();
}

bool operator==(Constraints const& first, Constraints const& second) {
    return first.getHashStr() == second.getHashStr();
}


std::size_t hash_BaseNode_x2_Int::operator()(std::tuple<BaseNode, BaseNode, int> const& s) const {
    return hashBaseNode{}(std::get<0>(s)) ^ (hashBaseNode{}(std::get<1>(s)) << 1) ^ (std::hash<int>{}(std::get<2>(s)));
}


// POSITIVE CONSTRAINTS FOR DISJOINT SPLITTING CBS
PositiveConstraints::PositiveConstraints() {
    _hashStr = "";
    _hash = std::hash<std::string>{}(_hashStr);
    _constraints = std::unordered_map<std::tuple<BaseNode, int>, std::unordered_set<Agent, hashAgent>, hashBaseNodeInt>();
    _agentConstraints = std::unordered_map<std::tuple<Agent, int>, BaseNode, hashAgentInt>();
    _latestConflicts1 = std::unordered_map<Agent, int, hashAgent>();
    _latestConflicts2 = std::unordered_map<BaseNode, int, hashBaseNode>();
}

void PositiveConstraints::updateHash(Agent agent, int step, BaseNode baseNode) {
    _hashStr += "{" + std::to_string(agent.getIndex()) + " " + std::to_string(baseNode._i) + " " +
                 std::to_string(baseNode._j) + " " + std::to_string(step) + "}";
    _hash = std::hash<std::string>{}(_hashStr);
}

std::size_t PositiveConstraints::getHash() const {
    return _hash;
}

std::string PositiveConstraints::getHashStr() const {
    return _hashStr;
}

void PositiveConstraints::addConstraint(Agent agent, int step, BaseNode baseNode) {
    if (_constraints.contains(std::tuple<BaseNode, int>(baseNode, step))) {
        _constraints[std::tuple<BaseNode, int>(baseNode, step)] = std::unordered_set<Agent, hashAgent>();
    }
    _constraints[std::tuple<BaseNode, int>(baseNode, step)].insert(agent);

    _agentConstraints[std::tuple<Agent, int>(agent, step)] = baseNode;

    if (baseNode != agent.getGoal()) {
        if (!_latestConflicts1.contains(agent)) {
            _latestConflicts1[agent] = -1;
        }
        _latestConflicts1[agent] = std::max(step, _latestConflicts1[agent]);
        if (!_latestConflicts2.contains(baseNode)) {
            _latestConflicts2[baseNode] = -1;
        }
        _latestConflicts2[baseNode] = std::max(step, _latestConflicts2[baseNode]);
    }

    updateHash(agent, step, baseNode);
}

bool PositiveConstraints::isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) {
    bool noOneElseHasToBeHere =
            !_constraints.contains(std::tuple<BaseNode, int>(nodeTo, step)) ||
            (_constraints[std::tuple<BaseNode, int>(nodeTo, step)].contains(agent) &&
            (_constraints[std::tuple<BaseNode, int>(nodeTo, step)].size() == 1));
    bool thisAgentDoesNotHaveToBeSomewhereElse =
            !_agentConstraints.contains(std::tuple<Agent, int>(agent, step)) ||
            _agentConstraints[std::tuple<Agent, int>(agent, step)] == nodeTo;

    bool noVertexConflicts = noOneElseHasToBeHere && thisAgentDoesNotHaveToBeSomewhereElse;
    bool noEdgeConflicts = 
            !_constraints.contains(std::tuple<BaseNode, int>(nodeTo, step - 1)) ||
            !_constraints.contains(std::tuple<BaseNode, int>(nodeFrom, step)) ||
            std::count_if(
                _constraints[std::tuple<BaseNode, int>(nodeTo, step - 1)].begin(),
                _constraints[std::tuple<BaseNode, int>(nodeTo, step - 1)].end(),
                [&](Agent a) {
                    if (!_constraints.contains(std::tuple<BaseNode, int>(nodeFrom, step))) {
                        return false;
                    }
                    return _constraints[std::tuple<BaseNode, int>(nodeFrom, step)].contains(a);
                }
            ) == 0;

    return noVertexConflicts && noEdgeConflicts;
}

int PositiveConstraints::getLatestConstraint(Agent agent) {
    int maxStep1 = 0, maxStep2 = 0;
    if (_latestConflicts1.contains(agent)) {
        maxStep1 = _latestConflicts1[agent];
    }
    if (_latestConflicts2.contains(agent.getGoal())) {
        maxStep2 = _latestConflicts2[agent.getGoal()];
    }
    return std::max(maxStep1, maxStep2);
}


// NEGATIVE CONSTRAINTS FOR DISJOINT SPLITTING CBS
NegativeConstraints::NegativeConstraints() {
    _hashStr = "";
    _hash = std::hash<std::string>{}(_hashStr);
    _vertexConstraints = std::unordered_map<std::tuple<BaseNode, int>, std::unordered_set<Agent, hashAgent>, hashBaseNodeInt>();
    _edgeConstraints = std::unordered_map<std::tuple<BaseNode, BaseNode, int>, std::unordered_set<Agent, hashAgent>, hash_BaseNode_x2_Int>();
    _latestConflicts = std::unordered_map<Agent, int, hashAgent>();
}

void NegativeConstraints::updateHashVertex(Agent agent, int step, BaseNode baseNode) {
    _hashStr += "{" + std::to_string(agent.getIndex()) + " " + std::to_string(baseNode._i) + " " +
                 std::to_string(baseNode._j) + " " + std::to_string(step) + "}";
    _hash = std::hash<std::string>{}(_hashStr);
}

void NegativeConstraints::updateHashEdge(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) {
    _hashStr += "{" + std::to_string(agent.getIndex()) + 
                " (" + std::to_string(nodeFrom._i) + " " + std::to_string(nodeFrom._j) + ") -/-> (" + 
                std::to_string(nodeTo._i) + " " + std::to_string(nodeTo._j) + ") " + std::to_string(step) + "}";
    _hash = std::hash<std::string>{}(_hashStr);
}

std::size_t NegativeConstraints::getHash() const {
    return _hash;
}

std::string NegativeConstraints::getHashStr() const {
    return _hashStr;
}

void NegativeConstraints::addVertexConstraint(Agent agent, int step, BaseNode baseNode) {
    if (!_vertexConstraints.contains(std::tuple<BaseNode, int>(baseNode, step))) {
        _vertexConstraints[std::tuple<BaseNode, int>(baseNode, step)] = std::unordered_set<Agent, hashAgent>();
    }

    if (!_vertexConstraints[std::tuple<BaseNode, int>(baseNode, step)].contains(agent)) {
        _vertexConstraints[std::tuple<BaseNode, int>(baseNode, step)].insert(agent);
        updateHashVertex(agent, step, baseNode);
    }

    if (baseNode == agent.getGoal()) {
        if (!_latestConflicts.contains(agent)) {
            _latestConflicts[agent] = -1;
        }
        _latestConflicts[agent] = std::max(step, _latestConflicts[agent]);
    }

}

void NegativeConstraints::addEdgeConstraint(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) {
    // Edge: nodeFrom --> nodeTo, agent at nodeTo at time step
    if (!_edgeConstraints.contains(std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step))) {
        _edgeConstraints[std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step)] = std::unordered_set<Agent, hashAgent>();
    }

    if (!_edgeConstraints[std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step)].contains(agent)) {
        _edgeConstraints[std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step)].insert(agent);
        updateHashEdge(agent, step, nodeFrom, nodeTo);
    }
}

bool NegativeConstraints::isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) {
    bool noVertexConflicts = !_vertexConstraints.contains(std::tuple<BaseNode, int>(nodeTo, step)) ||
                            !_vertexConstraints[std::tuple<BaseNode, int>(nodeTo, step)].contains(agent);
    bool noEdgeConflicts = !_edgeConstraints.contains(std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step)) ||
                            !_edgeConstraints[std::tuple<BaseNode, BaseNode, int>(nodeFrom, nodeTo, step)].contains(agent);
    return noVertexConflicts && noEdgeConflicts;
}

int NegativeConstraints::getLatestConstraint(Agent agent) {
    if (_latestConflicts.contains(agent)) {
        return _latestConflicts[agent];
    }
    return 0;
}