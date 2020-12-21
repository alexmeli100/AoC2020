#include <iostream>
#include <fstream>
#include "antlr4-runtime.h"
#include <ranges>
#include <numeric>
#include "generated/ALexer.h"
#include "generated/AParser.h"
#include "generated/AParser2.h"

#include "parser.h"
#include "parser2.h"

int main() {
    std::ifstream file("../input.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }
    antlr4::ANTLRInputStream input(file);
    antlrcpp::ALexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);

    antlrcpp::AParser parser(&tokens);
    antlrcpp::AParser::ProgContext *tree = parser.prog();

    tokens.seek(0);

    antlrcpp::AParser2 parser2(&tokens);
    antlrcpp::AParser2::ProgContext *tree2 = parser2.prog();


    OpMap ops{
        {Op::Mul, std::multiplies{}},
        {Op::Plus, std::plus{}}
    };

    ExprVisitor prog(ops);
    std::vector<uint64_t> exprs = prog.visitProg(tree);
    auto part1 = std::accumulate(exprs.begin(), exprs.end(), 0ull, std::plus<>());

    Expr2Visitor prog2{};
    std::vector<uint64_t> exprs2 = prog2.visitProg(tree2);
    auto part2 = std::accumulate(exprs2.begin(), exprs2.end(), 0ull, std::plus<>());

    std::cout << "Part1: " << part1 << "\n";
    std::cout << "Part2: " << part2 << "\n";
    return 0;
}
