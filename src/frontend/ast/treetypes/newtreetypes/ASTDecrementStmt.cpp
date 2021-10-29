#include "ASTDecrementStmt.h"
#include "ASTVisitor.h"

void ASTDecrementStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTDecrementStmt::print(std::ostream &out) const {
  out << *getArg() << "--;";
  return out;
}
