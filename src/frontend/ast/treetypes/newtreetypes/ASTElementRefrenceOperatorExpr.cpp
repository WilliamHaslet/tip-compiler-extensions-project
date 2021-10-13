#include "ASTElementRefrenceOperatorExpr.h"
#include "ASTVisitor.h"

void ASTElementRefrenceOperatorExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getArray()->accept(visitor);
    getIndex()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTElementRefrenceOperatorExpr::codegen(){
  return nullptr;
}

std::ostream& ASTElementRefrenceOperatorExpr::print(std::ostream &out) const {
  out << "(" << *getArray() << "[" << *getIndex() << "]" << ")";
  return out;
}  // LCOV_EXCL_LINE