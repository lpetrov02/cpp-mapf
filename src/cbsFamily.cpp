#include <chrono>

#include "cbsFamily.h"
#include "timeException.h"


std::tuple<bool, double, int, Solution> cbs(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::function<int(int, int, int, int)> heuristicFunc
) {
    std::chrono::steady_clock sc;
    auto start = sc.now();
    int astarCount = 0;
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
        if (static_cast<std::chrono::duration<double>>(sc.now() - start).count() > 300) {
            throw TimeException("time is over");
        }
    }

    return std::tuple<bool, double, int, Solution>(false, 0.0, 0, Solution());
}


std::tuple<bool, double, int, Solution> cbsDS(
    Map gridMap, 
    std::vector<std::pair<int, int>> starts,
    std::vector<std::pair<int, int>> goals,
    std::function<int(int, int, int, int)> heuristicFunc
) {
    std::chrono::steady_clock sc;
    auto start = sc.now();
    int astarCount = 0;
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
        if (static_cast<std::chrono::duration<double>>(sc.now() - start).count() > 300) {
            throw TimeException("time is over");
        }
    }

    return std::tuple<bool, double, int, Solution>(false, 0.0, 0, Solution());
}