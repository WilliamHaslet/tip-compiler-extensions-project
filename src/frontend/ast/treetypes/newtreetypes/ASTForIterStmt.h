#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for an interator for loop.
 */
class ASTForIterStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> LEFT;
  std::unique_ptr<ASTExpr> RIGHT;
  std::unique_ptr<ASTStmt> BODY;
public:
  ASTForIterStmt(std::unique_ptr<ASTExpr> LEFT, std::unique_ptr<ASTExpr> RIGHT, std::unique_ptr<ASTStmt> BODY)
      : LEFT(std::move(LEFT)), RIGHT(std::move(RIGHT)), BODY(std::move(BODY)) {}
  ASTExpr* getLeft() const { return LEFT.get(); }
  ASTExpr* getRight() const { return RIGHT.get(); }
  ASTStmt* getBody() const { return BODY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
