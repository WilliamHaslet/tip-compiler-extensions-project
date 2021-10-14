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

    getBody()->accept(visitor);
  }
  visitor->endVisit(this);
}

llvm::Value* ASTForRangeStmt::codegen()
{
  return nullptr;
}

std::ostream& ASTForRangeStmt::print(std::ostream &out) const
{
  out << "for (" << *getOne() << " : " << *getTwo() << " .. " << *getThree();
  
  if (getFour() != nullptr)
  {
    out << " by " << *getFour();
  }

  out << ") " << *getBody();
  return out;
}
