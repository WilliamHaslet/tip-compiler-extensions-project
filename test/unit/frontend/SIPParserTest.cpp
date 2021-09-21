#include "ParserHelper.h"
#include "FrontEnd.h"
#include "ParseError.h"
#include "ExceptionContainsWhat.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("SIP Parser: booleans", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        bool()
        {
            var t, f;
            if (true){
                t = true;
                f = false;
            }
            else {
                t = false;
                f = true;
            }
            return t;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: not test", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = true;
            if (not false){
            }
            if (not a){
            }
            return a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: not", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        opp()
        {
            var a;
            a = true;
            if (not false){
            }
            if (not a){
            }
            return a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: and", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        binary()
        {
            var a, b, c;
            a = true;
            b = false;
            if (a and b){
                c = a and b;
            }
            return c;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: or", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        binary()
        {
            var a, b, c;
            a = true;
            b = false;
            if (a or b){
                c = a or b;
            }
            return c;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: No capitalized not", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        opp()
        {
            var a;
            a = true;
            if (Not false){
                
            }
            if (not a){

            }

            return a;
        }
    )";
    REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

//Array SIP Tests

TEST_CASE("SIP Parser: array", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        arrs()
        {
            var a1, a2, a3, a6;
            a1 = [2, 3, 4, 5, true, false, -3];
            a2 = [a3, a6];
            return a2;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: array no comma fail", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        arrs()
        {
            var a;
            a = [4 5];
            return a2;
        }
    )";
    REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: array of test", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = [10 of 1];
            return a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: ofArray", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        arrs()
        {
            var x, y, z;
            x = [y of z];
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: array indexing", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = [4, 5, 6];
            return a[0];
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: array length test", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = [4, 5, 6];
            return #a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: EmptyArray", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        arrs()
        {
            var a1, a2, a3, a6;
            a1 = [];
            a2 = [];
            return a2;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: missingBracketArray", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        arrs()
        {
            var x, y, z;
            x = [y of z;
            return x;
        }
    )";
    REQUIRE_THROWS_MATCHES(FrontEnd::parse(stream), ParseError, ContainsWhat("missing ']'"));
}

TEST_CASE("SIP Parser: mod test", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = 5 % 2;
            return a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}


TEST_CASE("SIP Parser: incorrectMod", "[SIP Parser]")
{
    std::stringstream stream;
    stream << R"(
        modTest()
        {
            var a1, a2, a3;
            a1 = 6;
            a2 = not true;
            a3 = (6 + 9);
            if (a3 > 0) {
                a1 = a1 % a3;
            }
            else {
                a3 = %;
            }
            return a3;
        }
    )";
    REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: negation", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var a;
            a = 3;
            return -a;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: greater than equal to", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x;
            if (x >= 0) {
                while (x >= 0) {
                    x = x + 1;
                }
            }
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: less than", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x;
            if (x < 0) {
                while (x < 0) {
                    x = x + 1;
                }
            }
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: less than or equal to", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x;
            if (x <= 0) {
                while (x <= 0) {
                    x = x + 1;
                }
            }
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: ternary expression", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x, y;
            x = 5;
            y = 9;
            x = x > 3 ? 7 : y;
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: increment", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x;
            x = 5;
            x++;
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: decrement", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x;
            x = 5;
            x--;
            return x;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: iterator style for loop", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var x, i, o;
            x = [1, 3, 5, 7];
            for (i : x)
            {
                o = o + i;
            }
            return o;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: range style for loop", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var i, o;
            for (i : 0 .. 10)
            {
                o = o + i;
            }
            return o;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: range style for loop with step", "[SIP Parser]") 
{
    std::stringstream stream;
    stream << R"(
        short()
        {
            var i, o;
            for (i : 0 .. 10 by 2)
            {
                o = o + i;
            }
            return o;
        }
    )";
    REQUIRE(ParserHelper::is_parsable(stream));
}

//Testing for Precedence of Operators in SIP Extensions

TEST_CASE("SIP Parser: not higher precedence than and", "[SIP Parser]") {
    std::stringstream stream;
    stream << R"(main() { return true and not true; })";
    std::string expected = "(expr (expr true) and (expr not (expr true)))";
    std::string tree = ParserHelper::parsetree(stream);
    REQUIRE(tree.find(expected) != std::string::npos); 
}

TEST_CASE("SIP Parser: not higher precedence than or", "[SIP Parser]") {
    std::stringstream stream;
    stream << R"(main() { return true or not true; })";
    std::string expected = "(expr (expr true) or (expr not (expr true)))";
    std::string tree = ParserHelper::parsetree(stream);
    REQUIRE(tree.find(expected) != std::string::npos); 
}

TEST_CASE("SIP Parser: mod higher precedence than addition", "[SIP Parser]") {
    std::stringstream stream;
    stream << R"(main() { return x % y + z; })";
    std::string expected = "(expr (expr (expr x) % (expr y)) + (expr z))";
    std::string tree = ParserHelper::parsetree(stream);
    REQUIRE(tree.find(expected) != std::string::npos); 
}

//Illegal Comparisons and Operators

TEST_CASE("SIP Parser: Additional Greater Than", "[SIP Parser]") {
    std::stringstream stream;
    stream << R"(
      short() { var x; if (x>>=0) x = x + 1 return 0; }
    )";

    REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}

TEST_CASE("SIP Parser: Additional Less Than", "[SIP Parser]") {
    std::stringstream stream;
    stream << R"(
      short() { var x; if (x<<0) x = x + 1 return 0; }
    )";

    REQUIRE_FALSE(ParserHelper::is_parsable(stream));
}
