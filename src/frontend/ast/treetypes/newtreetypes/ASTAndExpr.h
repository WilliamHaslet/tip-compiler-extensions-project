#pragma once

#include "ASTExpr.h"

/*! \brief Class for a binary and operator.
 */
class ASTAndExpr : public ASTExpr {
  std::unique_ptr<ASTExpr> LEFT, RIGHT;
public:
  ASTAndExpr(std::unique_ptr<ASTExpr> LEFT, std::unique_ptr<ASTExpr> RIGHT)
      : LEFT(std::move(LEFT)), RIGHT(std::move(RIGHT)) {}
  ASTExpr* getLeft() const { return LEFT.get(); }
  ASTExpr* getRight() const { return RIGHT.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
