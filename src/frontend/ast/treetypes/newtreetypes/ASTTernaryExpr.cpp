#include "ASTTernaryExpr.h"
#include "ASTVisitor.h"

void ASTTernaryExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getCond()->accept(visitor);
    getThen()->accept(visitor);
    getElse()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTTernaryExpr::codegen(){
  return nullptr;
}

std::ostream& ASTTernaryExpr::print(std::ostream &out) const {
  out << "(" << *getCond() << " ? " << *getThen() << " : " << *getElse() << ")";
  return out;
}  // LCOV_EXCL_LINE