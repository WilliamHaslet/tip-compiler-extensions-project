#include "ASTOfArrayExpr.h"
#include "ASTVisitor.h"

void ASTOfArrayExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getLeft()->accept(visitor);
    getRight()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTOfArrayExpr::codegen()
{
  return nullptr;
}

std::ostream& ASTOfArrayExpr::print(std::ostream &out) const {
  out << "[" << *getLeft() << " of " << *getRight() << "]";
  return out;
}  // LCOV_EXCL_LINE
