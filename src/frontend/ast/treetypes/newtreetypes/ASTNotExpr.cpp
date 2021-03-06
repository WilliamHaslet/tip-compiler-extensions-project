#include "ASTNotExpr.h"
#include "ASTVisitor.h"

void ASTNotExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getExpr()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTNotExpr::print(std::ostream &out) const {
  out << "(not " << *getExpr() << ")";
  return out;
}
