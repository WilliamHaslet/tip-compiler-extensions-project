#pragma once

#include "ASTStmt.h"
#include "ASTExpr.h"

/*! \brief Class for an interator for loop.
 */
class ASTForRangeStmt : public ASTStmt {
  std::unique_ptr<ASTExpr> ONE;
  std::unique_ptr<ASTExpr> TWO;
  std::unique_ptr<ASTExpr> THREE;
  std::unique_ptr<ASTExpr> FOUR;
  std::unique_ptr<ASTStmt> BODY;
public:
  ASTForRangeStmt(std::unique_ptr<ASTExpr> ONE, std::unique_ptr<ASTExpr> TWO, 
      std::unique_ptr<ASTExpr> THREE, std::unique_ptr<ASTExpr> FOUR, std::unique_ptr<ASTStmt> BODY)
      : ONE(std::move(ONE)), TWO(std::move(TWO)), 
      THREE(std::move(THREE)), FOUR(std::move(FOUR)), BODY(std::move(BODY)) {}
  ASTExpr* getOne() const { return ONE.get(); }
  ASTExpr* getTwo() const { return TWO.get(); }
  ASTExpr* getThree() const { return THREE.get(); }
  ASTExpr* getFour() const { return FOUR.get(); }
  ASTStmt* getBody() const { return BODY.get(); }
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};
