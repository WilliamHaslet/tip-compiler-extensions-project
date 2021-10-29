#include "ASTHelper.h"
#include "ASTVariableExpr.h"
#include "TipFunction.h"
#include "TipInt.h"
#include "TipRef.h"
#include "TipAlpha.h"
#include "TipMu.h"
#include "TipBool.h"
#include "TipArray.h"
#include "TypeConstraintCollectVisitor.h"
#include "TypeConstraintUnifyVisitor.h"
#include "TypeConstraintVisitor.h"
#include "UnificationError.h"
#include "Unifier.h"

#include <catch2/catch_test_macros.hpp>

#include <iostream>

TEST_CASE("Unifier: Collect and then unify constraints", "[Unifier, Collect]") {

    SECTION("Test type-safe program 1") {
        std::stringstream program;
        program << R"(
            // x is int, y is &int, z is int, short is () -> int
            short() {
              var x, y, z;
              x = input;	
              y = alloc x;
              *y = x;
              z = *y;
              return z;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_NOTHROW(unifier.solve());

        // Expected types
        std::vector<std::shared_ptr<TipType>> emptyParams;
        auto intType = std::make_shared<TipInt>();
        auto funRetInt = std::make_shared<TipFunction>(emptyParams,intType);
        auto ptrToInt = std::make_shared<TipRef>(intType);

        auto fDecl = symbols->getFunction("short"); 
        auto fType = std::make_shared<TipVar>(fDecl); 

        REQUIRE(*unifier.inferred(fType) == *funRetInt);

        auto xType = std::make_shared<TipVar>(symbols->getLocal("x",fDecl));
        REQUIRE(*unifier.inferred(xType) == *intType);

        auto yType = std::make_shared<TipVar>(symbols->getLocal("y",fDecl));
        REQUIRE(*unifier.inferred(yType) == *ptrToInt);

        auto zType = std::make_shared<TipVar>(symbols->getLocal("z",fDecl));
        REQUIRE(*unifier.inferred(zType) == *intType);
    }

    SECTION("Test type-safe poly") {
        std::stringstream program;
        program << R"(
// poly is (&\alpha<*p>) -> \alpha<*p>, p is &\alpha<*p>
poly(p){
    return *p;
}
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_NOTHROW(unifier.solve());

        auto fDecl = symbols->getFunction("poly"); 
        auto fType = std::make_shared<TipVar>(fDecl); 
        auto pType = std::make_shared<TipVar>(symbols->getLocal("p",fDecl));

        auto polyInferred = unifier.inferred(fType);
        auto polyFun = std::dynamic_pointer_cast<TipFunction>(polyInferred); 
        REQUIRE(polyFun != nullptr); // function type
        REQUIRE(polyFun->getParams().size() == 1); // single parameter
        auto polyArg = polyFun->getParams().back();
        auto polyArgRef = std::dynamic_pointer_cast<TipRef>(polyArg); 
        REQUIRE(polyArgRef != nullptr); // param is ref
        auto polyArgAddressOfField = polyArgRef->getAddressOfField();
        REQUIRE(std::dynamic_pointer_cast<TipAlpha>(polyArgAddressOfField)); // param is ref of an alpha

        auto pInferred = unifier.inferred(pType);
        REQUIRE(*pInferred == *polyArg); // p is the parameter type
    }

    SECTION("Test unification error 1") {
        std::stringstream program;
        program << R"(
            bar(g,x) {
                var r;
                if (x==0){
                    r=g;
                } else {
                    r=bar(2,0);
                }
                return r+1;
            }

            main() {
                return bar(null,1);
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_THROWS_AS(unifier.solve(), UnificationError);
    }

    SECTION("Test unification error 2") {
        std::stringstream program;
        program << R"(
            foo(p) {
                return *p;
            }

            main() {
                var x;
                x = 5;
                x = foo;
                return 4;
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_THROWS_AS(unifier.solve(), UnificationError);
    }

    SECTION("Test unification error 3") {
        std::stringstream program;
        program << R"(
            main() {
                var x, y;
                x = 5;
                y = 10;
                x = &y;
                return 4;
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_THROWS_AS(unifier.solve(), UnificationError);
    }

SECTION("Test type-safe bools") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              x = true;
              y = false;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_NOTHROW(unifier.solve());

        // Expected types
        std::vector<std::shared_ptr<TipType>> emptyParams;
        auto intType = std::make_shared<TipInt>();
        auto boolType = std::make_shared<TipBool>();
        auto funRetInt = std::make_shared<TipFunction>(emptyParams,intType);

        auto fDecl = symbols->getFunction("short"); 
        auto fType = std::make_shared<TipVar>(fDecl); 

        REQUIRE(*unifier.inferred(fType) == *funRetInt);

        auto xType = std::make_shared<TipVar>(symbols->getLocal("x",fDecl));
        REQUIRE(*unifier.inferred(xType) == *boolType);

        auto yType = std::make_shared<TipVar>(symbols->getLocal("y",fDecl));
        REQUIRE(*unifier.inferred(yType) == *boolType);
    }

    SECTION("Test type-safe arrays") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = [1, 2, 3];
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintCollectVisitor visitor(symbols.get());
        ast->accept(&visitor);

        Unifier unifier(visitor.getCollectedConstraints());
        REQUIRE_NOTHROW(unifier.solve());

        // Expected types
        std::vector<std::shared_ptr<TipType>> emptyParams;
        auto intType = std::make_shared<TipInt>();
        auto arrayType = std::make_shared<TipArray>(intType);
        auto funRetInt = std::make_shared<TipFunction>(emptyParams,intType);

        auto fDecl = symbols->getFunction("short"); 
        auto fType = std::make_shared<TipVar>(fDecl); 

        REQUIRE(*unifier.inferred(fType) == *funRetInt);

        auto xType = std::make_shared<TipVar>(symbols->getLocal("x",fDecl));
        REQUIRE(*unifier.inferred(xType) == *arrayType);
    }
}

TEST_CASE("Unifier: Unify constraints on the fly", "[Unifier, On-the-fly]") {

    SECTION("Test type-safe program 1") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              x = input;
              y = alloc x;
              *y = x;
              z = *y;
              return z;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe record2") {
        std::stringstream program;
        program << R"(
main() {
    var n, r1;
    n = alloc {p: 4, q: 2};
    *n = {p:5, q: 6};
    r1 = (*n).p; // output 5
    output r1;
    return 0;
}    
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe function application") {
        std::stringstream program;
        program << R"(
            foo(x) {
                return null;
            }

            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = foo(x);
              return w;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe record4") {
        std::stringstream program;
        program << R"(
main() {
    var n, k, r1;
    k = {a: 1, b: 2};
    n = {c: &k, d: 4};
    r1 = ((*(n.c)).a); // output 1
    output r1;
    return 0;
}
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe poly") {
        std::stringstream program;
        program << R"(
poly(p){
    return *p;
}
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test unification error 1") {
        std::stringstream program;
        program << R"(
            bar(g,x) {
                var r;
                if (x==0){
                    r=g;
                } else {
                    r=bar(2,0);
                }
                return r+1;
            }

            main() {
                return bar(null,1);
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test unification error 2") {
        std::stringstream program;
        program << R"(
            foo(p) {
                return *p;
            }

            main() {
                var x;
                x = 5;
                x = foo;
                return 4;
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test unification error 3") {
        std::stringstream program;
        program << R"(
            main() {
                var x, y;
                x = 5;
                y = 10;
                x = &y;
                return 4;
            }
        )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test unification error 4") {
        std::stringstream program;
        program << R"(
        foo() {
            var r, q;
            q = {f: 1, h: 3};
            r = {f: 4, g: 13};
            r.h = q.h;
            return r.g;
        }
        )";
        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);    
    }

    SECTION("Test type-safe bools") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              x = true;
              y = false;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe bools") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              x = true;
              y = 2;
              x = 1;
              y = false;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe and or not") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              y = true;
              z = false;
              
              x = true and false;
              x = true and true;
              x = false and false;

              x = true or false;
              x = true or true;
              x = false or false;

              x = not true;
              x = not false;

              x = y and z;
              x = y and y;
              x = z and z;

              x = y or z;
              x = y or y;
              x = z or z;

              x = not y;
              x = not z;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe inc dec") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = 1;
              x++;
              x--;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe inc dec") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = true;
              x++;
              x--;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe ternary") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              y = 2;
              z = 4;
              x = true ? 0 : 1;
              x = x == 0 ? y : z;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe ternary") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              y = 2;
              z = false;
              x = 0 ? false : 1;
              x = x == 0 ? y : z;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe for iter") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              y = [1, 2, 3];
              for (x : y)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe for iter") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              y = 2;
              for (x : y)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe for range") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              for (x : 2 .. 10)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe for range") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              for (x : 2 .. false)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe for range by") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              for (x : 2 .. 10 by 2)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe for range by") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = false;
              for (x : 2 .. 10 by 2)
              {
                  
              }
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe array") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = [x, y, z];
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe array") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = true;
              y = 2;
              z = false;
              w = [x, y, z];
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe comparisons") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              y = 2;
              z = 3;

              x = 1 > 0;
              x = 1 >= 0;
              x = 1 < 0;
              x = 1 <= 0;
              x = 1 == 0;
              x = 1 != 0;

              x = y > z;
              x = y >= z;
              x = y < z;
              x = y <= z;
              x = y == z;
              x = y != z;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe comparisons") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y, z;
              y = 2;
              z = true;

              x = false > 0;
              x = false >= 0;
              x = false < 0;
              x = false <= 0;
              x = false == 0;
              x = false != 0;

              x = y > z;
              x = y >= z;
              x = y < z;
              x = y <= z;
              x = y == z;
              x = y != z;
              return 0;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test negation operator") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              x = 1;
              y = -x;
              return y;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test unsafe negation operator") {
        std::stringstream program;
        program << R"(
            short() {
              var x, y;
              x = true;
              y = -x;
              return y;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe element reference") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = [1, 2, 3];
              return w[0];
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe element reference") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = [1, 2, 3];
              return w[true];
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe array length") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = [x, y, z];
              return #w;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-safe array length 2") {
        std::stringstream program;
        program << R"(
            short() {
              var w, x, y, z;
              x = 1;
              y = 2;
              z = 3;
              w = [x, y, z];
              x = 1 + #w;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe array length") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = true;
              return #x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe mod") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = 5 % 2;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe mod") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = true % 2;
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }

    SECTION("Test type-safe of array") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = [10 of true];
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_NOTHROW(ast->accept(&visitor));
    }

    SECTION("Test type-unsafe of array") {
        std::stringstream program;
        program << R"(
            short() {
              var x;
              x = [true of 2];
              return x;
            }
         )";

        auto ast = ASTHelper::build_ast(program);
        auto symbols = SymbolTable::build(ast.get());

        TypeConstraintUnifyVisitor visitor(symbols.get());
        REQUIRE_THROWS_AS(ast->accept(&visitor), UnificationError);
    }
}


TEST_CASE("Unifier: Test unifying TipCons with different arities", "[Unifier]") {
    std::vector<std::shared_ptr<TipType>> paramsA {std::make_shared<TipInt>()};
    auto retA = std::make_shared<TipInt>();
    auto tipFunctionA = std::make_shared<TipFunction>(paramsA, retA);

    std::vector<std::shared_ptr<TipType>> paramsB {std::make_shared<TipInt>(), std::make_shared<TipInt>()};
    auto retB = std::make_shared<TipInt>();
    auto tipFunctionB = std::make_shared<TipFunction>(paramsB, retB);

    TypeConstraint constraint(tipFunctionA, tipFunctionB);
    std::vector<TypeConstraint> constraints {constraint};

    Unifier unifier(constraints);
    REQUIRE_THROWS_AS(unifier.unify(tipFunctionA, tipFunctionB), UnificationError);
}

TEST_CASE("Unifier: Test unifying TipCons with the same arity", "[Unifier]") {
    std::vector<std::shared_ptr<TipType>> params {std::make_shared<TipInt>()};
    auto ret = std::make_shared<TipInt>();
    auto tipFunctionA = std::make_shared<TipFunction>(params, ret);

    auto tipFunctionB = std::make_shared<TipFunction>(params, ret);

    TypeConstraint constraint(tipFunctionA, tipFunctionB);
    std::vector<TypeConstraint> constraints {constraint};

    Unifier unifier(constraints);
    REQUIRE_NOTHROW(unifier.unify(tipFunctionA, tipFunctionB));
}

TEST_CASE("Unifier: Test unifying proper types with a type variable", "[Unifier]") {
    ASTVariableExpr variableExpr("foo");
    auto tipVar = std::make_shared<TipVar>(&variableExpr);
    auto tipInt = std::make_shared<TipInt>();

    TypeConstraint constraint(tipVar, tipInt);
    std::vector<TypeConstraint> constraints {constraint};

    Unifier unifier(constraints);
    REQUIRE_NOTHROW(unifier.unify(tipVar, tipInt));
}

TEST_CASE("Unifier: Test unifying two different type variables", "[Unifier]") {
    ASTVariableExpr variableExprA("foo");
    auto tipVarA = std::make_shared<TipVar>(&variableExprA);

    ASTVariableExpr variableExprB("foo");
    auto tipVarB = std::make_shared<TipVar>(&variableExprB);

    TypeConstraint constraint(tipVarA, tipVarB);
    std::vector<TypeConstraint> constraints {constraint};

    Unifier unifier(constraints);
    REQUIRE_NOTHROW(unifier.unify(tipVarA, tipVarB));
}

TEST_CASE("Unifier: Test closing mu ", "[Unifier]") {
    // Some building block types for setting up test
    ASTVariableExpr variableExprG("g");
    auto theAlphaG = std::make_shared<TipAlpha>(&variableExprG);

    auto theInt = std::make_shared<TipInt>();

    ASTVariableExpr variableExprFoo("foo");
    auto theVarFoo = std::make_shared<TipVar>(&variableExprFoo);

    // mu alpha<f> . (alpha<f>, alpha<g>) -> alpha<g>
    ASTVariableExpr variableExprF("f");
    auto theAlphaF = std::make_shared<TipAlpha>(&variableExprF);

    std::vector<std::shared_ptr<TipType>> params {theAlphaF, theAlphaG};
    auto theFunction = std::make_shared<TipFunction>(params, theAlphaG);

    auto theMu = std::make_shared<TipMu>(theAlphaF, theFunction);

    // unify alpha<g> with int
    // unify var<foo> with theMu
    TypeConstraint constraint1(theAlphaG, theInt);
    TypeConstraint constraint2(theVarFoo, theMu);
    std::vector<TypeConstraint> constraints {constraint1, constraint2};
    Unifier unifier(constraints);

    // closing the mu should produce: mu alpha<f> . (alpha<f>, int) -> int
    auto closed = unifier.inferred(theMu);

    std::stringstream ss;
    ss << *closed;

    REQUIRE_NOTHROW(ss.str() == "\u03bc\u03B1<f>.(\u03B1<f>,int) -> int");
}

TEST_CASE("Unifier: Test unifying proper types with a type ", "[Unifier]") {
    ASTVariableExpr variableExpr("foo");
    auto tipVar = std::make_shared<TipVar>(&variableExpr);
    auto tipInt = std::make_shared<TipInt>();

    TypeConstraint constraint(tipVar, tipInt);
    std::vector<TypeConstraint> constraints {constraint};

    Unifier unifier(constraints);
    REQUIRE_NOTHROW(unifier.unify(tipVar, tipInt));
}
