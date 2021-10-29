#include "ASTForIterStmt.h"
#include "ASTVisitor.h"

void ASTForIterStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

std::ostream& ASTForIterStmt::print(std::ostream &out) const {
  out << "for (" << *getLeft() << " : " << *getRight() << ") " << *getBody();
  return out;
}
