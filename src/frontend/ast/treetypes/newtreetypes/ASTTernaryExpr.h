#pragma once

#include "ASTExpr.h"

/*! \brief Class for a ternary operator.
 */
class ASTTernaryExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> COND, THEN, ELSE;
public:
  ASTTernaryExpr(std::unique_ptr<ASTExpr> COND,
             std::unique_ptr<ASTExpr> THEN, std::unique_ptr<ASTExpr> ELSE)
      : COND(std::move(COND)), THEN(std::move(THEN)), ELSE(std::move(ELSE)) {}
  ASTExpr* getCond() const { return COND.get(); }
  ASTExpr* getThen() const { return THEN.get(); }
  ASTExpr* getElse() const { return ELSE.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};