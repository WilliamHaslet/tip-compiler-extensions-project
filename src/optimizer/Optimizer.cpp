#include "Optimizer.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"

using namespace llvm;

void Optimizer::optimize(Module* theModule, std::string disableOpt) {
  // Create a pass manager to simplify generated module
  auto TheFPM = std::make_unique<legacy::FunctionPassManager>(theModule);

  if (!(disableOpt=="cpmtrp")){
    // Promote allocas to registers.
    TheFPM->add(createPromoteMemoryToRegisterPass());
  }

  if (!(disableOpt=="cicp")){
    // Do simple "peephole" optimizations
    TheFPM->add(createInstructionCombiningPass());
  }

  if (!(disableOpt=="crp")){
    // Reassociate expressions.
    TheFPM->add(createReassociatePass());
  }

  if (!(disableOpt=="cgp")){
    // Eliminate Common SubExpressions.
    TheFPM->add(createGVNPass());
  }

  if (!(disableOpt=="ccsp")){
    // Simplify the control flow graph (deleting unreachable blocks, etc).
    TheFPM->add(createCFGSimplificationPass());
  }

  if (!(disableOpt=="clup")){
    // Simple loop unrolling
    TheFPM->add(createLoopUnrollPass());
  }


  // initialize and run simplification pass on each function
  TheFPM->doInitialization();
  for (auto &fun : theModule->getFunctionList()) {
    TheFPM->run(fun);
  }
}