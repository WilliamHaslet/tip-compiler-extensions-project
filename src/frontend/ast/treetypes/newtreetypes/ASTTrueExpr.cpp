#include "ASTTrueExpr.h"
#include "ASTVisitor.h"

#include <iostream>

void ASTTrueExpr::accept(ASTVisitor * visitor) {
  visitor->visit(this);
  visitor->endVisit(this);
}

std::ostream& ASTTrueExpr::print(std::ostream &out) const {
  out << "true";
  return out;
}