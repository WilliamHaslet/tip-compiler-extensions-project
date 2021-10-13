#include "ASTTrueExpr.h"
#include "ASTVisitor.h"

#include <iostream>

void ASTTrueExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

llvm::Value* ASTTrueExpr::codegen(){
  return nullptr;
}

std::ostream& ASTTrueExpr::print(std::ostream &out) const {
  out << "true";
  return out;
}