#include "ASTHelper.h"
#include "CheckAssignable.h"
#include "SemanticError.h"
#include "ExceptionContainsWhat.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("Check Assignable: variable lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 1; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: pointer lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(ptrlhs() { var x; *x = 1; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: field lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(fieldlhs() { var x; x = {f:0, g:1}; x.f = 1; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: complex field lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x; {f:0, g:1}.f = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("{f:0,g:1} is an expression, and not a variable corresponding to a record"));
}

TEST_CASE("Check Assignable: complex pointer lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo(x) { return &x; } ptrlhs() { var x; *foo(&x) = 1; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: address of var", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x, y; x = &y; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: address of field", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x, y; y = {f:0, g:1}; x = &(y.g); return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: address of element reference", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x, y; y = [1,2,3,4,5]; x = &(y[3]); return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: element reference lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = [1,2,3,4,5]; x[0] = 8; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: increment statment", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; x++; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: decrement statment", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; x--; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: for range loop", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; for (x : 12 .. 25){} return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

TEST_CASE("Check Assignable: for iter loop", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; a = [1,2,3,4]; for (x:a){} return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_NOTHROW(CheckAssignable::check(ast.get()));
}

/************** the following are expected to fail the check ************/

TEST_CASE("Check Assignable: constant lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(constlhs() { var x; 7 = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("7 not an l-value"));
}

TEST_CASE("Check Assignable: binary lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(binlhs() { var x; x+1 = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("(x+1) not an l-value"));
}

TEST_CASE("Check Assignable: function lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(foo() { return 0; } funlhs() { var x; foo() = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("foo() not an l-value"));
}

TEST_CASE("Check Assignable: alloc lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(alloclhs() { var x; alloc 1 = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("alloc 1 not an l-value"));
}

TEST_CASE("Check Assignable: record lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs() { var x; {f:0, g:1} = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("{f:0,g:1} not an l-value"));
}

TEST_CASE("Check Assignable: address of pointer", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs(p) { var x; x = &(*p); return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("(*p) not an l-value"));
}

TEST_CASE("Check Assignable: address of expr", "[Symbol]") {
    std::stringstream stream;
    stream << R"(recordlhs(p) { var x, y; x = &(y*y); return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("(y*y) not an l-value"));
}

TEST_CASE("Check Assignable: array lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(arraylhs(p) { var x, y; [1,2] = x; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("[1, 2] not an l-value"));
}

TEST_CASE("Check Assignable: array length lhs", "[Symbol]") {
    std::stringstream stream;
    stream << R"(lenlhs(p) { var x, y; x = [1,2,3,4,5]; #x = 4; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("(#x) not an l-value"));
}

TEST_CASE("Check Assignable: integer as iterator in for-iter loop", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; a = [1,2,3,4]; for (1:a){} return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("1 not an l-value"));
}

TEST_CASE("Check Assignable: integer as iterator in for-range loop", "[Symbol]") {
    std::stringstream stream;
    stream << R"(varlhs() { var x; x = 0; for (5 : 12 .. 25){} return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("5 not an l-value"));
}

TEST_CASE("Check Assignable: integer increment", "[Symbol]") {
    std::stringstream stream;
    stream << R"(binlhs() { var x; 3++; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("3 not an l-value"));
}

TEST_CASE("Check Assignable: integer decrement", "[Symbol]") {
    std::stringstream stream;
    stream << R"(binlhs() { var x; 3--; return 0; })";
    auto ast = ASTHelper::build_ast(stream);
    REQUIRE_THROWS_MATCHES(CheckAssignable::check(ast.get()),
                           SemanticError,
                           ContainsWhat("3 not an l-value"));
}


