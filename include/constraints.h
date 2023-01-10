#pragma once

#include "astarNodes.h"
#include "astarStructs.h"


class Constraints {        
    friend std::ostream& operator<<(std::ostream& os, Constraints constraints);
public:
    virtual void updateHash(Agent agent, int step, BaseNode baseNode);
    virtual std::size_t getHash() const;
    virtual void addConstraint(Agent agent, int step, BaseNode baseNode);
    virtual bool isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo);
    virtual int getLatestConstraint(Agent agent);
};


struct hashConstraints {
    typedef Constraints argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};


class PositiveConstraints : public Constraints {        
    friend std::ostream& operator<<(std::ostream& os, Constraints constraints);
public:
    PositiveConstraints();
    void updateHash(Agent agent, int step, BaseNode baseNode);
    std::size_t getHash() const;
    void addConstraint(Agent agent, int step, BaseNode baseNode) override;
    bool isAllowed(Agent agent, int step, BaseNode nodeFrom, BaseNode nodeTo) override;
    int getLatestConstraint(Agent agent) override;
    
private:
    std::string _hashStr;
    std::size_t _hash;
    std::unordered_map<std::tuple<BaseNode, int>, std::unordered_set<Agent, hashAgent>, hashBaseNodeInt> _constraints;
    std::unordered_map<std::tuple<Agent, int>, BaseNode, hashAgentInt> _agentConstraints;
    std::unordered_map<Agent, int, hashAgent> _latestConflicts1;
    std::unordered_map<BaseNode, int, hashBaseNode> _latestConflicts2;
};
