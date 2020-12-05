#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <regex>
#include <string>
#include <ranges>

int main() {
    std::ifstream input("../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<std::string> required = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };

    std::vector<std::regex> patterns = {
            std::regex{R"(byr:(19[2-9]\d|200[0-2]))"},
            std::regex{R"(iyr:(201\d|2020))"},
            std::regex{R"(eyr:(202\d|2030))"},
            std::regex{R"(hgt:((1[5-8][0-9]|19[0-3])cm|(59|6[0-9]|7[0-6])in))"},
            std::regex{R"(hcl:#[0-9a-f]{6}( |$))"},
            std::regex{R"(ecl:(amb|blu|brn|gry|grn|hzl|oth))"},
            std::regex{R"(pid:\d{9}( |$))"}
    };


    std::vector<std::string> passports;
    std::string line;
    std::string passport;

    while (std::getline(input, line)) {
        if (line.empty()) {
            passports.push_back(passport);
            passport = "";
        } else {
            passport += " " + line;
        }
    }

    // add the last entry
    passports.push_back(passport);

    auto pred1 = [&](const std::string &pass) {
        return std::ranges::all_of(required, [&](const std::string &s) {
            return pass.find(s) != std::string::npos;
        });
    };

    auto pred2 = [&](const std::string &pass) {
        std::smatch fields;

        auto valid = [&](std::regex &pat) {
            return std::regex_search(pass, fields, pat);
        };

        return std::ranges::all_of(patterns, valid);
    };

    auto part1 = std::ranges::count_if(passports, pred1);
    auto part2 = std::ranges::count_if(passports, pred2);

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
