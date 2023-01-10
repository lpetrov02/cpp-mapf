#include "astar.h"


AstarRes::AstarRes(bool found, std::vector<BaseNode> path, int steps) 
: _found(found), _steps(steps), _path(path) {}


int computeCost(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}

int manhattan(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}


AstarRes astar(Map gridMap, Agent agent, Constraints* constraints, std::function<int(int, int, int, int)> heuristicFunc) {
    auto ast = SearchTree();
    int steps = 0, nodesCreated = 0;
    int current_i, current_j;
    
    BaseNode currentPoint = agent.getStart();
    std::tie(current_i, current_j) = currentPoint.getTuple();
    Node currentNode = Node(current_i, current_j);
    nodesCreated += 1;
    ast.addToOpen(currentNode);
    int latestConstraint = constraints->getLatestConstraint(agent);

    while (!ast.openIsEmpty()) {
        currentNode = ast.getBestNodeFromOpen();

        std::tie(current_i, current_j, std::ignore) = currentNode.getTuple();
        currentPoint = BaseNode(current_i, current_j);
        if (ast.wasExpanded(currentNode)) {
            break;
        }

        steps += 1;
        ast.addToClosed(currentNode);
        auto neighbors = gridMap.getNeighbors(current_i, current_j);
        for (auto& point : neighbors) {
            nodesCreated += 1;
            auto newNode = Node(
                point.first, point.second, 
                heuristicFunc(agent.getGoal()._i, agent.getGoal()._j, point.first, point.second), currentNode.getTuple());
            auto newPoint = BaseNode(newNode._i, newNode._j);
            if (!ast.wasExpanded(newNode) && constraints->isAllowed(agent, newNode._t, currentPoint, newPoint)) {  
                if (newPoint == agent.getGoal() && newNode._t > latestConstraint) {
                    Node curNode = newNode;
                    std::vector<BaseNode> res;

                    while (ast.CLOSED().contains(curNode.getParentTuple())) {
                        res.push_back(curNode.getBaseNode());
                        curNode = ast.CLOSED().find(curNode.getParentTuple())->second;
                    }
                    res.push_back(curNode.getBaseNode());

                    return AstarRes(true, res, steps);
                }
                ast.addToOpen(newNode);
            }
        }
    }
        
    return AstarRes(false, std::vector<BaseNode>(), steps);
}
