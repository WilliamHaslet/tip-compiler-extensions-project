#include "ASTArrayLengthExpr.h"
#include "ASTVisitor.h"

void ASTArrayLengthExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArray()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTArrayLengthExpr::codegen(){
  return nullptr;
}

std::ostream& ASTArrayLengthExpr::print(std::ostream &out) const {
  out << "(#" << *getArray() << ")";
  return out;
}
