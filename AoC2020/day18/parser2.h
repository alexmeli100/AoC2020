//
// Created by alexm on 2020-12-21.
//

#ifndef DAY18_PARSER2_H
#define DAY18_PARSER2_H

#include <string>
#include <functional>
#include "antlr4-runtime.h"
#include "generated/AParser2.h"
#include "generated/ALexer.h"
#include "generated/AParser2BaseVisitor.h"

class Expr2Visitor: antlrcpp::AParser2BaseVisitor {
public:
    Expr2Visitor() = default;

    antlrcpp::Any visitProg(antlrcpp::AParser2::ProgContext *ctx) override {
        std::vector<uint64_t> exprs;

        for (auto exp: ctx->expression()) {
            uint64_t res = visit(exp);
            exprs.push_back(res);
        }

        return exprs;
    }

    antlrcpp::Any visitNumber(antlrcpp::AParser2::NumberContext *ctx) override {
        auto num = ctx->NUMBER()->getText();

        return uint64_t(std::stoi(num));
    }

    antlrcpp::Any visitPlus(antlrcpp::AParser2::PlusContext *ctx) override {
        uint64_t left = visit(ctx->expression(0));
        uint64_t right = visit(ctx->expression(1));

        return left + right;
    }

    antlrcpp::Any visitMul(antlrcpp::AParser2::MulContext *ctx) override {
        uint64_t left = visit(ctx->expression(0));
        uint64_t right = visit(ctx->expression(1));

        return left * right;
    }

    antlrcpp::Any visitParens(antlrcpp::AParser2::ParensContext *ctx) override {
        uint64_t expr = visit(ctx->expression());

        return expr;
    }

};

#endif //DAY18_PARSER2_H
