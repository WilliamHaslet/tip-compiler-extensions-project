#pragma once

#include "ASTExpr.h"

/*! \brief Class for a element reference in arrays.
 */
class ASTElementRefrenceOperatorExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> ARRAY, INDEX;
public:
  ASTElementRefrenceOperatorExpr(std::unique_ptr<ASTExpr> ARRAY,
             std::unique_ptr<ASTExpr> INDEX)
      : ARRAY(std::move(ARRAY)), INDEX(std::move(INDEX)) {}
  ASTExpr* getArray() const { return ARRAY.get(); }
  ASTExpr* getIndex() const { return INDEX.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};