#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <range/v3/view/map.hpp>
#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/minmax_element.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/algorithm//for_each.hpp>
#include <range/v3/view/slice.hpp>
#include <unordered_map>

void updatePreamble(std::unordered_map<int, int> &preamble, int i, int key) {
    --preamble[i];

    if (preamble[i] == 0)
        preamble.erase(i);

    ++preamble[key];
}

std::optional<int> detectNumber(const std::vector<int> &numbers, int end = 25) {
    std::unordered_map<int, int> preamble;
    auto f = numbers | ranges::views::slice(0, end);
    ranges::for_each(f, [&](int num) { ++preamble[num]; });
    int i = 0;

    auto isValid = [&](int num) {
        auto res = ranges::any_of(preamble | ranges::views::keys, [&](int n) {
            return preamble.contains(num - n);
        });

        return res;
    };

    auto invalid = [&](int num) {
        if (!isValid(num))
            return true;

        updatePreamble(preamble, numbers[i], num);
        i++;

        return false;
    };

    auto tail = numbers | ranges::views::slice(size_t(end), numbers.size());
    auto res =  ranges::find_if(tail, invalid);

    if (res == ranges::end(tail))
        return std::nullopt;

    return *res;
}

std::optional<std::pair<int, int>> findContigiousRange(std::vector<int> &numbers, int sum) {
    int currSum = numbers[0];
    int start = 0;

    for (int i = 1; i < numbers.size(); i++) {
        while (currSum > sum && start < i - 1) {
            currSum = currSum - numbers[start];
            start++;
        }

        if (currSum == sum) {
            return std::make_pair(start, i);
        }

        if (i < numbers.size())
            currSum = currSum + numbers[i];
    }

    return std::nullopt;
}

std::optional<int> findContigiousSum(std::vector<int> &numbers, int sum) {
    auto range = findContigiousRange(numbers, sum);

    if (!range.has_value())
        return std::nullopt;

    auto [first, last] = *range;
    auto contigiousNums = numbers| ranges::views::slice(first, last);
    auto [min, max] = ranges::minmax_element(contigiousNums);

    return *min + *max;
}

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<int> numbers { std::istream_iterator<int>{input}, {} };
    auto part1 = detectNumber(numbers, 25);

    if (part1) {
        std::cout << "Part1: " << *part1 << "\n";
        auto part2 = findContigiousSum(numbers, *part1);

        if (part2) {
            std::cout << "Part2: " << *part2 << "\n";
        } else {
            std::cout << "Part2: could not find contigious sum" << "\n";
        }
    }
    else
        std::cout << "Part1: could not find invalid number" << "\n";

    return 0;
}
