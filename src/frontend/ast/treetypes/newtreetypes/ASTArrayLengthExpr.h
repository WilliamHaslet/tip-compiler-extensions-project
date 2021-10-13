#pragma once

#include "ASTExpr.h"

/*! \brief Class for getting the length of an array
 */
class ASTArrayLengthExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> ARRAY;
public:
  ASTArrayLengthExpr(std::unique_ptr<ASTExpr> ARRAY) : ARRAY(std::move(ARRAY)) {}
  ASTExpr* getArray() const { return ARRAY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};