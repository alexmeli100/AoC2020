#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <range/v3/view/generate.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/transform.hpp>
#include <algorithm>


int countTrees(const std::vector<std::string> &map, const std::pair<int, int> &slopes) {
    auto width = map[0].length();
    auto height = map.size();

    auto indices = [&, i = std::make_pair(0, 0)]() mutable -> std::pair<int, int> {
        auto x = (i.first + slopes.first) % width;
        auto y = i.second + slopes.second;
        i = {x, y};

        return i;
    };

    auto validIndice = [&](auto const &p){ return p.second < height; };
    auto toSpot = [&](auto const &p) { return map[p.second][p.first]; };

    auto res = ranges::views::generate(indices)
            | ranges::views::take_while(validIndice)
            | ranges::views::transform(toSpot);


    
    return std::ranges::count_if(res, [](const auto &c) { return c == '#'; } );
}

auto countTotalTrees(const std::vector<std::string> &map, const std::vector<std::pair<int, int>> &slopes) {
    auto res = slopes | ranges::views::transform([&](const auto &p) {
        auto res = countTrees(map, p);
        return res;
    });

    return ranges::accumulate(res, 1ull, std::multiplies<>());
}

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<std::string> map;
    std::string line;

    while(std::getline(input, line)) {
        map.push_back(line);
    }

    std::vector<std::pair<int, int>> slopes { {1, 1}, { 3, 1}, {5, 1}, {7, 1}, {1, 2} };

    auto part1 = countTrees(map, {3, 1});
    auto part2 = countTotalTrees(map, slopes);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
