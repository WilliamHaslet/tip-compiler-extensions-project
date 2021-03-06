#include "CheckAssignable.h"
#include "PrettyPrinter.h"
#include "SemanticError.h"

#include <sstream>

namespace {

// Return true if expression has an l-value
bool isAssignable(ASTExpr* e) {
  if (dynamic_cast<ASTVariableExpr*>(e)) return true;
  if (dynamic_cast<ASTAccessExpr*>(e)){
    ASTAccessExpr* access = dynamic_cast<ASTAccessExpr*>(e);
    if (dynamic_cast<ASTVariableExpr*>(access->getRecord())){
      return true;
    }
    else if (dynamic_cast<ASTDeRefExpr*>(access->getRecord())){
      return true;
    }
    else{
      return false;
    }
  }
  return false;
}

}

void CheckAssignable::endVisit(ASTAssignStmt* element) {
  if (isAssignable(element->getLHS())) return;

  // Assigning through a pointer is also permitted
  if (dynamic_cast<ASTDeRefExpr*>(element->getLHS())) return;

  if (dynamic_cast<ASTElementRefrenceOperatorExpr*>(element->getLHS())) return;

  std::ostringstream oss;
  oss << "Assignment error on line " << element->getLine() << ": ";
  if(dynamic_cast<ASTAccessExpr*>(element->getLHS())){
    ASTAccessExpr* access = dynamic_cast<ASTAccessExpr*>(element->getLHS());
    oss << *access->getRecord() << " is an expression, and not a variable corresponding to a record\n";  
  }
  else{
    oss << *element->getLHS() << " not an l-value\n";
  }
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTRefExpr* element) {
  if (isAssignable(element->getVar())) return;

  if (dynamic_cast<ASTElementRefrenceOperatorExpr*>(element->getVar())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getVar() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTIncrementStmt* element) {
  if (isAssignable(element->getArg())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getArg() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTDecrementStmt* element) {
  if (isAssignable(element->getArg())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getArg() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTForRangeStmt* element) {
  if (isAssignable(element->getOne())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getOne() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::endVisit(ASTForIterStmt* element) {
  if (isAssignable(element->getLeft())) return;

  std::ostringstream oss;
  oss << "Address of error on line " << element->getLine() << ": ";
  oss << *element->getLeft() << " not an l-value\n";
  throw SemanticError(oss.str());
}

void CheckAssignable::check(ASTProgram* p) {
  CheckAssignable visitor;
  p->accept(&visitor);
}
