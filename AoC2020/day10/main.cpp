#include <iostream>
#include <fstream>
#include <functional>
#include <range/v3/view/slice.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/sort.hpp>
#include <range/v3/view/zip.hpp>
#include <range/v3/range/conversion.hpp>

std::pair<int, uint64_t> solve(std::vector<int>& adapters) {
    adapters.push_back(0);
    ranges::sort(adapters);
    auto builtIn = adapters.back() + 3;
    adapters.push_back(builtIn);
    auto first= adapters| ranges::views::slice(1ull, adapters.size());
    auto diffs = ranges::views::zip(first, adapters);
    std::unordered_map<int, int> diffCount;
    std::unordered_map<int, uint64_t> counts{ {0, 1ull} };

    ranges::for_each(diffs, [&](auto&& d) {
        const auto& [x, y] = d;
        ++diffCount[x - y];
        counts[x] = counts[x - 3] + counts[x - 2] + counts[x - 1];
    });

    return {diffCount[3] * diffCount[1], counts[adapters.back()] };
}

int main() {
    std::ifstream file("../../input.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<int> adapters{ std::istream_iterator<int>{file}, {} };
    const auto& [part1, part2] = solve(adapters);

    std::cout << "Part1: " << part1 << "\n";
    std::cout << "Part2: " << part2 << "\n";
    return 0;
}
