#include <iostream>
#include <ranges>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include <string>

struct Policy {
    int low{}, high{};
    char letter{};
    std::string password;
};

Policy parsePoliciy(const std::string &line, std::regex &reg) {
    std::smatch fields;
    std::regex_match(line, fields, reg);
    Policy p;

    p.low = std::stoi(fields[1]);
    p.high = std::stoi(fields[2]);
    p.letter = fields[3].str()[0];
    p.password = fields[4];

    return p;
}

int countValidPolicies(const std::vector<Policy> &policies, std::function<bool(const Policy&)> pred) {

    return std::ranges::count_if(policies, std::move(pred));
}

int main() {
    std::ifstream input("../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::string line;
    std::vector<Policy> policies;
    std::regex pat { "([0-9]+)-([0-9]+)\\s+([a-z]):\\s+([a-z]+)" };

    while(std::getline(input, line)) {
        policies.push_back(parsePoliciy(line, pat));
    }

    auto pred1 = [](const Policy &p) {
        auto c = std::ranges::count(p.password, p.letter);

        return c >= p.low && c <= p.high;
    };

    auto pred2 = [](const Policy &p) {
        bool first = p.password[p.low - 1] == p.letter;
        bool snd = p.password[p.high - 1] == p.letter;

        return (first && !snd) || (!first && snd);
    };

    int part1 = countValidPolicies(policies, pred1);
    int part2 = countValidPolicies(policies, pred2);

    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << part2 << std::endl;
    return 0;
}
