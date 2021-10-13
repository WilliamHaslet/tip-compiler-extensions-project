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
