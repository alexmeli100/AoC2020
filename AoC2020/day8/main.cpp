#include <iostream>
#include <functional>
#include <fstream>
#include <unordered_set>
#include <optional>
#include <ranges>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>

enum class OpCodes { Nop, Acc, Jmp };

using Instruction = std::pair<OpCodes, int>;

class VM {
public:
    explicit VM(std::vector<Instruction> instructions): instructions(std::move(instructions)) {
        initFuncs();
    }

    std::optional<int> findCorrupted() {
        auto nopOrJmp = instructions
                | ranges::views::transform([](const auto &p) { return p.first; })
                | ranges::views::enumerate
                | ranges::views::filter([](const auto &p) { return p.second == OpCodes::Nop || p.second == OpCodes::Jmp; });

        for (const auto &[i, op]: nopOrJmp) {
            auto revOp = op == OpCodes::Jmp ? OpCodes::Nop : OpCodes::Jmp;
            instructions[i] = { revOp, instructions[i].second };
            const auto &[val, res] = detectLoop();

            if (res)
                return val;

            reset(i, revOp);
        }

        return std::nullopt;
    }

    std::pair<int, bool> detectLoop() {
        std::unordered_set<int> executedOps;

        while (true) {

            if (pc >= instructions.size())
                return { accumulator, true };

            if (executedOps.contains(pc))
                return { accumulator, false };

            executedOps.insert(pc);
            auto [op, val] = instructions[pc];
            opFuncs.at(op)();
        }
    }

    void reset() {
        pc = 0;
        accumulator = 0;
    }
private:
    void initFuncs() {
        opFuncs[OpCodes::Nop] = [&](){
            pc++;
        };

        opFuncs[OpCodes::Acc] = [&]() {
            auto [op, val] = instructions[pc];
            accumulator += val;
            pc++;
        };

        opFuncs[OpCodes::Jmp] = [&]() {
            auto [op, val] = instructions[pc];
            pc += val;
        };
    }

    void reset(int i, OpCodes op) {
        pc = 0;
        accumulator = 0;
        instructions[i] = { op == OpCodes::Jmp ? OpCodes::Nop : OpCodes::Jmp, instructions[i].second };
    }

    std::vector<Instruction> instructions;
    std::unordered_map<OpCodes, std::function<void()>> opFuncs;
    int pc{};
    int accumulator{};
};

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    const std::unordered_map<std::string, OpCodes> strToOpcode = {
            {"nop", OpCodes::Nop},
            {"jmp", OpCodes::Jmp},
            {"acc", OpCodes::Acc}
    };

    std::vector<Instruction> instructions;
    std::string line;

    while (std::getline(input, line)) {
        auto splitText = line | ranges::views::split(' ') | ranges::to<std::vector<std::string>>();
        auto op = strToOpcode.at(splitText[0]);
        auto val = std::stoi(splitText[1]);
        instructions.emplace_back(op, val);
    }


    VM vm(instructions);
    const auto &[part1, res] = vm.detectLoop();
    vm.reset();
    auto part2 = vm.findCorrupted();


    std::cout << "Part1: " << part1 << "\n";

    if (part2)
        std::cout << "Part2: " << part2.value() << "\n";
    else
        std::cout << "Part2: " << "could not fix program" << "\n";


    return 0;
}
