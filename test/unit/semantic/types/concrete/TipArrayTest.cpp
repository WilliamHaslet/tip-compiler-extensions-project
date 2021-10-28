#include "TipInt.h"
#include "TipRef.h"
#include "TipBool.h"
#include "TipArray.h"
#include "TipFunction.h"

#include <catch2/catch_test_macros.hpp>

#include <sstream>
#include <iostream>


TEST_CASE("TipArray: Test arity is one" "[TipArray]") {
    auto term = std::make_shared<TipInt>();
    TipArray tipArray(term);
    REQUIRE(1 == tipArray.arity());
}

TEST_CASE("TipArray: Test equality" "[TipArray]") {
    auto term = std::make_shared<TipInt>();
    TipArray tipArrayA(term);

    SECTION("Equal when both arrays have the same underlying type") {
        auto termB = std::make_shared<TipInt>();
        TipArray tipArrayB(termB);

        REQUIRE(tipArrayA == tipArrayB);
    }

    SECTION("Not equal when underlying type is different") {
        auto termB = std::make_shared<TipBool>();
        TipArray tipArrayB(termB);

        REQUIRE(tipArrayA != tipArrayB);
    }
}

TEST_CASE("TipArray: Test getter" "[TipArray]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipArray>(std::make_shared<TipInt>());
    auto tipFunction = std::make_shared<TipFunction>(params, ret);
    TipArray tipArray(tipFunction);

    REQUIRE(*tipFunction == *tipArray.getType());
}

TEST_CASE("TipArray: Test output stream" "[TipArray]") {
    std::vector<std::shared_ptr<TipType>> params {
            std::make_shared<TipInt>(),
            std::make_shared<TipInt>()
    };
    auto ret = std::make_shared<TipInt>();
    auto tipFunction = std::make_shared<TipFunction>(params, ret);
    TipArray tipArray(tipFunction);

    auto expectedValue = "[] (int,int) -> int";
    std::stringstream stream;
    stream << tipArray;
    std::string actualValue = stream.str();

    REQUIRE(expectedValue == actualValue);
}