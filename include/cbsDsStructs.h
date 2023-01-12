#include "cbsStructs.h"


class HighLevelNode_DS {
    friend bool operator<(HighLevelNode_DS const& first, HighLevelNode_DS const& second);
    friend bool operator==(HighLevelNode_DS const& first, HighLevelNode_DS const& second);
    friend struct hashHighLevelNode_DS;
public:
    HighLevelNode_DS();
    void resolve(Agent agent, std::vector<BaseNode> path);
    int getCost() const;
    Solution getSolution();
    PositiveConstraints* getConstraints();
    std::tuple<bool, std::unordered_set<Agent, hashAgent>, BaseNode, int> findVertexConflicts();
    std::tuple<bool, BaseNode, BaseNode, Agent, Agent, int, int> findEdgeConflicts();

private:
    PositiveConstraints _constraints;
    Solution _solution;
};

bool operator<(HighLevelNode_DS const& first, HighLevelNode_DS const& second);

struct hashHighLevelNode_DS {
    typedef HighLevelNode_DS argument_type;
    typedef std::size_t result_type;

    result_type operator()(argument_type const& s) const;
};

struct InversedComparator_HLDS {
    typedef HighLevelNode_DS argument_type;
    typedef bool result_type;

    result_type operator()(argument_type const& f, argument_type const& s) const;
};

bool operator==(HighLevelNode_DS const& first, HighLevelNode_DS const& second);


class ConflictTree_DS {
public:
    ConflictTree_DS();
    size_t getSize();
    bool openIsEmpty();
    void addToOpen(HighLevelNode_DS const& item);
    HighLevelNode_DS getBestNodeFromOpen();    
    void addToClosed(HighLevelNode_DS& item);
    bool wasExpanded(HighLevelNode_DS& item);
    std::vector<HighLevelNode_DS> OPEN();
    std::unordered_set<HighLevelNode_DS, hashHighLevelNode_DS> CLOSED();

private:
    std::vector<HighLevelNode_DS> _open;
    std::unordered_set<HighLevelNode_DS, hashHighLevelNode_DS> _closed;
};