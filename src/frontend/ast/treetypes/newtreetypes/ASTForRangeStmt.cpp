#include "ASTForRangeStmt.h"
#include "ASTVisitor.h"

void ASTForRangeStmt::accept(ASTVisitor * visitor) {
  if (visitor->visit(this)) {
    getOne()->accept(visitor);
    getTwo()->accept(visitor);
    getThree()->accept(visitor);

    if (getFour() != nullptr)
    {
      getFour()->accept(visitor);
    }
  }
  visitor->endVisit(this);
}

llvm::Value* ASTForRangeStmt::codegen()
{
  return nullptr;
}

std::ostream& ASTForRangeStmt::print(std::ostream &out) const {
  if (getFour() != nullptr)
  {
    out << "for (" << *getOne() << " : " << *getTwo() << " .. " << *getThree() << " by " << *getFour() << ") " << *getBody();
  }
  else
  {
    out << "for (" << *getOne() << " : " << *getTwo() << " .. " << *getThree() << ") " << *getBody();
  }
  return out;
}
