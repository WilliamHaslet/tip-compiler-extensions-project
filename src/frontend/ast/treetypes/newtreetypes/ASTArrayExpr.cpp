#include "ASTArrayExpr.h"
#include "ASTVisitor.h"
#include "ASTinternal.h"

std::vector<ASTExpr*> ASTArrayExpr::getEntries() const {
  return rawRefs(ENTRIES);
}

void ASTArrayExpr::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    for (auto f : getEntries()) {
      f->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

llvm::Value* ASTArrayExpr::codegen()
{
  return nullptr;
}

std::ostream& ASTArrayExpr::print(std::ostream &out) const {
  out << "[";
  bool skip = true;
  for (auto &f : getEntries()) {
    if (skip) {
      skip = false;
      out << *f;
      continue;
    } 
    out << "," << *f;
  } 
  out << "]";
  return out;
}  // LCOV_EXCL_LINE
