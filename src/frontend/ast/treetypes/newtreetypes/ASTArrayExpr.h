#pragma once

#include "ASTExpr.h"

/*! \brief Class for defining a standard array.
 */
class ASTArrayExpr : public ASTExpr {
  std::vector<std::unique_ptr<ASTExpr>> ENTRIES;
public:
  ASTArrayExpr(std::vector<std::unique_ptr<ASTExpr>> ENTRIES)
      : ENTRIES(std::move(ENTRIES)) {}
  std::vector<ASTExpr*> getEntries() const;
  void accept(ASTVisitor * visitor) override;
  llvm::Value* codegen() override;

protected:
  std::ostream& print(std::ostream &out) const override;
};