#include "constraints.h"


// BASE CLASS CONSTRAINTS

void Constraints::updateHash(Agent agent, int step, BaseNode baseNode) {
    std::ignore = agent;
    std::ignore = step;
    std::ignore = baseNode;
}

std::size_t Constraints::getHash() const { return 0; }

void Constraints::addConstraint(Agent agent, int step, BaseNode baseNode) {
    std::ignore = agent;
    std::ignore = step;
    std::ignore = baseNode;
}

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
