#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for an increment statement.
 */
class ASTIncrementStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> ARG;
public:
  ASTIncrementStmt(std::unique_ptr<ASTExpr> ARG) : ARG(std::move(ARG)) {}
  ASTExpr* getArg() const { return ARG.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
