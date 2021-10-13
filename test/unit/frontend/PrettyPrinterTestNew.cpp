#include "ASTHelper.h"
#include "GeneralHelper.h"
#include "PrettyPrinter.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("PrettyPrinter: Test and", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; x = 3 and 5; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = (3 and 5);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test decrement", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; x = 3; x--; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = 3;
  x--;
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test for iter", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var i, x, p;
        x = [2 of 10];
        for (i : x)
        {
            p = i;
        }
        return 0; })";

    std::string expected = R"(prog() 
{
  var i, x, p;
  x = [2 of 10];
  for (i : x)
    {
      p = i;
    }
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test for range", "[PrettyPrinter]")
{
  std::stringstream stream;
  stream << R"(prog()
  {
    for (1 : 2 .. 3 by 4)
    {
    }
    return 0;
  })";

  std::string expected = R"(prog() 
{
  for (1 : 2 .. 3 by 4) 
    {
    }
  return 0;
}
)";

  std::stringstream pp;
  auto ast = ASTHelper::build_ast(stream);
  PrettyPrinter::print(ast.get(), pp, ' ', 2);
  std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
  expected = GeneralHelper::removeTrailingWhitespace(expected);
  REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test increment", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; x = 3; x++; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = 3;
  x++;
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test negation expr", "[PrettyPrinter]") {
        std::stringstream stream;
    stream << R"(prog() { var x, y; y = 3; x = -y; return 0; })";

    std::string expected = R"(prog() 
{
  var x, y;
  y = 3;
  x = -(y);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test not expr", "[PrettyPrinter]") {
        std::stringstream stream;
    stream << R"(prog() { var x; x = not 3; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = (not 3);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test of array", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; x = [2 of 10]; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = [2 of 10];
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test or", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x; x = 4 or 8; return 0; })";

    std::string expected = R"(prog() 
{
  var x;
  x = (4 or 8);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test array expression", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
prog() { var x, y, z; x = [ 2, 3, 4, 5 ]; y = [ 43-10, z+x, 12] return x; })";

    std::string expected = R"(prog() 
{
  var x, y, z;
  x = [2, 3, 4, 5];
  y = [(43 - 10), (z + x), 12];
  return x;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test element reference expression", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
prog() { var x, y, z; x = [ 2, 3, 4, 5 ]; y = x[2]; return y; })";

    std::string expected = R"(prog() 
{
  var x, y, z;
  x = [2, 3, 4, 5];
  y = (x[2]);
  return y;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test array length operator", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
prog() { var x, y, z; x = [ 2, 3, 4, 5 ]; y = # x; return y; })";

    std::string expected = R"(prog() 
{
  var x, y, z;
  x = [2, 3, 4, 5];
  y = (#x);
  return y;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}

TEST_CASE("PrettyPrinter: Test ternary operator", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(
prog() { var v, w, x, y, z; w = 10; y = true; v = false; z = 5; x = w ? y : z; return x; })";

    std::string expected = R"(prog() 
{
  var v, w, x, y, z;
  w = 10;
  y = true;
  v = false;
  z = 5;
  x = (w ? y : z);
  return x;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}


TEST_CASE("PrettyPrinter: Test modulus expr", "[PrettyPrinter]") {
    std::stringstream stream;
    stream << R"(prog() { var x, y; x = y % 3 + 4 - y; return 0; })";

    std::string expected = R"(prog() 
{
  var x, y;
  x = (((y % 3) + 4) - y);
  return 0;
}
)";

    std::stringstream pp;
    auto ast = ASTHelper::build_ast(stream);
    PrettyPrinter::print(ast.get(), pp, ' ', 2);
    std::string ppString = GeneralHelper::removeTrailingWhitespace(pp.str());
    expected = GeneralHelper::removeTrailingWhitespace(expected);
    REQUIRE(ppString == expected);
}
