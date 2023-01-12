#include "cbsFamily.h"


int notMain() {
    auto mapInfo = getMap("./samples/map1.txt");

    Map map = Map();
    map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

    // Agent agent = Agent(0, std::pair<int, int>(0, 0), std::pair<int, int>(0, 2));

    // PositiveConstraints constraints;
    // constraints.addConstraint(agent, 25, BaseNode(9, 9));

    // NegativeConstraints negConstraints;
    // negConstraints.addEdgeConstraint(agent, 1, BaseNode(0, 0), BaseNode(1, 0));
    // negConstraints.addVertexConstraint(agent, 3, BaseNode(2, 0));

    // auto astarRes = astar(map, agent, &negConstraints);

    std::vector<std::pair<int, int>> starts = { {7, 1}, {9, 1}, {8, 0}, {8, 2}, {7, 0}, {9, 2}, {7, 2}, {9, 0} };
    std::vector<std::pair<int, int>> goals = { {9, 1}, {7, 1}, {8, 2}, {8, 0}, {9, 2}, {7, 0}, {9, 0}, {7, 2} };

    // std::vector<std::pair<int, int>> starts = { {1, 6}, {1, 10} };
    // std::vector<std::pair<int, int>> goals = { {1, 10}, {1, 6} };

    // std::vector<std::pair<int, int>> starts = { {7, 1}, {9, 1}, {8, 0}, {8, 2} };
    // std::vector<std::pair<int, int>> goals = { {9, 1}, {7, 1}, {8, 2}, {8, 0} };

    auto res = cbsDS(map, starts, goals);
    std::cout << (std::get<0>(res) ? "Found!" : "Not found") << std::endl;
    if (std::get<0>(res)) {
        writeCbsSolution(std::get<3>(res), "./results/res1.txt");
    }

    // writePathToFile(astarRes._path, "./results/res0.txt");
    return 0;
}