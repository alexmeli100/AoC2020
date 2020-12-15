#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <unordered_map>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/map.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/algorithm/for_each.hpp>

std::vector<uint64_t> generateIdxs(const std::string &mask, char c) {
    auto idxs = mask
        | ranges::views::reverse
        | ranges::views::enumerate
        | ranges::views::filter([c = c](auto&& p) { return p.second == c; })
        | ranges::views::transform([](auto&& p) { return p.first; })
        | ranges::to<std::vector<uint64_t>>();

    return idxs;
}

void generateRecur(std::vector<uint64_t> &floats, std::vector<std::pair<uint64_t, uint64_t>> &addresses, std::pair<uint64_t, uint64_t> address, int i) {
    if (i >= floats.size()) {
        addresses.push_back(address);
        return;
    }


    auto idx = floats[i];
    auto [zeros, ones] = address;
    std::pair<uint64_t, uint64_t> addr1{ zeros & ~(1ull << idx), ones };
    std::pair<uint64_t, uint64_t> addr2{ zeros, ones | 1ull << idx };
    i++;

    generateRecur(floats, addresses, addr1, i);
    generateRecur(floats, addresses, addr2, i);

}

std::vector<std::pair<uint64_t, uint64_t>> generateAddress(const std::string& mask) {
    auto floats = generateIdxs(mask, 'X');
    std::vector<std::pair<uint64_t, uint64_t>> address;
    generateRecur(floats, address, std::make_pair(0xffffffffffffffff, 0ull), 0);

    return address;
}

std::pair<uint64_t, uint64_t> transformMask(const std::string &s) {
    std::pair<uint64_t, uint64_t> masks{0xffffffffffffffff, 0ull};
    auto m = s | ranges::views::reverse | ranges::views::enumerate;

    return ranges::accumulate(m, masks, [](auto&& p, auto&& c) -> std::pair<uint64_t, uint64_t> {
        auto const& [zeros, ones] = p;
        auto const& [idx, bit] = c;

        switch (bit) {
            case '1':
                return { zeros, ones | 1ull << idx };
            case '0':
                return { zeros & ~(1ull << idx), ones };
            default:
                return p;
        }
    });
}

struct MemoryAssignChunk {
    MemoryAssignChunk(std::string masks, std::vector<std::pair<uint64_t, uint64_t>> assignments):
        masks(std::move(masks)), assignments(std::move(assignments)){}

    std::string masks;
    std::vector<std::pair<uint64_t, uint64_t>> assignments;

};

class Program {
public:
    explicit Program(std::vector<MemoryAssignChunk> memoryChunks): memoryChunks(std::move(memoryChunks)){}

    void initialize() {
        for (const auto& m: memoryChunks)
            ranges::for_each(m.assignments, [&](auto&& p) {
                auto const& [idx, val] = p;
                auto const& [zeros, ones] = transformMask(m.masks);

                memory[idx] = (val & zeros) | ones;
            });
    }

    void initialize2() {
        for (const auto& m: memoryChunks) {
            ranges::for_each(m.assignments, [&](auto&& p) {
                auto& [addr, val] = p;
                auto ones = generateIdxs(m.masks, '1');
                auto newAddr = ranges::accumulate(ones, addr, [](uint64_t x, uint64_t y) { return x| 1ull << y; });
                auto addrs = generateAddress(m.masks);

                ranges::for_each(addrs, [&, val = val](auto&& p) {
                    auto const &[z, o] = p;
                    auto a = (newAddr & z) | o;
                    memory[a] = val;
                });
            });
        }
    }

    auto getMemorySum() {
        return ranges::accumulate(memory | ranges::views::values, 0ull);
    }

    void reset() {
        memory.clear();
    }

private:
    std::unordered_map<uint64_t, uint64_t> memory{};
    std::vector<MemoryAssignChunk> memoryChunks;
};

std::unique_ptr<Program> parseProgram(const std::vector<std::string> &input) {
    std::vector<MemoryAssignChunk> chunks;
    std::regex maskReg{ R"(^mask = ([01X]{36})$)" };
    std::regex assignmentReg{ R"(^mem\[(\d+)\] = (\d+)$)" };
    std::smatch fields;

    int i = 0;

    while (i < input.size()) {
        std::regex_match(input[i], fields, maskReg);
        std::string mask(fields[1].str());
        std::vector<std::pair<uint64_t, uint64_t>> cs;

        while (++i < input.size() && !input[i].starts_with("mask")) {
            std::regex_match(input[i], fields, assignmentReg);
            auto zeroes = uint64_t(std::stoi(fields[1].str()));
            auto ones = uint64_t(std::stoi(fields[2].str()));
            cs.emplace_back(zeroes, ones);
        }

        chunks.emplace_back(mask, cs);

    }

    return std::make_unique<Program>(chunks);

}

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(input, line))
        lines.push_back(line);

    auto p = parseProgram(lines);
    p->initialize();
    auto part1 = p->getMemorySum();
    p->reset();
    p->initialize2();
    auto part2 = p->getMemorySum();

    std::cout << "Part1: " << part1 << std::endl;
    std::cout << "Part2: " << part2 << std::endl;
    return 0;
}
