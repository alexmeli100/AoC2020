//
// Created by alexm on 2020-12-19.
//

#ifndef DAY18_PARSER_H
#define DAY18_PARSER_H

#include <string>
#include <functional>
#include "antlr4-runtime.h"
#include "generated/AParser.h"
#include "generated/ALexer.h"
#include "generated/AParserBaseVisitor.h"

enum class Op { Plus, Mul };

using OpMap = std::unordered_map<Op, std::function<uint64_t (uint64_t , uint64_t)>>;

class ExprVisitor: antlrcpp::AParserBaseVisitor {
public:
    explicit ExprVisitor(OpMap &ops): ops(ops){}

    antlrcpp::Any visitProg(antlrcpp::AParser::ProgContext *ctx) override {
        std::vector<uint64_t> exprs;

        for (auto exp: ctx->expression()) {
            uint64_t res = visit(exp);
            exprs.push_back(res);
        }

        return exprs;
    }

    antlrcpp::Any visitNumber(antlrcpp::AParser::NumberContext *ctx) override {
        auto num = ctx->NUMBER()->getText();

        return uint64_t(std::stoi(num));
    }

    antlrcpp::Any visitPlusMult(antlrcpp::AParser::PlusMultContext *ctx) override {
        Op op = ctx->children[1]->getText() == "+" ? Op::Plus : Op::Mul;
        uint64_t left = visit(ctx->expression(0));
        uint64_t right = visit(ctx->expression(1));

        return ops[op](left, right);
    }

    antlrcpp::Any visitParens(antlrcpp::AParser::ParensContext *ctx) override {
        uint64_t expr = visit(ctx->expression());

        return expr;
    }

private:
    OpMap ops;
};



#endif //DAY18_PARSER_H
