#include "astar.h"
#include "forTesting.h"


int main() {
    auto mapInfo = getMap("./samples/map0.txt");

    Map map = Map();
    map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

    Agent agent = Agent(0, std::pair<int, int>(0, 0), std::pair<int, int>(0, 2));

    PositiveConstraints constraints;
    constraints.addConstraint(agent, 25, BaseNode(9, 9));

    NegativeConstraints negConstraints;
    negConstraints.addEdgeConstraint(agent, 1, BaseNode(0, 0), BaseNode(1, 0));
    negConstraints.addVertexConstraint(agent, 3, BaseNode(2, 0));

    auto astarRes = astar(map, agent, &negConstraints);
    std::cout << (astarRes._found ? "Found!" : "Not found") << std::endl;

    writePathToFile(astarRes._path, "./results/res0.txt");
    return 0;
}