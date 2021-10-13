#pragma once

#include "ASTExpr.h"

/*! \brief Class for the unary on an expression
 */
class ASTNegationExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> VAR;
public:
  ASTNegationExpr(std::unique_ptr<ASTExpr> VAR) : VAR(std::move(VAR)) {}
  ASTExpr* getExpr() const { return VAR.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
