#include "TypeConstraintVisitor.h"
#include "TipVar.h"
#include "TipFunction.h"
#include "TipAlpha.h"
#include "TipRef.h"
#include "TipRecord.h"
#include "TipAbsentField.h"
#include "TipInt.h"
#include "TipBool.h"
#include "TipArray.h"

TypeConstraintVisitor::TypeConstraintVisitor(SymbolTable* st, std::unique_ptr<ConstraintHandler> handler)
  : symbolTable(st), constraintHandler(std::move(handler)) {};

/*! \fn astToVar
 *  \brief Convert an AST node to a type variable.
 *
 * Utility function that creates type variables and uses declaration nodes
 * as a canonical representative for program variables.  There are two case
 * that need to be checked: if the variable is local to a function or if
 * it is a function value.
 */
std::shared_ptr<TipType> TypeConstraintVisitor::astToVar(ASTNode * n) {
  if (auto ve = dynamic_cast<ASTVariableExpr*>(n)) {
    ASTDeclNode * canonical;
    if ((canonical = symbolTable->getLocal(ve->getName(), scope.top()))) {
      return std::make_shared<TipVar>(canonical);
    } else if ((canonical = symbolTable->getFunction(ve->getName()))) {
      return std::make_shared<TipVar>(canonical);
    } 
  }  // LCOV_EXCL_LINE

  return std::make_shared<TipVar>(n);
}

bool TypeConstraintVisitor::visit(ASTFunction * element) {
    scope.push(element->getDecl());
    return true;
}

/*! \brief Type constraints for function definition.
 *
 * Type rules for "main(X1, ..., Xn) { ... return E; }":
 *   [[X1]] = [[Xn]] = [[E]] = int
 * To express this we will equate all type variables to int.
 *
 * Type rules for "X(X1, ..., Xn) { ... return E; }":
 *   [[X]] = ([[X1]], ..., [[Xn]]) -> [[E]]
 */
void TypeConstraintVisitor::endVisit(ASTFunction * element) {
  if (element->getName() == "main") {
    std::vector<std::shared_ptr<TipType>> formals;
    for(auto &f : element->getFormals()) {
      formals.push_back(astToVar(f));
      // all formals are int
      constraintHandler->handle(astToVar(f), std::make_shared<TipInt>());
    }

    // Return is the last statement and must be int
    auto ret = dynamic_cast<ASTReturnStmt*>(element->getStmts().back());
    constraintHandler->handle(astToVar(ret->getArg()), std::make_shared<TipInt>());

    constraintHandler->handle(astToVar(element->getDecl()),
                              std::make_shared<TipFunction>(formals, astToVar(ret->getArg())));
  } else {
    std::vector<std::shared_ptr<TipType>> formals;
    for(auto &f : element->getFormals()) {
      formals.push_back(astToVar(f));
    }

    // Return is the last statement 
    auto ret = dynamic_cast<ASTReturnStmt*>(element->getStmts().back());

    constraintHandler->handle(astToVar(element->getDecl()),
                              std::make_shared<TipFunction>(formals, astToVar(ret->getArg())));
  }
}

/*! \brief Type constraints for numeric literal.
 *
 * Type rules for "I":  
 *   [[I]] = int
 */
void TypeConstraintVisitor::endVisit(ASTNumberExpr * element) {
    constraintHandler->handle(astToVar(element), std::make_shared<TipInt>());
}

/*! \brief Type constraints for binary operator.
 *
 * Type rules for "E1 op E2":
 *   [[E1 op E2]] = int
 * and if "op" is not equality or disequality
 *   [[E1]] = [[E2]] = int
 * otherwise
 *   [[E1]] = [[E2]]
 */
void TypeConstraintVisitor::endVisit(ASTBinaryExpr * element) {
  auto op = element->getOp();
  auto intType = std::make_shared<TipInt>();
  auto boolType = std::make_shared<TipBool>();

  if (op == ">" || op == ">=" || op == "<" || op == "<=") {
    // result is bool
    constraintHandler->handle(astToVar(element), boolType);
    // operands are integer
    constraintHandler->handle(astToVar(element->getLeft()), intType);
    constraintHandler->handle(astToVar(element->getRight()), intType);
  }
  else if (op != "==" && op != "!=") {
    // result is int
    constraintHandler->handle(astToVar(element), intType);
    // operands are integer
    constraintHandler->handle(astToVar(element->getLeft()), intType);
    constraintHandler->handle(astToVar(element->getRight()), intType);
  }
  else {
    // result is bool
    constraintHandler->handle(astToVar(element), boolType);
    // operands have the same type
    constraintHandler->handle(astToVar(element->getLeft()), astToVar(element->getRight()));
  }
}

/*! \brief Type constraints for input statement.
 *
 * Type rules for "input":
 *  [[input]] = int
 */
void TypeConstraintVisitor::endVisit(ASTInputExpr * element) {
  constraintHandler->handle(astToVar(element), std::make_shared<TipInt>());
}

/*! \brief Type constraints for function application.
 *
 * Type Rules for "E(E1, ..., En)":
 *  [[E]] = ([[E1]], ..., [[En]]) -> [[E(E1, ..., En)]]
 */
void TypeConstraintVisitor::endVisit(ASTFunAppExpr  * element) {
  std::vector<std::shared_ptr<TipType>> actuals;
  for(auto &a : element->getActuals()) {
    actuals.push_back(astToVar(a));
  }
  constraintHandler->handle(astToVar(element->getFunction()),
                            std::make_shared<TipFunction>(actuals, astToVar(element)));
}

/*! \brief Type constraints for heap allocation.
 *
 * Type Rules for "alloc E":
 *   [[alloc E]] = &[[E]]
 */
void TypeConstraintVisitor::endVisit(ASTAllocExpr * element) {
  constraintHandler->handle(astToVar(element),
                            std::make_shared<TipRef>(astToVar(element->getInitializer())));
}

/*! \brief Type constraints for address of.
 *
 * Type Rules for "&X":
 *   [[&X]] = &[[X]]
 */
void TypeConstraintVisitor::endVisit(ASTRefExpr * element) {
  constraintHandler->handle(astToVar(element),
                            std::make_shared<TipRef>(astToVar(element->getVar())));
}

/*! \brief Type constraints for pointer dereference.
 *
 * Type Rules for "*E":
 *   [[E]] = &[[*E]]
 */
void TypeConstraintVisitor::endVisit(ASTDeRefExpr * element) {
  constraintHandler->handle(astToVar(element->getPtr()),
                            std::make_shared<TipRef>(astToVar(element)));
}

/*! \brief Type constraints for null literal.
 *
 * Type Rules for "null":
 *   [[null]] = & \alpha
 */
void TypeConstraintVisitor::endVisit(ASTNullExpr * element) {
  constraintHandler->handle(astToVar(element),
                            std::make_shared<TipRef>(std::make_shared<TipAlpha>(element)));
}

/*! \brief Type rules for assignments.
 *
 * Type rules for "E1 = E":
 *   [[E1]] = [[E2]]
 *
 * Type rules for "*E1 = E2":
 *   [[E1]] = &[[E2]]
 * 
 * Note that these are slightly more general than the rules in the SPA book.
 * The first allows for record expressions on the left hand side and the second
 * allows for more complex assignments, e.g., "**p = &x"
 */
void TypeConstraintVisitor::endVisit(ASTAssignStmt  * element) {
  // If this is an assignment through a pointer, use the second rule above
  if (auto lptr = dynamic_cast<ASTDeRefExpr*>(element->getLHS())) {
    constraintHandler->handle(astToVar(lptr->getPtr()),
                              std::make_shared<TipRef>(astToVar(element->getRHS())));
  } else {
    constraintHandler->handle(astToVar(element->getLHS()), astToVar(element->getRHS()));
  }
}

/*! \brief Type constraints for while loop.
 *
 * Type rules for "while (E) S":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTWhileStmt * element) {
  constraintHandler->handle(astToVar(element->getCondition()), std::make_shared<TipBool>());
}

/*! \brief Type constraints for if statement.
 *
 * Type rules for "if (E) S1 else S2":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTIfStmt * element) {
  constraintHandler->handle(astToVar(element->getCondition()), std::make_shared<TipBool>());
}

/*! \brief Type constraints for output statement.
 *
 * Type rules for "output E":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTOutputStmt * element) {
  constraintHandler->handle(astToVar(element->getArg()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for record expression.
 *
 * Type rule for "{ X1:E1, ..., Xn:En }":
 *   [[{ X1:E1, ..., Xn:En }]] = { f1:v1, ..., fn:vn }
 * where fi is the ith field in the program's uber record
 * and vi = [[Ei]] if fi = Xi and \alpha otherwise
 */
void TypeConstraintVisitor::endVisit(ASTRecordExpr * element) {
  auto allFields = symbolTable->getFields();
  std::vector<std::shared_ptr<TipType>> fieldTypes;
  for (auto &f : allFields) {
    bool matched = false;
    for (auto &fe : element->getFields()) {
      if (f == fe->getField()) {
        fieldTypes.push_back(astToVar(fe->getInitializer()));
        matched = true;
        break;
      }
    }
    if (matched) continue;

    fieldTypes.push_back(std::make_shared<TipAbsentField>());
  } 
  constraintHandler->handle(astToVar(element), std::make_shared<TipRecord>(fieldTypes, allFields));
}

/*! \brief Type constraints for field access.
 *
 * Type rule for "E.X":
 *   [[E]] = { f1:v1, ..., fn:vn }
 * where fi is the ith field in the program's uber record
 * and vi = [[E.X]] if fi = X and \alpha otherwise
 */
void TypeConstraintVisitor::endVisit(ASTAccessExpr * element) {
  auto allFields = symbolTable->getFields();
  std::vector<std::shared_ptr<TipType>> fieldTypes;
  for (auto &f : allFields) {
    if (f == element->getField()) {
      fieldTypes.push_back(astToVar(element));
    } else {
      fieldTypes.push_back(std::make_shared<TipAlpha>(element, f));
    }
  } 
  constraintHandler->handle(astToVar(element->getRecord()),
                            std::make_shared<TipRecord>(fieldTypes, allFields));
}

/*! \brief Type constraints for error statement.
 *
 * Type rules for "error E":
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTErrorStmt * element) {
  constraintHandler->handle(astToVar(element->getArg()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for true expression.
 *
 * Type rules for "true":
 *   [[true]] = bool
 */
void TypeConstraintVisitor::endVisit(ASTTrueExpr * element) {
    constraintHandler->handle(astToVar(element), std::make_shared<TipBool>());
}

/*! \brief Type constraints for false expression.
 *
 * Type rules for "false":
 *   [[false]] = bool
 */
void TypeConstraintVisitor::endVisit(ASTFalseExpr * element) {
    constraintHandler->handle(astToVar(element), std::make_shared<TipBool>());
}

/*! \brief Type constraints for not expression.
 *
 * Type rules for "not E1":
 *   [[not E1]] = [[E1]] = bool
 */
void TypeConstraintVisitor::endVisit(ASTNotExpr * element) {
  auto boolType = std::make_shared<TipBool>();
  constraintHandler->handle(astToVar(element), boolType);
  constraintHandler->handle(astToVar(element->getExpr()), boolType);
}

/*! \brief Type constraints for and expression.
 *
 * Type rules for "E1 and E2":
 *   [[E1 and E2]] = [[E1]] = [[E2]] = bool
 */
void TypeConstraintVisitor::endVisit(ASTAndExpr * element) {
  auto boolType = std::make_shared<TipBool>();
  constraintHandler->handle(astToVar(element), boolType);
  constraintHandler->handle(astToVar(element->getLeft()), boolType);
  constraintHandler->handle(astToVar(element->getRight()), boolType);
}

/*! \brief Type constraints for or expression.
 *
 * Type rules for "E1 or E2":
 *   [[E1 or E2]] = [[E1]] = [[E2]] = bool
 */
void TypeConstraintVisitor::endVisit(ASTOrExpr * element) {
  auto boolType = std::make_shared<TipBool>();
  constraintHandler->handle(astToVar(element), boolType);
  constraintHandler->handle(astToVar(element->getLeft()), boolType);
  constraintHandler->handle(astToVar(element->getRight()), boolType);
}

/*! \brief Type constraints for increment statement.
 *
 * Type rules for "E1++":
 *   [[E1]] = int
 */
void TypeConstraintVisitor::endVisit(ASTIncrementStmt * element) {
  constraintHandler->handle(astToVar(element->getArg()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for decrement statement.
 *
 * Type rules for "E1--":
 *   [[E1]] = int
 */
void TypeConstraintVisitor::endVisit(ASTDecrementStmt * element) {
  constraintHandler->handle(astToVar(element->getArg()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for range for loop.
 *
 * Type rules for "for (E1 : E2) S":
 *   [[E1]] = type of the elements in E2
 *   [[E2]] = array
 *
 */
void TypeConstraintVisitor::endVisit(ASTForIterStmt * element) {
  /*std::vector<ASTExpr*> entries = dynamic_cast<ASTArrayExpr*>(element->getRight())->getEntries();

  if (entries.size()==0)
  {
    constraintHandler->handle(astToVar(element->getLeft()), std::make_shared<TipAlpha>());
    constraintHandler->handle(astToVar(element->getRight()), std::make_shared<TipArray>(std::make_shared<TipAlpha>()));
  }
  else
  {
    auto firstType = astToVar(entries[0]);
    constraintHandler->handle(astToVar(element->getLeft()), firstType);
    constraintHandler->handle(astToVar(element->getRight()), std::make_shared<TipArray>());
  }*/
}

/*! \brief Type constraints for range for loop.
 *
 * Type rules for "for (E1 : E2 .. E3) S":
 *   [[E1]] = [[E2]] = [[E3]] = int
 *
 * Type rules for "for (E1 : E2 .. E3 by E4) S":
 *   [[E1]] = [[E2]] = [[E3]] = [[E4]] = int
 */
void TypeConstraintVisitor::endVisit(ASTForRangeStmt * element) {
  auto intType = std::make_shared<TipInt>();
  constraintHandler->handle(astToVar(element->getOne()), intType);
  constraintHandler->handle(astToVar(element->getTwo()), intType);
  constraintHandler->handle(astToVar(element->getThree()), intType);

  if (element->getFour() != nullptr)
  {
    constraintHandler->handle(astToVar(element->getFour()), intType);
  }
}

/*! \brief Type constraints for ternary expression.
 *
 * Type rules for "E1 ? E2 : E3":
 *   [[E1]] = bool
 *   [[E1 ? E2 : E3]] = [[E2]] = [[E3]]
 */
void TypeConstraintVisitor::endVisit(ASTTernaryExpr * element) {
  auto ternaryType = astToVar(element->getThen());

  constraintHandler->handle(astToVar(element), ternaryType);
  constraintHandler->handle(astToVar(element->getCond()), std::make_shared<TipBool>());
  constraintHandler->handle(astToVar(element->getThen()), ternaryType);
  constraintHandler->handle(astToVar(element->getElse()), ternaryType);
}

/*! \brief Type constraints for of array.
 *
 * Type rules for "[E1 of E2]":
 *   [[E1]] = [[E2]] = int
 *   [[[E1 of E2]]] = array of int
 */
void TypeConstraintVisitor::endVisit(ASTOfArrayExpr * element) {
  auto intType = std::make_shared<TipInt>();

  constraintHandler->handle(astToVar(element), std::make_shared<TipArray>(std::make_shared<TipInt>()));
  constraintHandler->handle(astToVar(element->getLeft()), intType);
  constraintHandler->handle(astToVar(element->getRight()), intType);
}
/*! \brief Type constraints for array expression.
 *
 * Type rule for "[E1, E2, E3, ..., En]":
 *   [[[E1, E2, E3, ..., En]]] = array
 * [[E1]] = [[E2]] = ... = [[En]]
 */
void TypeConstraintVisitor::endVisit(ASTArrayExpr * element) {
  std::vector<ASTExpr*> entries = element->getEntries();
  if (entries.size()==0) {
    constraintHandler->handle(astToVar(element), 
                    std::make_shared<TipArray>(std::make_shared<TipAlpha>(element)));
  }
  else {
    auto firstType = astToVar(entries[0]);
    for (auto &e : element->getEntries()) {
      constraintHandler->handle(astToVar(e), firstType);
    } 
    constraintHandler->handle(astToVar(element), std::make_shared<TipArray>(firstType));
  }
}

/*! \brief Type constraints for array element reference expression.
 *
 * Type rule for "E1[E2]":
 *   [[E1[E2]]] = the type of the elements in the array E1
 * [[E2]] = int
 */
void TypeConstraintVisitor::endVisit(ASTElementRefrenceOperatorExpr * element) {
  auto arr = dynamic_cast<ASTArrayExpr*>(element->getArray());
  auto entries = arr->getEntries();
  auto firstType = astToVar(entries[0]);
  constraintHandler->handle(astToVar(element), firstType);
  constraintHandler->handle(astToVar(element->getIndex()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for negation expression.
 *
 * Type rules for "-(E)":  
 *   [[E]] = int
 */
void TypeConstraintVisitor::endVisit(ASTNegationExpr * element) {
    constraintHandler->handle(astToVar(element->getExpr()), std::make_shared<TipInt>());
}

/*! \brief Type constraints for array length expression.
 *
 * Type rules for "#E":  
 *   [[E]] = array
 */
void TypeConstraintVisitor::endVisit(ASTArrayLengthExpr * element) {
    constraintHandler->handle(astToVar(element->getArray()), 
                    std::make_shared<TipArray>(std::make_shared<TipAlpha>(element)));
}
