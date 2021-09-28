#include "ASTOrExpr.h"
#include "ASTVisitor.h"

void ASTOrExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTOrExpr::print(std::ostream &out) const {
  out << "(" << *getLeft() << " or " << *getRight() << ")";
  return out;
}  // LCOV_EXCL_LINE
