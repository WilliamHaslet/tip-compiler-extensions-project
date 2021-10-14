#include "ASTAndExpr.h"
#include "ASTVisitor.h"

void ASTAndExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTAndExpr::codegen()
{
  return nullptr;
}

std::ostream& ASTAndExpr::print(std::ostream &out) const {
  out << "(" << *getLeft() << " and " << *getRight() << ")";
  return out;
}  // LCOV_EXCL_LINE
