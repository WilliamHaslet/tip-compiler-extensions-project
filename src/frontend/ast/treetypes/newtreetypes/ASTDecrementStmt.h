#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for an decrement statement.
 */
class ASTDecrementStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> ARG;
public:
  ASTDecrementStmt(std::unique_ptr<ASTExpr> ARG) : ARG(std::move(ARG)) {}
  ASTExpr* getArg() const { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
