#include <iostream>
#include <range/v3/view/slice.hpp>
#include <range/v3/view/generate_n.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/take_last.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <vector>

int game(const std::vector<int> &seq, int n) {
    std::vector<int> seen(40000000);

    auto prev = seq | ranges::views::slice(size_t(0), seq.size() - 1) | ranges::views::enumerate;
    ranges::for_each(prev, [&](auto &&p) { seen[p.second] = p.first + 1; });

    auto sequence = [&, curr = seq.back(), idx = int(seq.size())]() mutable -> int {
        int last = std::exchange(seen[curr], idx);
        curr = last == 0 ? 0 : idx - last;
        idx++;

        return curr;
    };

    auto res = ranges::views::generate_n(sequence, n - seq.size()) | ranges::views::take_last(1);

    return *res.begin();
}

int main() {
    std::vector<int> input{ 10, 16, 6, 0, 1, 17 };
    auto part1 = game(input, 2020);
    auto part2 = game(input, 30000000);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
