#include "ASTIncrementStmt.h"
#include "ASTVisitor.h"

void ASTIncrementStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArg()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTIncrementStmt::codegen()
{
  return nullptr;
}

std::ostream& ASTIncrementStmt::print(std::ostream &out) const {
  out << *getArg() << "++;";
  return out;
}

