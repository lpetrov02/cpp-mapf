#pragma once

#include "astarNodes.h"
#include "astarStructs.h"


class Constraints {        
    friend std::ostream& operator<<(std::ostream& os, Constraints constraints);
public:
    Constraints();
    void updateHash(Agent agent, int step, BaseNode baseNode);
    void addConstraint(Agent agent, int step, BaseNode baseNode);
    bool isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo);
    int getLatestConstraint(Agent agent);
    
private:
    std::string _hashStr;
    std::unordered_map<std::tuple<BaseNode, int>, std::unordered_set<Agent, hashAgent>, hashBaseNodeInt> _constraints;
    std::unordered_map<std::tuple<Agent, int>, BaseNode, hashAgentInt> _agentConstraints;
    std::unordered_map<Agent, int, hashAgent> _latestConflicts1;
    std::unordered_map<BaseNode, int, hashBaseNode> _latestConflicts2;

public:
    int _hash;
};


struct hashConstraints {
    typedef Constraints argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};