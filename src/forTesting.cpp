#include "forTesting.h"


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


void writePathToFile(std::vector<BaseNode> path, std::string filename) {
    std::ofstream output;
    output.open(filename);

    for (auto it = path.rbegin(); it != path.rend(); it++) {
        output << *it << "\n";
    }

    output.close();
}