#include <iostream>
#include <fstream>
#include <complex>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/range/conversion.hpp>
#include <unordered_map>
#include <vector>

enum class Direction {  North, South, East, West, Left, Right, Forward };


using Instruction = std::pair<Direction, int>;

class Ship {
public:
    explicit Ship(std::vector<Instruction>&& instructions):
    instructions(std::move(instructions)), dir(1, 0), pos(0, 0){}

    int Distance() {
        return std::abs(pos.real()) + std::abs(pos.imag());
    }

    int solve1() {
        ranges::for_each(instructions, [&](auto&& p) {
            auto const& [i, v] = p;

            walk1(i, v);
        });

        return Distance();
    }

    int solve2() {
        ranges::for_each(instructions, [&](auto&& p) {
            auto const& [i, v] = p;

            walk2(i, v);
        });

        return Distance();
    }

    void resetPosition(std::complex<int> d) {
        dir = d;
        pos = std::complex<int>(0, 0);
    }

private:

    void walk1(Direction d, int val) {
        switch (d) {
            case Direction::North: pos += std::complex<int>{0, val}; break;
            case Direction::South: pos -= std::complex<int>(0, val); break;
            case Direction::East:  pos += std::complex<int>(val, 0); break;
            case Direction::West:  pos -= std::complex<int>(val, 0); break;
            case Direction::Left:
                ranges::for_each(ranges::views::ints(0, val/90),
                                 [&](auto&& i) { dir *= std::complex<int>(0, 1); }); break;
            case Direction::Right:
                ranges::for_each(ranges::views::ints(0, val/90),
                                 [&](auto&& i) { dir *= std::complex<int>(0, -1); }); break;
            case Direction::Forward:
                pos += val * dir; break;
        }
    }

    void walk2(Direction d, int val) {
        switch (d) {
            case Direction::North: dir += std::complex<int>{0, val}; break;
            case Direction::South: dir -= std::complex<int>(0, val); break;
            case Direction::East:  dir += std::complex<int>(val, 0); break;
            case Direction::West:  dir -= std::complex<int>(val, 0); break;
            case Direction::Left:
                ranges::for_each(ranges::views::ints(0, val/90),
                                 [&](auto&& i) { dir *= std::complex<int>(0, 1); }); break;
            case Direction::Right:
                ranges::for_each(ranges::views::ints(0, val/90),
                                 [&](auto&& i) { dir *= std::complex<int>(0, -1); }); break;
            case Direction::Forward:
                pos += val * dir; break;
        }
    }


    std::complex<int> pos;
    std::complex<int> dir;
    std::vector<Instruction> instructions;
};

int main() {
    std::ifstream file("../../input.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::unordered_map<char, Direction> dirs{
        {'N', Direction::North},
        {'S', Direction::South},
        {'E', Direction::East},
        {'W', Direction::West},
        {'L', Direction::Left},
        {'R', Direction::Right},
        {'F', Direction::Forward}
    };

    std::vector<std::string> lines{ std::istream_iterator<std::string>{file}, {}};
    auto instructions = lines | ranges::views::transform([&](auto&& s) {
        char ins = s[0];
        auto val = std::stoi(s.c_str() + 1);

        return std::make_pair(dirs[ins], val);
    }) | ranges::to<std::vector<Instruction>>();

    Ship s(std::move(instructions));
    auto part1 = s.solve1();
    s.resetPosition(std::complex<int>(10, 1));
    auto part2 = s.solve2();

    std::cout << "Part1: " << part1 << "\n";
    std::cout << "Part2: " << part2 << "\n";
    return 0;
}
