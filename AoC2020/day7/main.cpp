#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/transform.hpp>
#include <regex>

using BagNode = std::pair<std::string, int>;

struct BagNodeHash {
    size_t operator() (const BagNode &n) const {
        return std::hash<std::string>()(n.first) ^ std::hash<int>()(n.second);
    }
};

using Children = std::unordered_set<BagNode, BagNodeHash>;



class Rules {
public:
    Rules(std::unordered_map<std::string, Children> graph, std::unordered_map<std::string, Children> revGraph):
        graph(std::move(graph)), reversedGraph(std::move(revGraph)) {}

    int getCount(const std::string& bag) {
        std::unordered_set<std::string> visited;
        return getCountIter(bag, visited) - 1;
    }

    int countInnerBags(const std::string& bag) {
        if (!graph.contains(bag)) {
            return 0;
        }

        auto &children = graph.at(bag);
        auto counts = children | ranges::views::transform([&](const auto &p) {
            return p.second + p.second * countInnerBags(p.first);
        });

        return ranges::accumulate(counts, 0, std::plus<>());
    }

private:
    int getCountIter(const std::string &curr, std::unordered_set<std::string> &visited) {
        visited.insert(curr);

        if (!reversedGraph.contains(curr)) {
            return 1;
        }

        int total = 1;

        for (const auto &[n, s]: reversedGraph.at(curr)) {
            if (!visited.contains(n)) {
                total += getCountIter(n, visited);
            }
        }

        return total;
    }

    std::unordered_map<std::string, Children> graph{};
    std::unordered_map<std::string, Children> reversedGraph{};
};

std::unique_ptr<Rules> parseRules(std::vector<std::string> &rules) {
    std::unordered_map<std::string, Children> graph{};
    std::unordered_map<std::string, Children> reversedGraph{};
    std::regex bagNameReg{ R"((.+?) bags contain)" };
    std::regex contents { R"((\d+) (.+?) bag)" };
    std::smatch fields;

    for (const auto &r: rules) {
        std::regex_search(r, fields, bagNameReg);
        std::sregex_iterator iter(r.begin(), r.end(), contents);
        std::sregex_iterator end;

        if (iter->empty()) {
            continue;
        }

        while (iter != end) {
            auto n = (*iter)[2].str();
            auto s = std::stoi((*iter)[1]);
            BagNode b(n, s);
            BagNode revB(fields[1], s);

            if (graph.contains(fields[1])) {
                auto &val = graph.at(fields[1]);
                val.insert(b);
            } else {
                graph.insert({fields[1], Children{b}});
            }

            if (reversedGraph.contains(n)) {
                auto &val = reversedGraph.at(n);
                val.insert(revB);
            } else {
                reversedGraph.insert({n, Children{revB}});
            }

            iter++;
        }
    }

    return std::make_unique<Rules>(graph, reversedGraph);
}

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<std::string> rules;
    std::string line;

    while(std::getline(input, line)) {
        rules.push_back(line);
    }

    auto r = parseRules(rules);
    auto part1 = r->getCount(std::string("shiny gold"));
    auto part2 = r->countInnerBags("shiny gold");

    std::cout << "Part1: " << part1 << "\n";
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
