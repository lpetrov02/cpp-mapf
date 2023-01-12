#pragma once

#include "astar.h"


class Solution {
public:
    Solution();
    void addAgentPath(Agent agent, std::vector<BaseNode> path);
    void updateCost();
    int getCost() const;
    std::unordered_map<Agent, std::vector<BaseNode>, hashAgent> getSolutions();

private:
    std::unordered_map<Agent, std::vector<BaseNode>, hashAgent> _solutions;
    int _cost;
};


class HighLevelNode {
    friend bool operator<(HighLevelNode const& first, HighLevelNode const& second);
    friend bool operator==(HighLevelNode const& first, HighLevelNode const& second);
    friend struct hashHighLevelNode;
public:
    HighLevelNode();
    void resolve(Agent agent, std::vector<BaseNode> path);
    int getCost() const;
    Solution getSolution();
    NegativeConstraints* getConstraints();
    std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int> findVertexConflicts();
    std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int> findEdgeConflicts();

private:
    NegativeConstraints _constraints;
    Solution _solution;
};

bool operator<(HighLevelNode const& first, HighLevelNode const& second);

struct hashHighLevelNode {
    typedef HighLevelNode argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct InversedComparator_HL {
    typedef HighLevelNode argument_type;
    typedef bool result_type;

    result_type operator()(argument_type const& f, argument_type const& s) const;
};

bool operator==(HighLevelNode const& first, HighLevelNode const& second);


class ConflictTree {
public:
    ConflictTree();
    size_t getSize();
    bool openIsEmpty();
    void addToOpen(HighLevelNode const& item);
    HighLevelNode getBestNodeFromOpen();    
    void addToClosed(HighLevelNode& item);
    bool wasExpanded(HighLevelNode& item);
    std::vector<HighLevelNode> OPEN();
    std::unordered_set<HighLevelNode, hashHighLevelNode> CLOSED();

private:
    std::vector<HighLevelNode> _open;
    std::unordered_set<HighLevelNode, hashHighLevelNode> _closed;
};
