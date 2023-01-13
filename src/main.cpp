#include <chrono>
#include <fstream>

#include "astar.h"
#include "forTesting.h"
#include "cbsStructs.h"
#include "cbsDsStructs.h"
#include "timeException.h"


std::tuple<bool, double, int, Solution> cbs(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::size_t maxTime = 30,
    std::function<int(int, int, int, int)> heuristicFunc = manhattan
) {
    std::chrono::steady_clock sc;
    auto start = sc.now();
    int astarCount = 0;
    int nextBound = 100;
    int hlNodes = 1;

    int steps = 0;
    ConflictTree ct;
    HighLevelNode currentNode;

    std::vector<Agent> agents;
    for (std::size_t i = 0; i < starts.size(); i++) {
        agents.push_back(Agent(i, BaseNode(starts[i].first, starts[i].second), BaseNode(goals[i].first, goals[i].second)));
    }

    for (auto& agent : agents) {
        AstarRes res = astar(gridMap, agent, currentNode.getConstraints(), heuristicFunc);
        astarCount += 1;
        if (!res._found) {
            return std::tuple<bool, double, int, Solution>(
                false, 0.0, 0, Solution()
            );
        }
        currentNode.resolve(agent, res._path);
    }

    ct.addToOpen(currentNode);

    while (!ct.openIsEmpty()) {
        currentNode = ct.getBestNodeFromOpen();
        if (ct.wasExpanded(currentNode)) {
            break;
        }

        steps += 1;
        ct.addToClosed(currentNode);
        
        auto vertexInfo = currentNode.findVertexConflicts();
        auto edgeInfo = currentNode.findEdgeConflicts();

        // std::cout << std::get<0>(vertexInfo) << " " << std::get<2>(vertexInfo) << std::endl;
        // std::cout << std::get<0>(edgeInfo) << " " << std::get<1>(edgeInfo) << " " << std::get<2>(edgeInfo) << std::endl;

        if (!std::get<0>(vertexInfo) && !std::get<0>(edgeInfo)) {
            return std::tuple<bool, double, int, Solution>(
                true, static_cast<std::chrono::duration<double>>(sc.now() - start).count(), hlNodes, currentNode.getSolution()
            );
        }
        if (!std::get<0>(edgeInfo) || (std::get<0>(vertexInfo) && (std::get<3>(vertexInfo)) <= std::get<5>(edgeInfo))) {
            for (auto& agent : std::get<1>(vertexInfo)) {
                auto newNode = currentNode;
                bool hasSolution = true;

                for (auto& agent2 : std::get<1>(vertexInfo)) {
                    if (!(agent2 == agent)) {
                        newNode.getConstraints()->addVertexConstraint(agent2, std::get<3>(vertexInfo), std::get<2>(vertexInfo));
                    }
                }
                for (auto& agent2 : std::get<1>(vertexInfo)) {
                    if (!(agent2 == agent)) {
                        AstarRes res = astar(gridMap, agent2, newNode.getConstraints(), heuristicFunc);
                        astarCount += 1;
                        if (!res._found) {
                            hasSolution = false;
                            break;
                        }
                        newNode.resolve(agent2, res._path);
                    }
                }
                if (hasSolution) {
                    hlNodes += 1;
                    ct.addToOpen(newNode);
                }
            }
        }
        else {
            auto newNode1 = currentNode, newNode2 = currentNode;
            newNode1.getConstraints()->addEdgeConstraint(
                std::get<3>(edgeInfo), std::get<6>(edgeInfo), std::get<1>(edgeInfo), std::get<2>(edgeInfo)
            );
            AstarRes res1 = astar(gridMap, std::get<3>(edgeInfo), newNode1.getConstraints(), heuristicFunc);
            astarCount += 1;
            if (res1._found) {
                newNode1.resolve(std::get<3>(edgeInfo), res1._path);
                hlNodes += 1;
                ct.addToOpen(newNode1);
            }

            newNode2.getConstraints()->addEdgeConstraint(
                std::get<4>(edgeInfo), std::get<6>(edgeInfo), std::get<2>(edgeInfo), std::get<1>(edgeInfo)
            );
            AstarRes res2 = astar(gridMap, std::get<4>(edgeInfo), newNode2.getConstraints(), heuristicFunc);
            astarCount += 1;
            if (res2._found) {
                newNode2.resolve(std::get<4>(edgeInfo), res2._path);
                hlNodes += 1;
                ct.addToOpen(newNode2);
            }
        }
        if (astarCount > nextBound) {
            if (static_cast<std::chrono::duration<double>>(sc.now() - start).count() > maxTime) {
                throw TimeException("time is over");
            }
            nextBound += 100;
        }
    }

    return std::tuple<bool, double, int, Solution>(false, 0.0, 0, Solution());
}


std::tuple<bool, double, int, Solution> cbsDS(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::size_t maxTime = 30, 
    std::function<int(int, int, int, int)> heuristicFunc = manhattan
) {
    std::chrono::steady_clock sc;
    auto start = sc.now();
    int astarCount = 0;
    int nextBound = 100;
    int hlNodes = 1;

    int steps = 0;
    ConflictTree_DS ct;
    HighLevelNode_DS currentNode;

    std::vector<Agent> agents;
    for (std::size_t i = 0; i < starts.size(); i++) {
        agents.push_back(Agent(i, BaseNode(starts[i].first, starts[i].second), BaseNode(goals[i].first, goals[i].second)));
    }

    for (auto& agent : agents) {
        AstarRes res = astar(gridMap, agent, currentNode.getConstraints(), heuristicFunc);
        astarCount += 1;
        if (!res._found) {
            return std::tuple<bool, double, int, Solution>(false, 0.0, 0, Solution());
        }
        currentNode.resolve(agent, res._path);
    }

    ct.addToOpen(currentNode);

    while (!ct.openIsEmpty()) {
        currentNode = ct.getBestNodeFromOpen();
        if (ct.wasExpanded(currentNode)) {
            break;
        }

        steps += 1;
        ct.addToClosed(currentNode);
        
        auto vertexInfo = currentNode.findVertexConflicts();
        auto edgeInfo = currentNode.findEdgeConflicts();

        // std::cout << "Conflicts info:" << std::endl;
        // std::cout << std::get<0>(vertexInfo) << ": vertex " << std::get<2>(vertexInfo) << ", step " << std::get<3>(vertexInfo) << std::endl;
        // std::cout << std::get<0>(edgeInfo) << ": from " << std::get<1>(edgeInfo) << " to " << std::get<2>(edgeInfo) << ", steps " << std::get<5>(edgeInfo) << "->" << std::get<6>(edgeInfo) << std::endl;
        // std::cout << "CurrentNode.cost: " << currentNode.getCost() << "\n" << std::endl;

        if (!std::get<0>(vertexInfo) && !std::get<0>(edgeInfo)) {
            return std::tuple<bool, double, int, Solution>(
                true, static_cast<std::chrono::duration<double>>(sc.now() - start).count(), hlNodes, currentNode.getSolution()
            );
        }
        if (!std::get<0>(edgeInfo) || (std::get<0>(vertexInfo) && (std::get<3>(vertexInfo)) <= std::get<5>(edgeInfo))) {
            for (auto& agent : std::get<1>(vertexInfo)) {
                auto newNode = currentNode;
                bool hasSolution = true;

                newNode.getConstraints()->addVertexConstraint(agent, std::get<3>(vertexInfo), std::get<2>(vertexInfo));
                
                for (auto& agent2 : std::get<1>(vertexInfo)) {
                    if (!(agent2 == agent)) {
                        AstarRes res = astar(gridMap, agent2, newNode.getConstraints(), heuristicFunc);
                        astarCount += 1;
                        if (!res._found) {
                            hasSolution = false;
                            break;
                        }
                        newNode.resolve(agent2, res._path);
                    }
                }
                if (hasSolution) {
                    hlNodes += 1;
                    ct.addToOpen(newNode);
                }
            }
        }
        else {
            auto newNode1 = currentNode, newNode2 = currentNode;
            newNode1.getConstraints()->addEdgeConstraint(
                std::get<3>(edgeInfo), std::get<6>(edgeInfo), std::get<1>(edgeInfo), std::get<2>(edgeInfo)
            );
            AstarRes res1 = astar(gridMap, std::get<4>(edgeInfo), newNode1.getConstraints(), heuristicFunc);
            astarCount += 1;
            if (res1._found) {
                newNode1.resolve(std::get<4>(edgeInfo), res1._path);
                hlNodes += 1;
                ct.addToOpen(newNode1);
            }

            newNode2.getConstraints()->addEdgeConstraint(
                std::get<4>(edgeInfo), std::get<6>(edgeInfo), std::get<2>(edgeInfo), std::get<1>(edgeInfo)
            );
            AstarRes res2 = astar(gridMap, std::get<3>(edgeInfo), newNode2.getConstraints(), heuristicFunc);
            astarCount += 1;
            if (res2._found) {
                newNode2.resolve(std::get<3>(edgeInfo), res2._path);
                hlNodes += 1;
                ct.addToOpen(newNode2);
            }
        }

        if (astarCount > nextBound) {
            if (static_cast<std::chrono::duration<double>>(sc.now() - start).count() > maxTime) {
                throw TimeException("time is over");
            }
            nextBound += 100;
        }
    }

    return std::tuple<bool, double, int, Solution>(false, 0.0, 0, Solution());
}


int main() {
    auto mapInfo = getMap("./samples/warehouse.txt");

    Map map = Map();
    map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

    for (int n = 4; n < 13; n++) {
        std::string filename1 = "./tests/" + std::to_string(n) + ".txt";
        std::string filename2 = "./results/ds_" + std::to_string(n) + ".txt";
        std::ifstream input(filename1);
        std::ofstream output(filename2);

        int x1, y1, x2, y2;
        std::vector<std::pair<int, int>> starts, goals;

        std::size_t maxTime = n > 8 ? 30 : 60;
        for (int i = 0; i < 100; i++) {
            starts.clear();
            goals.clear();
            std::cout << n << " agents, test #" << i << std::endl;
            input >> x1;
            for (int j = 0; j < n; j++) {
                input >> y1 >> x1 >> y2 >> x2;
                starts.push_back(std::pair(x1, y1));
                goals.push_back(std::pair(x2, y2));
            }
            std::tuple<bool, double, int, Solution> res;
            try {
                res = cbsDS(map, starts, goals, maxTime);
                if (std::get<0>(res)) {
                    output << std::get<1>(res) << " " << std::get<2>(res) << "\n";
                } else {
                    output << "0 0" << "\n";
                }
            } catch (TimeException& e) {
                std::cout << e.what() << std::endl;
                output << std::to_string(maxTime) << " 0" << "\n";
            }
        }
    }

    return 0;
}


// int main() {
//     auto mapInfo = getMap("./samples/warehouse.txt");

//     Map map = Map();
//     map.readFromString(std::get<2>(mapInfo), std::get<0>(mapInfo), std::get<1>(mapInfo));

//     // Agent agent = Agent(0, std::pair<int, int>(455, 437), std::pair<int, int>(47, 119));

//     // PositiveConstraints constraints;
//     // constraints.addConstraint(agent, 25, BaseNode(9, 9));

//     // NegativeConstraints negConstraints;
//     // negConstraints.addEdgeConstraint(agent, 1, BaseNode(0, 0), BaseNode(1, 0));
//     // negConstraints.addVertexConstraint(agent, 3, BaseNode(2, 0));

//     // auto astarRes = astar(map, agent, &constraints);

//     // std::vector<std::pair<int, int>> starts = { {7, 1}, {9, 1}, {8, 0}, {8, 2}, {7, 0}, {9, 2}, {7, 2}, {9, 0} };
//     // std::vector<std::pair<int, int>> goals = { {9, 1}, {7, 1}, {8, 2}, {8, 0}, {9, 2}, {7, 0}, {9, 0}, {7, 2} };

//     std::vector<std::pair<int, int>> starts = { {20, 3}, {11, 3}, {28, 4}, {26, 0} };
//     std::vector<std::pair<int, int>> goals = { {8, 6}, {16, 9}, {2, 7}, {25, 1} };

//     // std::vector<std::pair<int, int>> starts = { {10, 131} };
//     // std::vector<std::pair<int, int>> goals = { {470, 90} };

//     auto res = cbs(map, starts, goals);
//     std::cout << (std::get<0>(res) ? "Found!" : "Not found") << std::endl;
//     if (std::get<0>(res)) {
//         writeCbsSolution(std::get<3>(res), "./results/res202.txt");
//     }

//     // std::cout << (astarRes._found ? "Found!" : "Not found") << std::endl;
//     // if (astarRes._found) {
//     //     writePathToFile(astarRes._path, "./results/res202.txt");
//     // }
    
//     return 0;
// }
