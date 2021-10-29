#include "ASTNegationExpr.h"
#include "ASTVisitor.h"

void ASTNegationExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getExpr()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTNegationExpr::print(std::ostream &out) const {
  out << "-(" << *getExpr() << ")";
  return out;
}
