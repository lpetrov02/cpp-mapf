#include <fstream>

#include "astarStructs.h"


int computeCost(int i1, int j1, int i2, int j2) {
    return std::abs(i1 - i2) + std::abs(j1 - j2);
}


std::tuple<int, int, std::string> getMap(std::string filename) {
    std::ifstream input;
    input.open(filename);

    int w, h;
    std::string res = "", tmp;

    input >> w >> h;
    getline(input, tmp);
    while (getline(input, tmp)) {
        std::cout << tmp << std::endl;
        res = res + tmp + "\n";
    }

    input.close();
    return std::tuple<int, int, std::string>(w, h, res);
}


std::tuple<bool, Node, int> astar(Map gridMap, Agent agent, std::function<int(int, int, int, int)> heuristicFunc = manhattan) {
    auto ast = SearchTree();
    int steps = 0, nodesCreated = 0;
    int current_i, current_j;
    
    BaseNode currentPoint = agent.getStart();
    std::tie(current_i, current_j) = currentPoint.getTuple();
    Node currentNode = Node(current_i, current_j);
    nodesCreated += 1;
    ast.addToOpen(currentNode);
    // latest_constraint = constraints.get_latest_constraint(agent)

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
                point.first, point.second, currentNode._g + computeCost(point.first, point.second, currentNode._i, currentNode._j), 
                heuristicFunc(agent.getGoal()._i, agent.getGoal()._j, point.first, point.second), &currentNode);
            auto newPoint = BaseNode(newNode._i, newNode._j);
            if (!ast.wasExpanded(newNode) /*&& constraints.is_allowed(agent, new_node.time, current_base_node, new_base_node)*/) {  
                if (newPoint == agent.getGoal() /*&& newNode.time > latest_constraint*/) {
                    return std::tuple<bool, Node, int>(true, newNode, steps);
                }
                ast.addToOpen(newNode);
            }
        }
    }
        
    return std::tuple<bool, Node, int>(false, Node(-1, -1), steps);
}


int main() {
    auto mapInfo = getMap("./samples/map0.txt");

    Map map = Map();
    map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

    Agent agent = Agent(0, std::pair<int, int>(0, 0), std::pair<int, int>(0, 2));

    auto astarRes = astar(map, agent);
    std::cout << std::get<0>(astarRes) << std::endl;
    return 0;
}