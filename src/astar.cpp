#include <fstream>

#include "astarStructs.h"


struct AstarRes {
public:
    AstarRes(bool found, Node goal, std::vector<std::pair<int, int>> path, int steps) 
    : _found(found), _goal{goal}, _steps(steps), _path(path) {}

public:
    bool _found;
    Node _goal;
    int _steps;
    std::vector<std::pair<int, int>> _path;
};


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


void writePathToFile(std::vector<std::pair<int, int>> path, std::string filename) {
    std::ofstream output;
    output.open(filename);

    for (auto it = path.rbegin(); it != path.rend(); it++) {
        output << it->first << " " << it->second << "\n";
    }

    output.close();
}


AstarRes astar(Map gridMap, Agent agent, std::function<int(int, int, int, int)> heuristicFunc = manhattan) {
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
                heuristicFunc(agent.getGoal()._i, agent.getGoal()._j, point.first, point.second), currentNode.getTuple());
            auto newPoint = BaseNode(newNode._i, newNode._j);
            if (!ast.wasExpanded(newNode) /*&& constraints.is_allowed(agent, new_node.time, current_base_node, new_base_node)*/) {  
                if (newPoint == agent.getGoal() /*&& newNode.time > latest_constraint*/) {
                    Node curNode = newNode;
                    std::vector<std::pair<int, int>> res;

                    while (ast.CLOSED().contains(curNode.getParentTuple())) {
                        res.push_back(std::pair<int, int>(curNode._i, curNode._j));
                        curNode = ast.CLOSED().find(curNode.getParentTuple())->second;
                    }
                    res.push_back(std::pair<int, int>(curNode._i, curNode._j));

                    return AstarRes(true, newNode, res, steps);
                }
                ast.addToOpen(newNode);
            }
        }
    }
        
    return AstarRes(false, Node(-1, -1), std::vector<std::pair<int, int>>(), steps);
}


int main() {
    auto mapInfo = getMap("./samples/map0.txt");

    Map map = Map();
    map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

    Agent agent = Agent(0, std::pair<int, int>(0, 0), std::pair<int, int>(0, 2));

    auto astarRes = astar(map, agent);
    std::cout << (astarRes._found ? "Found!" : "Not found") << std::endl;

    writePathToFile(astarRes._path, "./results/res0.txt");
    return 0;
}