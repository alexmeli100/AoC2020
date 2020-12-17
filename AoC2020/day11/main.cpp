#include <iostream>
#include <fstream>
#include <vector>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/algorithm/for_each.hpp>
#include <range/v3/algorithm/count.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/indices.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/take_while.hpp>
#include <range/v3/view/generate.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>

enum class Position {
    Floor,
    Occupied,
    Empty
};

inline Position charToPosition(char c) {
    switch (c) {
        case '#': return Position::Occupied;
        case 'L': return Position::Empty;
        default: return Position::Floor;
    }
}

inline char positionToChar(Position p) {
    switch (p) {
        case Position::Occupied: return '#';
        case Position::Empty: return 'L';
        default: return '.';
    }
}

std::vector<int> getNeighbors(int idx, int width) {
    int row = idx / width;
    int col = idx % width;

    return {
        ((row - 1) * width) + (col - 1), ((row - 1) * width) + col,
        ((row - 1) * width) + (col + 1), (row * width) + (col - 1),
        (row * width) + (col + 1), ((row + 1) * width) + (col - 1),
        ((row + 1) * width) + col, ((row + 1) * width) + (col + 1)
    };
}

using Direction = std::pair<int, int>;

bool check(const std::vector<Position> &layout, Direction & dir, int idx, int width, int height) {
    int row = idx / width;
    int col = idx % width;

    auto rngDir = [&, r = row, c = col]() mutable-> std::pair<int, int>{
        r += dir.first; c += dir.second;
        return {r, c};
    };

    auto checkIdx = [&](auto&& p) {
        const auto &[row, col] = p;
        return row >= 0 && col >= 0 && row < height && col < width;
    };

    auto idxs = ranges::views::generate(rngDir)
        | ranges::views::take_while(checkIdx)
        | ranges::views::transform([&](auto&& p) { return layout[p.first * width + p.second]; });

    auto pos =  ranges::find_if(idxs, [](auto&& p) { return p == Position::Occupied || p == Position::Empty; });

    return pos != idxs.end() && *pos == Position::Occupied;
}

template<typename Pred>
Position checkDirections(const std::vector<Position> &layout, int idx, int width, int height, Pred&& pred) {
    std::vector<Direction> dirs{ {1, 0}, {-1, 0}, {0, 1},{0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

    auto res = dirs | ranges::views::transform([&](auto&& d) { return check(layout, d, idx, width, height); });

    return pred(ranges::count(res, true));
}

template<typename Pred>
Position checkNeighbors(const std::vector<Position> &layout, int idx, int width, Pred&& pred) {
    auto n = getNeighbors(idx, width);
    auto res = n | ranges::views::transform([&](int x) { return layout[x]; });

    return pred(ranges::count(res, Position::Occupied));
}

template<typename Pred>
struct change_pos2 {
    explicit change_pos2(Pred&& pred): pred(pred){}

    Position operator()(const std::vector<Position> &layout, int idx, int width, int height) {
        return checkDirections(layout, idx, width, height, pred);
    };

private:
    Pred pred;
};

template<typename Pred>
struct [[maybe_unused]] change_pos {
    explicit change_pos(Pred&& pred): pred(pred){}

    Position operator()(const std::vector<Position> &layout, int idx, int width, int height) {
        return checkNeighbors(layout, idx, width, pred);
    }

private:
    Pred pred;
};

template<typename Pred1, typename Pred2>
class Layout {
public:
    explicit Layout(std::vector<Position> initState, int width, int height, Pred1 empty, Pred2 occupied):
    state(std::move(initState)), width(width), height(height), emptyPred(std::move(empty)), occupiedPred(std::move(occupied)) {
        std::vector<Position> t(state.size());
        // Fill topmost row
        ranges::for_each(ranges::views::ints(0, width), [&](int col) { t[col] = Position::Floor; });
        // fill bottom row
        ranges::for_each(ranges::views::ints(0, width), [&](int col) { t[(height - 1) * width + col] = Position::Floor; } );
        // fill leftmost column
        ranges::for_each(ranges::views::ints(0, height), [&](int row) { t[row * height] = Position::Floor; });
        // fill rightmost column
        ranges::for_each(ranges::views::ints(0, height), [&](int row) { t[row * height + (width - 1)] = Position::Floor; });
        temp = t;
    }

    void updateState() {
        auto cols = ranges::views::ints(1, width - 1);
        auto rows = ranges::views::ints(1, height - 1);

        ranges::for_each(cols, [&](int i) {
            ranges::for_each(rows, [&](int j) {
                int idx = j * width + i;

                switch (state[idx]) {
                    case Position::Floor: break;
                    case Position::Empty:
                        temp[idx] = emptyPred(state, idx, width, height);
                        break;
                    case Position::Occupied:
                        temp[idx] = occupiedPred(state, idx, width, height);
                        break;
                }
            });
        });

        state = temp;
    }

    void stabilizeState() {
        std::vector<Position> prev(state);

        while (true) {
            updateState();

            if (state == prev)
                break;

            prev = state;
        }
    }

    auto countOccupiedSeats() {
        return ranges::count(state, Position::Occupied);
    }

    friend std::ostream& operator<<(std::ostream& os, const Layout& t) {
        auto rowToString = [](auto&& rng) {
            return rng | ranges::views::transform(positionToChar) | ranges::to<std::string>();
        };
        auto s = t.state
                | ranges::views::chunk(t.width)
                | ranges::views::transform(rowToString);


        ranges::for_each(s, [&](auto&& row) {
            os << row << "\n";
        });

        return os;
    }

private:
    std::vector<Position> state;
    std::vector<Position> temp;
    Pred1 emptyPred;
    Pred2 occupiedPred;
    int height;
    int width;
};

std::vector<Position> getInitialState(const std::vector<std::string> &input) {
    auto getPositions = [](const std::string &line) {
        return line | ranges::views::transform(charToPosition);
    };

    return input
        | ranges::views::transform(getPositions)
        | ranges::views::join
        | ranges::to<std::vector<Position>>();
}

template<typename Pred1, typename Pred2>
auto createLayout(const std::vector<std::string> &input, int width, int height, Pred1 empty, Pred2 occupied) {
    return std::make_unique<Layout<Pred1, Pred2>>(getInitialState(input), width, height, empty, occupied);
}

int main() {
    std::ifstream input("../../input.txt");

    if (!input.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }

    std::vector<std::string> lines;
    std::string line;
    std::getline(input, line);
    static const int width = int(line.size()) + 2;
    auto s = ranges::views::repeat_n('.', width) | ranges::to<std::string>();
    lines.push_back(s);

    input.clear();
    input.seekg(0, std::ios::beg);

    while (std::getline(input, line)) {
        lines.push_back("." + line + ".");
    }

    lines.push_back(s);
    auto state = getInitialState(lines);
    change_pos2 empty([](auto&& count) { return count == 0 ? Position::Occupied: Position::Empty;});
    change_pos2 occupied([](auto&& count) { return count >= 5 ? Position::Empty : Position::Occupied; });
    auto layout = createLayout(lines, width, lines.size(), empty, occupied);
    layout->stabilizeState();

    std::cout << layout->countOccupiedSeats() << std::endl;
    return 0;
}
