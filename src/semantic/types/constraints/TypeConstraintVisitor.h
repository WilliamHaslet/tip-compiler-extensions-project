#pragma once

#include "ASTVisitor.h"
#include "ConstraintHandler.h"
#include "SymbolTable.h"
#include "TipType.h"
#include <memory>
#include <set>
#include <stack>
#include <string>
#include <vector>

/*! \class TypeConstraintVisitor
 *  \brief Visitor generates type constraints and processes them.
 *
 * This visitor performs a post-order visit of the program AST.  It
 * constructs type constraints for each node that requires a type judgement.
 * The constraints are then processed by a concrete implemntation of
 * A ConstraintHandler. This provides flexibility in using the visitor - it
 * can simply record the constraints or it can solve them on the fly.
 */
class TypeConstraintVisitor: public ASTVisitor {
public:
    TypeConstraintVisitor() = delete;

    /**
     * Construct a TypeConstraintVisitor from the given SymbolTable and ConstraintHandler.
     *
     * Subclasses define the handler (eg. TypeConstraintUnifyVisitor defines the ConstraintUnfier
     * as its handler) and pass it up to the base class during construction. This allows us to
     * not call virtual methods in the constructor.
     *
     * Scott Meyers defines this pattern in his Effective C++ series. You can read more about it here,
     * https://www.aristeia.com/EC3E/3E_item9.pdf.
     */
    TypeConstraintVisitor(SymbolTable* st, std::unique_ptr<ConstraintHandler> handler);

    bool visit(ASTFunction * element) override;
    void endVisit(ASTAccessExpr * element) override;
    void endVisit(ASTAllocExpr * element) override;
    void endVisit(ASTAssignStmt * element) override;
    void endVisit(ASTBinaryExpr * element) override;
    void endVisit(ASTDeRefExpr * element) override;
    void endVisit(ASTErrorStmt * element) override;
    void endVisit(ASTFunAppExpr * element) override;
    void endVisit(ASTFunction * element) override;
    void endVisit(ASTIfStmt * element) override;
    void endVisit(ASTInputExpr * element) override;
    void endVisit(ASTNullExpr * element) override;
    void endVisit(ASTNumberExpr * element) override;
    void endVisit(ASTOutputStmt * element) override;
    void endVisit(ASTRecordExpr * element) override;
    void endVisit(ASTRefExpr * element) override;
    void endVisit(ASTWhileStmt * element) override;
    
    void endVisit(ASTElementRefrenceOperatorExpr * element) override;
    void endVisit(ASTArrayLengthExpr * element) override;
    void endVisit(ASTArrayExpr * element) override;
    void endVisit(ASTNegationExpr * element) override;

    void endVisit(ASTTrueExpr * element) override;
    void endVisit(ASTFalseExpr * element) override;
    void endVisit(ASTNotExpr * element) override;
    void endVisit(ASTAndExpr * element) override;
    void endVisit(ASTOrExpr * element) override;
    void endVisit(ASTIncrementStmt * element) override;
    void endVisit(ASTDecrementStmt * element) override;
    void endVisit(ASTForIterStmt * element) override;
    void endVisit(ASTForRangeStmt * element) override;
    void endVisit(ASTTernaryExpr * element) override;
    void endVisit(ASTOfArrayExpr * element) override;

    // E1 ? E2 : E3
    // [E1 of E2]
    // [E1, ..., En]
    // E1[E2]
    // #[]
    // E1++
    // E1--
    
    // true
    // false
    // and
    // or
    // >=
    // <
    // <=
    // %
    // not
    // -(E1)
    // for (E1 : E2 .. E3 by E4)
    // for (E1 : E2)

protected:
    std::unique_ptr<ConstraintHandler> constraintHandler;

private:
    std::stack<ASTDeclNode *> scope;
    SymbolTable *symbolTable;
    std::shared_ptr<TipType> astToVar(ASTNode * n);
};
